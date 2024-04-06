#!/usr/bin/env python3

# preprocess.py
# Preprocessor for Kamek code

from argparse import ArgumentParser
from io import StringIO
from pathlib import Path
import mangle

REPLACE_STRING = 'REPLACE'
REPLACE_STATIC_STRING = 'REPLACE_STATIC'
REPLACED_STRING = 'REPLACED'
BRANCH_MANGLE = 'BRANCH_CPP'
CALL_MANGLE = 'CALL_CPP'
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
    'BOOL': 'signed int',
    'Func': 'void (*)()',
}

NEST_KEYWORDS = {
    'namespace ',
    'class ',
    'struct ',
    'enum ',
}

NEST_KEYWORD_TERMINATORS = {
    '{',
    ';',
}

class _Preprocessor:
    def __init__(self, src: Path, symbol_file: Path, include_dir: Path, dest: Path):

        # Import arguments
        self.src = src
        self.symbol_file = symbol_file
        self.dest = dest
        self.include_dir = include_dir

        # Set up the rest
        self.buffer = StringIO()
        self.curr_namespaces = []
        self.detected_types = {}
        self.symbols = {}
        self.missing_symbols = []
        self.externdefs = []


    def read_symbol_file(self) -> None:
        with self.symbol_file.open(encoding='utf-8') as f:
            for line in f:
                line = line.rstrip().split('=')
                self.symbols[line[0]] = line[1]


    def check_sequence(self, src_code: str, curr_pos: int, sequence: str) -> bool:
        return src_code[curr_pos:curr_pos+len(sequence)] == sequence


    def parse_string_literal(self, src_code: str, char: str, curr_pos: int, copy: bool = False) -> int:

        # Set up loop
        start_pos = curr_pos

        # Find the next quote
        while True:
            curr_pos = src_code.index(char, curr_pos + 1)

            # Ensure it's not escaped and the escaped character is itself not escaped
            if src_code[curr_pos-1] != '\\' or src_code[curr_pos-2] == '\\':

                # Write the literal if requested
                if copy:
                    self.buffer.write(src_code[start_pos:curr_pos+1])

                # Return updated position
                return curr_pos + 1


    def parse_comment(self, src_code: str, curr_pos: int, copy: bool = False) -> int:

        # Find the next newline character
        comment_end = src_code.find('\n', curr_pos)

        # Ensure this works for the last line as well
        if comment_end == -1:
            comment_end = len(src_code) - 1

        # Copy the line if requested
        if copy:
            self.buffer.write(src_code[curr_pos:comment_end+1])

        # Return updated position
        return comment_end + 1


    def parse_multiline_comment(self, src_code: str, curr_pos: int, copy: bool = False) -> int:

        # Find end of comment
        comment_end = src_code.index('*/', curr_pos)

        # Copy comment if requested
        if copy:
            self.buffer.write(src_code[curr_pos:comment_end+2])

        # Return updated position
        return comment_end + 2


    def try_parse_keyword(self, src_code: str, curr_pos: int) -> tuple[str, int]:

        # Default to empty type and current position
        type = ''
        type_end_idx = curr_pos

        # Test for each keyword
        for keyword in NEST_KEYWORDS:
            keyword_end = curr_pos + len(keyword)
            if src_code[curr_pos:keyword_end] == keyword:

                # Find the closest terminator
                type_end_idx = len(src_code)
                for terminator in NEST_KEYWORD_TERMINATORS:
                    term_idx = src_code.find(terminator, keyword_end)
                    if term_idx != -1 and term_idx < type_end_idx:
                        type_end_idx = term_idx

                # Read the type
                type = src_code[keyword_end:type_end_idx].strip()

                # Remove any inheritances and exit the loop
                type = type.split(': ')[0].strip()
                break

        # Return both the type and the updated position
        return type, type_end_idx


    def process_header(self, src: Path) -> None:

        # Check if the header file exists
        if not src.is_file():
            raise SystemExit(f'ERROR: Header file {src} not found!')

        # Read the file
        src_code = src.read_text(encoding='utf-8')

        # Set up loop
        curr_pos = 0
        max_pos = len(src_code)
        curr_types = []

        while curr_pos < max_pos:

            # If the current character is a quote, find the corresponding closing quote
            if self.check_sequence(src_code, curr_pos, '"') or self.check_sequence(src_code, curr_pos, '\''):
                curr_pos = self.parse_string_literal(src_code, src_code[curr_pos], curr_pos)

            # If the current characters start a comment, copy the entire line unchanged
            elif self.check_sequence(src_code, curr_pos, '//'):
                curr_pos = self.parse_comment(src_code, curr_pos)

            # If the current characters start a multi-line comment, copy the entire comment
            elif self.check_sequence(src_code, curr_pos, '/*'):
                curr_pos = self.parse_multiline_comment(src_code, curr_pos)

            # Else test for keywords
            else:
                type, type_end_idx = self.try_parse_keyword(src_code, curr_pos)
                curr_pos = type_end_idx

                # If a keyword was found, add it to the types
                if type:
                    self.detected_types[type] = '::'.join(curr_types)

                # If we have found a brace, add the type to the stack (even if empty)
                if src_code[curr_pos] == '{':
                    curr_types.append(type)

                # If we have found a reverse brace remove the latest type
                elif src_code[curr_pos] == '}':
                    curr_types.pop()

                # Update position
                curr_pos += 1


    def parse_include(self, src_code: str, curr_pos: int) -> int:

        # Isolate the path
        include_start = src_code.index('<', curr_pos) + 1
        include_end = src_code.index('>', include_start)

        # Only process C++ headers
        include_path = Path(self.include_dir, src_code[include_start:include_end])
        if include_path.suffix == '.hpp':
            self.process_header(include_path)

        # Copy the line in any case
        self.buffer.write(src_code[curr_pos:include_end+1])
        return include_end + 1


    def split_function_signature(self, func: str) -> tuple[str, str, str, str]:

        # Divide the function signature into return type, name, arguments and ending
        arg_start, arg_end = mangle.isolate_args(func)
        if arg_start == arg_end == -1:
            raise SystemExit('Missing function arguments!')

        # Split the arguments off
        func_end = func[arg_end+1:].strip()
        func_args = func[arg_start+1:arg_end].strip()

        # Isolate the function name from the return type
        func_ret_plus_name = func[:arg_start].strip()
        name_start = mangle.isolate_func_name(func_ret_plus_name)
        func_name = func_ret_plus_name[name_start:]
        func_ret = func_ret_plus_name[:name_start].strip()

        # Remove all keywords from the return type
        for keyword in mangle.PREPEND_KEYWORDS:
            func_ret = func_ret.replace(f'{keyword} ', '').strip()

        # Return the pieces
        return func_ret, func_name, func_args, func_end


    def fill_type(self, type: str, include_curr_namespaces: bool = False) -> str:

        # Preserve decorators on both sides
        pre_decors = ''
        post_decors = ''

        # Remove post-decorators
        for i, c in mangle.renumerate(type):
            if not (c == '*' or c == '&' or c.isspace()):
                post_decors = type[i+1:]
                type = type[:i+1]
                break

        # Remove pre-decorators if present
        type_split = type.split()
        for i, value in enumerate(type_split):
            if value in mangle.DECORS:
                pre_decors += f'{value} '
            else:
                type_split = ' '.join(type_split[i:])
                break

        # Try to split the type
        type_split = mangle.split_decorated_type(type_split)

        # Scan each piece until a replacement is found
        # TODO support templates and function pointers
        found_filler = False
        for i, type in enumerate(type_split):
            if type in self.detected_types:

                # Remove extra keywords
                type_split = type_split[i:]

                # Insert the replacement if it's not empty
                if self.detected_types[type]:
                    type_split.insert(0, self.detected_types[type])

                # Exit the loop
                found_filler = True
                break

        # If a filler wasn't found and the current namespaces must be included, fill them in!
        if not found_filler and include_curr_namespaces:
            type_split = self.curr_namespaces + [type]

        # Reassemble the type back
        return f'{pre_decors}{"::".join(type_split)}{post_decors}'


    def fill_function(self, func_data: tuple[str, str, str, str]) -> str:

        # Unpack the data
        func_ret, func_name, func_args, func_end = func_data

        # Fill in the function name and the return type if necessary
        func_name = self.fill_type(func_name, True)
        if func_ret:
            func_ret = self.fill_type(func_ret)

        # Fill in each argument as well
        formatted_args = []
        for arg in mangle.split_args(func_args):
            arg = mangle.isolate_arg(arg)
            formatted_args.append(self.fill_type(arg))

        # Reassemble the function
        func = f'{func_ret} {func_name}({", ".join(formatted_args)}) {func_end}'.strip()
        return func


    def read_function_body(self, src_code: str, curr_pos: int) -> int:

        # Set up loop
        curr_nest_level = 0
        max_pos = len(src_code)

        # Check braces until the ending brace is found
        while curr_pos < max_pos:

            # Brace check
            if src_code[curr_pos] == '{':
                curr_nest_level += 1
            elif src_code[curr_pos] == '}':
                curr_nest_level -= 1
                if curr_nest_level == 0:
                    return curr_pos

            # Update position
            curr_pos += 1

        # If we get to this point, the ending brace was not found, so raise an exception
        raise SystemExit('Function body end not found!')


    def create_thunk(self, mangled_name: str, func_data: tuple[str, str, str, str], symbol_addr: str, is_static_func: bool) -> str:

        # Extract the function data and convert the symbol to an integer value
        func_ret, func_name, func_args, func_end = func_data
        symbol_addr = int(symbol_addr, 16)

        # Create the thunk function
        thunk_name = f'{THUNK_FUNCTION_PATTERN}_{mangled_name}'
        self.buffer.write(f'extern "C" asm {func_ret if func_ret else "void"} {thunk_name}(')

        # Try splitting the class name into its decorated components
        func_class = mangle.split_decorated_type(func_name)

        # Add the class if present
        if len(func_class) > 1 and not is_static_func:
            func_class = '::'.join(func_class[:-1])
            self.buffer.write(f'{func_class}* self')

            # Add the comma for the extra args if necessary
            if func_args:
                self.buffer.write(', ')

        # Add the arguments even if empty
        self.buffer.write(func_args)

        # End the signature and add the body
        self.buffer.write(') { nofralloc; opword ')
        self.buffer.write(MAGIC_OPWORD)
        self.buffer.write('; blr; }\n')

        # Add the fake branch and the patch exit hooks
        self.buffer.write(f'kmBranch({hex(symbol_addr+4)}, {thunk_name});\n')
        self.buffer.write(f'kmPatchExitPoint({thunk_name}, {hex(symbol_addr + 4)});\n\n')
        return thunk_name


    def apply_function_replacement(self, src_code: str, curr_pos: int, is_static: bool = False) -> int:

        # Skip the replacement keyword
        if is_static:
            curr_pos += len(REPLACE_STATIC_STRING) + 1
        else:
            curr_pos += len(REPLACE_STRING) + 1

        # Detect the start of the function and cut it off
        func_body_start_idx = src_code.index('{', curr_pos)
        func = src_code[curr_pos:func_body_start_idx]

        # Split off any constructor initializer if present
        filled_func = func.split(': ')[0].strip()

        # Divide the function into pieces and pass them to the fill function
        func_data = self.split_function_signature(filled_func)
        filled_func = self.fill_function(func_data)

        # Prepend extern "C" if there are no classes and no namespaces
        if '::' not in func_data[1] and not self.curr_namespaces:
            filled_func = f'extern "C" {filled_func}'

        # Mangle the filled function
        mangled_func = mangle.mangle_function(filled_func, TYPEDEFS)

        # Get the function body
        func_body_end_idx = self.read_function_body(src_code, func_body_start_idx)
        func_body = src_code[func_body_start_idx:func_body_end_idx+1]

        # If the symbol for the mangled function is missing, bail (it's going to error out later anyway)
        if mangled_func not in self.symbols:
            self.missing_symbols.append(mangled_func)
            return func_body_end_idx + 1

        # Else get it
        symbol_addr = self.symbols[mangled_func]

        # If the original function call is in the body, add the thunk and insert the calls in the body
        if REPLACED_STRING in func_body:

            # Create the thunk
            thunk_name = self.create_thunk(mangled_func, func_data, symbol_addr, is_static)

            # Insert replacement calls
            # Check if the function has a class, and if so prepend the this argument
            if '::' in func_data[1] and not is_static:
                func_body = func_body.replace(f'{REPLACED_STRING}(', f'{thunk_name}(this{", " if func_data[2] else ""}')
            else:
                func_body = func_body.replace(REPLACED_STRING, thunk_name)

        # Write the function and the body
        self.buffer.write(func)
        self.buffer.write(func_body)
        self.buffer.write('\n')

        # If the function contains C++ syntax, add a reference for the branch hook
        if '::' in filled_func:
            self.buffer.write(f'\nextern "C" void {mangled_func}();')

        # Write the branch hook
        self.buffer.write(f'\nkmBranch({symbol_addr}, {mangled_func});')

        # Return updated position
        return func_body_end_idx + 1


    def apply_mangled_call(self, src_code: str, curr_pos: int, is_call: bool) -> int:

        # Skip the keyword
        curr_pos = src_code.index('(', curr_pos)

        # Set up loop
        curr_nest_level = 0
        macro_start_idx = curr_pos + 1
        macro_end_idx = -1
        maxpos = len(src_code)

        # Find the end of the macro
        while curr_pos < maxpos:

            # Parse braces
            if src_code[curr_pos] == '(':
                curr_nest_level += 1
            elif src_code[curr_pos] == ')':
                curr_nest_level -= 1
                if curr_nest_level == 0:
                    macro_end_idx = curr_pos
                    break

            # Update position
            curr_pos += 1

        # If the macro end was not found, bail
        if macro_end_idx == -1:
            raise SystemExit('Mangle call macro end not found!')

        # Get the function within the macro, fill it and mangle it
        func = src_code[macro_start_idx:macro_end_idx]
        func = self.fill_function(self.split_function_signature(func))
        func = mangle.mangle_function(func, TYPEDEFS)

        # Create the extern declaration to be prepended and add it to the list if it doesn't already exist
        decl = f'extern "C" void {func}();\n'
        if decl not in self.externdefs:
            self.externdefs.append(decl)

        # Write the instruction to the buffer instead of the macro
        self.buffer.write(f'{"bl" if is_call else "b"} {func}')

        # Return updated position
        return macro_end_idx + 1


    def process_file(self) -> None:

        # Check if the source and symbol files exist
        if not self.src.is_file():
            raise SystemExit('ERROR: Source file not found!')

        if not self.symbol_file.is_file():
            raise SystemExit('ERROR: Symbol map not found!')

        # Read the source file
        src_code = self.src.read_text(encoding='utf-8')

        # If no keyword replacements were found, write the file directly and bail
        if REPLACE_STRING not in src_code and BRANCH_MANGLE not in src_code and CALL_MANGLE not in src_code:
            self.dest.write_text(src_code, encoding='utf-8')
            return

        # Read all symbols from the map
        self.read_symbol_file()

        # Set up loop
        curr_pos = 0
        max_pos = len(src_code)
        curr_namespace = ''

        # Start reading the file character by character
        while curr_pos < max_pos:

            # If the current character is a quote, find the corresponding closing quote
            if self.check_sequence(src_code, curr_pos, '"') or self.check_sequence(src_code, curr_pos, '\''):
                curr_pos = self.parse_string_literal(src_code, src_code[curr_pos], curr_pos, True)

            # If the current characters start a comment, copy the entire line unchanged
            elif self.check_sequence(src_code, curr_pos, '//'):
                curr_pos = self.parse_comment(src_code, curr_pos, True)

            # If the current characters start a multi-line comment, copy the entire comment
            elif self.check_sequence(src_code, curr_pos, '/*'):
                curr_pos = self.parse_multiline_comment(src_code, curr_pos, True)

            # If the current characters include a header, add it to the list
            elif self.check_sequence(src_code, curr_pos, '#include'):
                curr_pos = self.parse_include(src_code, curr_pos)

            # If the current characters define a static function replacement, apply the necessary changes
            elif self.check_sequence(src_code, curr_pos, REPLACE_STATIC_STRING):
                curr_pos = self.apply_function_replacement(src_code, curr_pos, True)

            # If the current characters define a function replacement, apply the necessary changes
            elif self.check_sequence(src_code, curr_pos, REPLACE_STRING):
                curr_pos = self.apply_function_replacement(src_code, curr_pos)

            # If the current characters define a mangled function branch, apply the necessary changes
            elif self.check_sequence(src_code, curr_pos, BRANCH_MANGLE):
                curr_pos = self.apply_mangled_call(src_code, curr_pos, False)

            # If the current characters define a mangled function call, apply the necessary changes
            elif self.check_sequence(src_code, curr_pos, CALL_MANGLE):
                curr_pos = self.apply_mangled_call(src_code, curr_pos, True)

            # If the current characters define a namespace, set it as current
            elif self.check_sequence(src_code, curr_pos, 'namespace'):

                # Find the next brace
                brace_pos = src_code.index('{', curr_pos)

                # Write everything until the brace to the buffer
                self.buffer.write(src_code[curr_pos:brace_pos])

                # Extract the namespace name and update position
                curr_namespace = src_code[curr_pos+10:brace_pos].strip()
                curr_pos = brace_pos

            # If a brace was opened, add the current namespace to the list
            elif self.check_sequence(src_code, curr_pos, '{'):
                self.curr_namespaces.append(curr_namespace)
                curr_namespace = ''
                self.buffer.write(src_code[curr_pos])
                curr_pos += 1

            # If a brace was closed, remove the last namespace added
            elif self.check_sequence(src_code, curr_pos, '}'):
                self.curr_namespaces.pop()
                self.buffer.write(src_code[curr_pos])
                curr_pos += 1

            # If none of the above was detected, just copy the character and move along
            else:
                self.buffer.write(src_code[curr_pos])
                curr_pos += 1

        # If there were any missing symbols, bail
        if self.missing_symbols:
            err = 'ERROR: The following symbols were not found: ' + ', '.join(self.missing_symbols)
            raise SystemExit(err)

        # Get the buffer contents
        src_code = self.buffer.getvalue()

        # Prepend the extern declarations if necessary
        src_code = ''.join(self.externdefs) + src_code

        # Write the result to the file
        self.dest.write_text(src_code, encoding='utf-8')


if __name__ == '__main__':

    # Get arguments
    parser = ArgumentParser(description='Preprocessor for Kamek code')
    parser.add_argument('source', type=Path, help='The file to be preprocessed')
    parser.add_argument('-m', '--map', type=Path, required=True, help='The path to the symbol map')
    parser.add_argument('-o', '--out', type=Path, required=True, help='The path to the output file')
    parser.add_argument('-i', '--include', type=Path, required=True, help='The folder that contains the includes')
    args = parser.parse_args()

    # Start processing
    _Preprocessor(args.source, args.map, args.include, args.out).process_file()
