#!/usr/bin/env python3

# mangle.py
# CodeWarrior mangler

TYPE_ENDINGS = [
    '*',
    '&',
    ')',
    '>',
]

PREPEND_KEYWORDS = [
    'extern "C"',
    'static',
    'virtual',
    'inline',
    'explicit',
    'friend',
]

DECORS = {
    '*': 'P',
    '&': 'R',
    'const': 'C',
    'volatile': 'V',
}

BUILTIN_TYPES = {
    'void': 'v',
    'wchar_t': 'w',
    'bool': 'b',
    'char': 'c',
    'signed char': 'sc',
    'unsigned char': 'Uc',
    'short': 's',
    'signed short': 's',
    'unsigned short': 'Us',
    'int': 'i',
    'signed int': 'i',
    'unsigned int': 'Ui',
    'long': 'l',
    'signed long': 'l',
    'unsigned long': 'Ul',
    'long long': 'x',
    'signed long long': 'x',
    'unsigned long long': 'Ux',
    'float': 'f',
    'double': 'd',
    '...': 'e',
}

OPERATORS = {
    'new': '__nw',
    'delete': '__dl',
    'new[]': '__nwa',
    'delete[]': '__dla',
    '=': '__as',
    '+': '__pl',
    '-': '__mi',
    '*': '__ml',
    '/': '__dv',
    '%': '__md',
    "^": "__er",
    "&": "__ad",
    "|": "__or",
    "~": "__co",
    "!": "__nt",
    "<": "__lt",
    ">": "__gt",
    '+=': '__apl',
    '-=': '__ami',
    "*=": "__amu",
    "/=": "__adv",
    "%=": "__amd",
    "^=": "__aer",
    "&=": "__aad",
    "|=": "__aor",
    "<<": "__ls",
    ">>": "__rs",
    "<<=": "__als",
    ">>=": "__ars",
    '==': '__eq',
    '!=': '__ne',
    "<=": "__le",
    ">=": "__ge",
    "&&": "__aa",
    "||": "__oo",
    "++": "__pp",
    "--": "__mm",
    ",": "__cm",
    "->*": "__rm",
    "->": "__rf",
    '()': '__cl',
    '[]': '__vc',
}

# Status bool, not very elegant but needed
isTemplatedFunction = False


def renumerate(data: list):
    for i in range(len(data)-1, -1, -1):
        yield (i, data[i])


def apply_changes(func: str, typedefs: dict[str, str], substitutions: dict[str, str]) -> str:

    # Apply substitutions first
    if substitutions is not None:
        for key, val in substitutions.items():
            func = func.replace(key, val)

    # Then apply typedefs
    if typedefs is not None:
        for key, val in typedefs.items():
            func = func.replace(key, val)

    # Return modified function
    return func


def isolate_args(func: str) -> tuple[int, int]:

    # Set up loop
    start_brace_idx = -1
    end_brace_idx = -1
    curr_nest_level = 0

    # Iterate through the string in reverse to find the first ending ")" character
    # and the corresponding "(" character
    for i, c in renumerate(func):
        if c == ')':
            if curr_nest_level == 0:
                end_brace_idx = i
            curr_nest_level += 1
        elif c == '(':
            curr_nest_level -= 1
            if curr_nest_level == 0:
                start_brace_idx = i
                break

    # Sanity checks
    if curr_nest_level:
        raise ValueError(f'Mismatched braces!')

    # Return indexes
    return (start_brace_idx, end_brace_idx)


def split_args(args: str) -> list[str]:

    # Account for empty arg list
    if not args:
        return ['void']

    # Set up loop
    pieces = []
    curr_nest_level = 0
    prev_component_idx = 0

    # Iterate through the string and split by non-nested commas
    for i, c in enumerate(args):

        # Detect templates and function pointers
        if c == '<' or c == '(':
            curr_nest_level += 1
        elif c == '>' or c == ')':
            curr_nest_level -= 1

        # Detect arg splits
        elif c == ',' and curr_nest_level == 0:

            # Prevent series of comma characters
            if i == prev_component_idx:
                raise ValueError('Empty argument!')

            # Check passed, move along
            pieces.append(args[prev_component_idx:i].strip())
            prev_component_idx = i + 1

    # Add the final piece
    pieces.append(args[prev_component_idx:].strip())

    # Detect mismatched braces
    if curr_nest_level:
        raise ValueError('Mismatched braces!')

    # Return the pieces
    return pieces


