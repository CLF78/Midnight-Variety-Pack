#!/usr/bin/env python3

# preprocess.py
# Preprocessor for Kamek code

from argparse import ArgumentParser
from pathlib import Path

from mangle import mangle_function

REPLACE_STRING = 'REPLACE'
REPLACED_STRING = 'REPLACED'
MAGIC_OPWORD = '0x78787878'
THUNK_FUNCTION_PATTERN = 'thunk_replaced'
TYPEDEFS = {
    'u8': 'unsigned char',
    'u16': 'unsigned short',
    'u32': 'unsigned int',
    'u64': 'unsigned long long',
    's8': 'signed char',
    's16': 'signed short',
    's32': 'signed int',
    's64': 'signed long long',
    'f32': 'float',
    'f64': 'double',
    'size_t': 'unsigned long',
    'ulong': 'unsigned long',
}

def getFunctionBodyLength(src: str, start_pos: int, maxlen: int) -> int:
    brace_count = 1
    curr_pos = start_pos + 1
    while brace_count > 0 and curr_pos < maxlen:

        # Skip comments, now with shitty hardcode!
        if src[curr_pos:curr_pos+2] == '//' and src[curr_pos-1] != ':':
            curr_pos = src.index('\n', curr_pos)
            continue
        elif src [curr_pos:curr_pos+2] == '/*':
            curr_pos = src.index('*/', curr_pos)
            continue

        # Detect braces to properly include the entire body
        if src[curr_pos] == '{':
            brace_count += 1
        elif src[curr_pos] == '}':
            brace_count -= 1
        curr_pos += 1

    if brace_count == 0:
        return curr_pos - start_pos
    else:
        return -1  # Function body end not found

def splitFunction(signature: str) -> tuple[str, str, str, str]:

    # Clean up the signature first
    signature = signature.strip()

    # Split the arguments and account for empty args
    start, args = signature.split('(', 1)
    if len(args) == 1:
        args = ''
    else:
        args = args[0:-1]

    # Split the return type
    retsplit = start.split()
    returntype = ' '.join(retsplit[0:-1])
    name = retsplit[-1]

    # Split the classes from the function name and merge them
    namespl = name.split('::')
    name = namespl[-1]
    clsname = '::'.join(namespl[0:-1])

    # Return the split values
    return (returntype, clsname, name, args)

def createFunctionThunk(mangledFuncName: str, funcClass: str, funcReturn: str, funcArgs: str, symbolAddr: str) -> tuple[str, str]:

    # Convert the symbol to an integer value
    symbolAddr = int(symbolAddr, 16)

    # Create the thunk function
    thunkFuncName = f'{THUNK_FUNCTION_PATTERN}_{mangledFuncName}'
    thunk = f'\nextern "C" asm {funcReturn} {thunkFuncName}('

    # Add the class if present
    if funcClass:
        thunk += f'{funcClass}* self'

        # Add the comma for the extra args
        if funcArgs:
            thunk += ', '

    # Add the extra arguments
    if funcArgs:
        thunk += funcArgs

    # End the signature and add the body
    thunk += ') { nofralloc; opword '
    thunk += MAGIC_OPWORD
    thunk += '; blr; }\n'

    # Add the fake branch and the patch exit hooks
    thunk += f'kmBranch({hex(symbolAddr+4)}, {thunkFuncName});\n'
    thunk += f'kmPatchExitPoint({thunkFuncName}, {hex(symbolAddr + 4)});\n\n'
    return thunkFuncName, thunk