def isolate_template(type: str) -> tuple[int, int]:

    # Set up loop
    start_brace_idx = -1
    end_brace_idx = -1
    curr_nest_level = 0

    # Iterate through the string to find the first non-nested "<" character
    # and the corresponding ">" character
    for i, c in enumerate(type):
        if c == '<' or c == '(':
            if curr_nest_level == 0 and c == '<':
                start_brace_idx = i
            curr_nest_level += 1
        elif c == '>' or c == ')':
            curr_nest_level -= 1
            if curr_nest_level == 0 and c == '>':
                end_brace_idx = i
                break

    # If the template was nested, ignore it
    if start_brace_idx == -1 and end_brace_idx == -1:
        return (start_brace_idx, end_brace_idx)

    # Sanity checks
    if curr_nest_level:
        raise ValueError(f'Mismatched braces!')

    # Return indexes
    return (start_brace_idx, end_brace_idx)


def mangle_func_ptr(type: str) -> tuple[int, int]:

    # Set up loop
    start_func_idx = -1
    end_func_idx = -1
    start_args_idx = -1
    end_args_idx = -1
    curr_nest_level = 0

    # Iterate through the string in reverse to find the first two non-nested ")" characters
    # and the corresponding "(" characters
    for i, c in renumerate(type):
        if c == ')':
            if curr_nest_level == 0:
                if end_args_idx != -1:
                    end_func_idx = i
                else:
                    end_args_idx = i
            curr_nest_level += 1
        elif c == '>':
            curr_nest_level += 1
        elif c == '(':
            curr_nest_level -= 1
            if curr_nest_level == 0:
                if start_args_idx != -1:
                    start_func_idx = i
                    break
                else:
                    start_args_idx = i
        elif c == '<':
            curr_nest_level -= 1

    # If the function was nested, ignore it
    if start_func_idx == start_args_idx == end_func_idx == end_args_idx == -1:
        return ''

    # Sanity checks
    if end_func_idx == -1 or curr_nest_level:
        raise ValueError(f'Mismatched braces!')

    # Set up result
    mangledFunc = ''

    # Isolate the pieces
    funcReturn = type[:start_func_idx].strip()
    funcName = type[start_func_idx+1:end_func_idx].strip()
    funcArgs = type[start_args_idx+1:end_args_idx].strip()

    # Ensure the return type and function name are properly declared
    if not funcReturn or not funcName:
        raise ValueError('Invalid function pointer!')

    # Count the pointers from the function name
    ptrcount = 0
    for i, c in renumerate(funcName):

        # Count the pointers
        if c == '*':
            ptrcount += 1

        # Exit if the first of the two "::" characters are found
        elif c == ':':
            funcName = funcName[:i-1]
            break

    # Remove asterisks
    funcName = funcName.rstrip('*')

    # If it's not a pointer to member function, we can simply add PF
    if not funcName:
        mangledFunc += f'{DECORS["*"] * ptrcount}F'

    # Else mangle the class
    else:
        mangledFunc += f'M{mangle_type(funcName)}F'

    # Split and mangle the args
    funcArgs = split_args(funcArgs)
    for arg in funcArgs:
        mangledFunc += mangle_type(arg)

    # Mangle the return type
    mangledFunc += f'_{mangle_type(funcReturn)}'

    # We got it!
    return mangledFunc


def mangle_type(type: str, noLength: bool = False) -> str:
    global isTemplatedFunction

    # Initialize type
    mangledType = ''

    # Detect pointers and references and remove them from the type
    refcount = 0
    ptrcount = 0
    for i, c in renumerate(type):
        if c == '*':
            ptrcount += 1
        elif c == '&':
            refcount += 1
        elif c.isspace():
            continue
        else:
            type = type[:i+1]
            break

    # Add the mangled decorators (one per count)
    mangledType += DECORS['&'] * refcount
    mangledType += DECORS['*'] * ptrcount

    # Detect initial const and remove it if found
    if type.startswith('const '):
        mangledType += DECORS['const']
        type = type.lstrip('const').lstrip()

    # Do the same for volatile, but ensure it only adds the mangled decorator for pointer/ref types
    if type.startswith('volatile '):
        type = type.lstrip('volatile').lstrip()
        if refcount or ptrcount:
            mangledType += DECORS['volatile']

    # Detect built in types
    if type in BUILTIN_TYPES:
        return mangledType + BUILTIN_TYPES[type]

    # Try to split the type into decorated types and mangle each one of them
    splitTypes = split_decorated_type(type)
    if len(splitTypes) > 1:
        mangledType += mangle_decorated_type(splitTypes)
        return mangledType

    # If the type contains a template, isolate it
    if '<' in type or '>' in type:
        templateStart, templateEnd = isolate_template(type)

        # If the template was not nested, split the contents and mangle each piece
        if not (templateStart == -1 and templateEnd == -1):
            template = type[templateStart+1:templateEnd].strip()
            type = type[:templateStart+1].strip()
            templateArgs = split_args(template)
            type += ','.join(map(mangle_type, templateArgs))
            type += '>'

            # If the length is to be omitted this is the function name, so mark function as templated
            if noLength:
                isTemplatedFunction = True

    # If the type contains a function pointer, isolate it and mangle each component properly
    if '(' in type or ')' in type:
        mangledFunc = mangle_func_ptr(type)
        if mangledFunc:
            type = mangledFunc
            noLength = True

    # Prepend the length if wanted
    if not noLength:
        mangledType += str(len(type))

    # Append the type itself (might be mangled or not mangled)
    return mangledType + type


def mangle_arg(arg: str) -> str:

    # Set up loop
    typeEnd = len(arg)

    # Remove arg names before mangling, if found
    for i, c in renumerate(arg):
        if c in TYPE_ENDINGS or c.isspace():
            typeEnd = i
            break

    # Mangle the type
    argType = arg[:typeEnd+1].strip()
    return mangle_type(argType)


def isolate_func_name(func: str) -> int:

    # Set up loop
    funcStart = -1
    curr_nest_level = 0

    # Iterate through the string in reverse to find the first non-nested split character
    # Only detect templates since function names cannot contain function pointers
    for i, c in renumerate(func):
        if c == '>':
            curr_nest_level += 1
        elif c == '<':
            curr_nest_level -= 1
        elif (c in TYPE_ENDINGS or c.isspace()) and curr_nest_level == 0:
            funcStart = i
            break

    # Detect mismatched braces
    if curr_nest_level:
        raise ValueError('Mismatched braces!')

    # Detect missing return type
    if funcStart == -1:
        raise ValueError('Missing return type!')

    # Return index
    return funcStart + 1


def split_decorated_type(type: str) -> list[str]:

    # Set up loop
    pieces = []
    curr_nest_level = 0
    prev_component_idx = 0

    # Iterate through the string and split by non-nested :: characters
    for i, c in enumerate(type):

        # Detect templates and function pointers
        if c == '<' or c == '(':
            curr_nest_level += 1
        elif c == '>' or c == ')':
            curr_nest_level -= 1

        # Detect name splits
        elif type[i:i+2] == '::' and curr_nest_level == 0:

            # Prevent series of more than two :: characters
            if i <= prev_component_idx:
                raise ValueError('Invalid type provided!')

            # Check passed, move along
            pieces.append(type[prev_component_idx:i])
            prev_component_idx = i + 2

    # Add the final piece
    pieces.append(type[prev_component_idx:])

    # Failsafe
    if not pieces or not pieces[-1]:
        raise ValueError('Invalid type provided!')

    # Detect mismatched braces
    if curr_nest_level:
        raise ValueError('Mismatched braces!')

    # Return the pieces
    return pieces


def mangle_decorated_type(types: list[str]):

    # Set up string
    mangledTypes = ''

    # Add Q if there are multiple pieces
    if len(types) > 1:
        mangledTypes += f'Q{len(types)}'

    # Mangle each piece
    for type in types:
        mangledTypes += mangle_type(type)

    # Return result
    return mangledTypes


def mangle_operator(operator: str) -> str:

    # Check for default operators
    if operator in OPERATORS:
        return OPERATORS[operator]

    # If it's not a cast operator, blame the user
    if not operator.endswith('()'):
        raise ValueError('Invalid operator!')

    # Isolate the type and mangle it
    operatorType = operator[:-2]
    return f'__op{mangle_type(operatorType)}'


def mangle_function_name(pieces: list[str], isConst: bool) -> str:

    # Initialize string
    mangledName = ''

    # Isolate the last piece
    lastPiece = pieces.pop()

    # Check for special names
    if pieces:

        # If the last piece is the same as the previous it's a constructor, use the dedicated keyword
        if lastPiece == pieces[-1]:
            mangledName += '__ct'

        # If the last piece is the same as the previous with a "~" it's a destructor, use the dedicated keyword
        elif lastPiece == f'~{pieces[-1]}':
            mangledName += '__dt'

        # If the piece starts with "operator", get the operator
        elif lastPiece.startswith('operator'):
            lastPiece = lastPiece.split('operator', 1)[-1].strip()
            mangledName += mangle_operator(lastPiece)

        # No special cases, add the name as is
        else:
            mangledName += mangle_type(lastPiece, True)

    # It's a regular function, add the name as is
    else:
        mangledName += mangle_type(lastPiece, True)

    # Add the separator
    mangledName += '__'

    # Mangle the rest of the function
    if pieces:
        mangledName += mangle_decorated_type(pieces)

    # Add the const function identifier
    if isConst:
        mangledName += DECORS['const']

    # Terminate the function name
    mangledName += 'F'
    return mangledName