def process_file(src: Path, symbol_file: Path, dest: Path) -> bool:

    # Check if the source and symbol files exist
    if not src.is_file():
        print('ERROR: Source file not found!')
        return False

    if not symbol_file.is_file():
        print('ERROR: Symbol map not found!')
        return False

    # Read all symbols from the map
    symbols = {}
    with symbol_file.open(encoding='utf-8') as f:
        for line in f:
            line = line.rstrip().split('=')
            symbols[line[0]] = line[1]

    # Read the source file
    src_code = src.read_text(encoding='utf-8')

    # Set up loop
    srcCodePos = 0
    dest_code = ''
    missing_syms = []
    while srcCodePos < len(src_code):

        # Get the current line
        endLinePos = src_code.index('\n', srcCodePos) + 1
        line = src_code[srcCodePos:endLinePos]

        # Strip comments, now with shitty hardcode
        if line.find('//') != -1 and line[line.find('//')-1] != ':':
            line = line[0:line.index('//')]
        elif line.find('/*') != -1:
            line = line[0:line.index('/*')]
            endLinePos = src_code.index('*/', srcCodePos) + 1

        # Find the magic word in the current line
        # If not found, add all non-empty lines to the file
        if REPLACE_STRING not in line:
            line = line.rstrip()
            if line:
                dest_code += line
                dest_code += '\n'
            srcCodePos = endLinePos
            continue
        else:
            srcCodePos += line.index(REPLACE_STRING)

        # Get the function signature
        funcSignatureStart = srcCodePos + len(REPLACE_STRING) + 1
        funcStart = src_code.index('{', funcSignatureStart)
        funcSignature = src_code[funcSignatureStart:funcStart]

        # Get the function body with failsafe
        funcBodyLen = getFunctionBodyLength(src_code, funcStart, len(src_code))
        if funcBodyLen == -1:
            raise ValueError("Function end not found!")
        funcBody = src_code[funcStart:funcStart+funcBodyLen]
        srcCodePos = funcStart + funcBodyLen

        # Split the function signature
        funcReturn, funcClass, funcName, funcArgs = splitFunction(funcSignature)

        # Try mangling the signature (let exceptions propagate)
        mangledFuncSignature = mangle_function(funcSignature, TYPEDEFS)

        # Find the corresponding symbol (try with mangled first, then demangled name if applicable)
        isExtern = False
        if mangledFuncSignature not in symbols and not funcClass:
            mangledFuncSignature = funcName
            isExtern = True

        # If the symbol was not found, add it to the missing list which we can display later
        if mangledFuncSignature not in symbols:
            missing_syms.append(mangledFuncSignature)
            continue

        # Get the address
        symbolAddr = symbols[mangledFuncSignature]

        # Find instances of the replaced keyword in the function body
        # If found, create the thunk to call the original game function and add it to the destination code
        if funcBody.find(REPLACED_STRING) != -1:
            thunkFuncName, thunk = createFunctionThunk(mangledFuncSignature, funcClass, funcReturn, funcArgs, symbolAddr)
            dest_code += thunk

            # Replace the keyword properly
            toReplace = REPLACED_STRING
            replacement = thunkFuncName
            if funcClass:
                replacement += f'(this{", " if funcArgs else ""}'
                toReplace += '('

            # Do the substitution in the whole function body
            funcBody = funcBody.replace(toReplace, replacement)

        # Write the branch hook
        branchHook = f'kmBranch({symbolAddr}, {mangledFuncSignature});\n'
        if not isExtern:
            branchHook = f'\nextern "C" void {mangledFuncSignature}();\n' + branchHook
        dest_code += branchHook

        # Copy the function signature and the body
        dest_code += funcSignature
        dest_code += funcBody
        dest_code += '\n'

    # If any symbols were not found, print an error and do not continue
    if missing_syms:
        print('ERROR: The following symbols were not found:', ','.join(missing_syms))
        return False

    # Write all the data to the output file
    dest.write_text(dest_code, encoding='utf-8')
    return True

if __name__ == '__main__':

    # Get arguments
    parser = ArgumentParser(description='Preprocessor for Kamek code')
    parser.add_argument('source', type=Path, help='The file to be preprocessed')
    parser.add_argument('-m', '--map', type=Path, required=True, help='The path to the symbol map')
    parser.add_argument('-o', '--out', type=Path, required=True, help='The path to the output file')
    args = parser.parse_args()
    process_file(args.source, args.map, args.out)