def mangle_function(func: str, typedefs: dict[str, str] = None, substitutions: dict[str, str] = None) -> str:
    global isTemplatedFunction

    # Reset status bool
    isTemplatedFunction = False

    # Apply typedefs and substitutions and strip excess whitespace
    func = apply_changes(func, typedefs, substitutions)
    func = ' '.join(func.split())

    # Bail on any array argument
    if '[' in func or ']' in func:
        raise NotImplementedError('Array types are not supported!')

    # Ensure the function ends with a valid keyword
    if not (func.endswith(')') or func.endswith('const') or func.endswith('override')):
        raise ValueError('Invalid function ending!')

    # Isolate the arguments from the function
    # Do so by finding the first ending ")" and the corresponding "(" characters
    argStart, argEnd = isolate_args(func)
    funcEnd = func[argEnd+1:].strip()
    funcArgs = func[argStart+1:argEnd].strip()

    # Check for const at the end of the function
    isConstFunc = 'const' in funcEnd

    # Isolate the function name from the return type
    funcRetPlusName = func[:argStart].strip()
    nameStart = isolate_func_name(funcRetPlusName)
    funcName = funcRetPlusName[nameStart:]
    funcRet = funcRetPlusName[:nameStart].strip()

    # Check if the return type contains extern "C"
    # If it's a C function, return the name without mangling it
    if 'extern "C" ' in funcRet and '::' not in funcName:
        return funcName

    # Remove all keywords from the return type
    for keyword in PREPEND_KEYWORDS:
        funcRet = funcRet.replace(f'{keyword} ', '').strip()

    # Prepare the final string
    mangledName = ''

    # Split the function name and mangle it
    funcNameSplit = split_decorated_type(funcName)
    mangledName += mangle_function_name(funcNameSplit, isConstFunc)

    # Split the arguments and mangle them
    splitArgs = split_args(funcArgs)
    for arg in splitArgs:
        mangledName += mangle_arg(arg)

    # If the function is templated, add the mangled return type too
    if isTemplatedFunction:
        mangledName += f'_{mangle_type(funcRet)}'

    # Return result
    return mangledName


def main():
    TESTS = (
        ('void *EGG::TSystem<EGG::Video, EGG::AsyncDisplay, EGG::XfbManager, EGG::SimpleAudioMgr, EGG::SceneManager, EGG::ProcessMeter>::Configuration::getVideo(sStateIf_c*& state, int (fBase_c::*func1)(const void*, void*), int (fBase_c::*func2)(const void*, void*), void (fBase_c::*)(const void*, void*, fBase_c::MAIN_STATE)) const',
         'getVideo__Q33EGG126TSystem<Q23EGG5Video,Q23EGG12AsyncDisplay,Q23EGG10XfbManager,Q23EGG14SimpleAudioMgr,Q23EGG12SceneManager,Q23EGG12ProcessMeter>13ConfigurationCFRP10sStateIf_cM7fBase_cFPCvPv_iM7fBase_cFPCvPv_iM7fBase_cFPCvPvQ27fBase_c10MAIN_STATE_v'),
        ('void std::__sort132<bool (*)( const nw4r::g3d::detail::workmem::MdlZ&, const nw4r::g3d::detail::workmem::MdlZ& )&, nw4r::g3d::detail::workmem::MdlZ*>( nw4r::g3d::detail::workmem::MdlZ*, nw4r::g3d::detail::workmem::MdlZ*, nw4r::g3d::detail::workmem::MdlZ*, bool (*)( const nw4r::g3d::detail::workmem::MdlZ&, const nw4r::g3d::detail::workmem::MdlZ& )& )',
         '__sort132<RPFRCQ54nw4r3g3d6detail7workmem4MdlZRCQ54nw4r3g3d6detail7workmem4MdlZ_b,PQ54nw4r3g3d6detail7workmem4MdlZ>__3stdFPQ54nw4r3g3d6detail7workmem4MdlZPQ54nw4r3g3d6detail7workmem4MdlZPQ54nw4r3g3d6detail7workmem4MdlZRPFRCQ54nw4r3g3d6detail7workmem4MdlZRCQ54nw4r3g3d6detail7workmem4MdlZ_b_v'),
        ('virtual void nw4r::snd::detail::AxfxImpl::HookAlloc( void* (**)( unsigned long ), void (**)( void* ) )',
         'HookAlloc__Q44nw4r3snd6detail8AxfxImplFPPFUl_PvPPFPv_v'),
        ('void nw4r::snd::detail::Test(volatile unsigned int* x) override', 'Test__Q34nw4r3snd6detailFPVUi'),
        ('extern "C" void DWCi_ProcessPacket(int value)', 'DWCi_ProcessPacket')
    )

    print('Running tests...')
    for src, mangled in TESTS:
        print('Mangling', src, end=' -> ')
        result = mangle_function(src)
        print(result)
        if mangled != result:
            raise AssertionError('Test failed!')
    print('All tests passed!')


if __name__ == '__main__':
    main()
