#!/usr/bin/env python3

# mwcceppc_wine_wrapper.py
# A modified version of RoadrunnerWMC's version with reduced use of winepath to save building time.

import ntpath, posixpath
import sys
import subprocess
from path_utils import PathConverter

def fix_makefile(text: str, codePath: str) -> str:
    """
    Converts all Windows paths to Unix paths within a CodeWarrior-generated Makefile.
    """
    paths = []

    # Find all relevant path lines
    for line in text.splitlines():

        # Header line ("whatever.o: whatever.cpp \")
        if line.count(': ') == 1 and line.endswith(' \\'):
            split_point = line.index(': ')
            paths.append(line[split_point+2:-2])

        # Continuation of a rule ("\tsome_header.h \\")
        elif line.startswith('\t'):
            line = line[1:-2 if line.endswith(' \\') else len(line)]
            paths.append(line)

    # Convert the common path of every collected path
    converter = PathConverter(ntpath.commonpath(paths), True)

    # Assemble the new file and return it
    new_lines = []
    for i, path in enumerate(paths):
        if i == 0:
            continue
        elif i == 1:
            new_lines.append(f'{codePath}: {converter.w2u(path)} \\')
        else:
            new_lines.append('\t%s%s' % (converter.w2u(path), " \\" if i != len(paths) - 1 else ""))

    return '\n'.join(new_lines)

def main():
    argv = sys.argv

    if len(argv) < 2:
        print(f'usage: {argv[0]} /path/to/mwcceppc.exe [arguments to CodeWarrior]...')
        return

    # Ignore this Python script's own filename
    argv.pop(0)

    # First argument is the path to CodeWarrior
    cw_exe = argv.pop(0)

    # Obtain the Windows makefile path from the object code path
    makefile_path_windows = ''
    for i, arg in enumerate(argv):
        prev_arg = argv[i - 1] if i > 0 else None
        if prev_arg == '-o':
            makefile_path_windows = arg + '.d'
            break

    # Save the Linux makefile path and replace it in the command line args
    makefile_path = ''
    for i, arg in enumerate(argv):
        prev_arg = argv[i - 1] if i > 0 else None
        if prev_arg in ['-Mfile', '-MMfile', '-MDfile', '-MMDfile']:
            makefile_path = arg
            argv[i] = makefile_path_windows
            break

    # Invoke CodeWarrior
    proc = subprocess.run(['wine', cw_exe, *argv])
    if proc.returncode != 0:
        exit(proc.returncode)

    # Fix up the generated makefile
    with open(makefile_path, 'r+', encoding='utf-8') as f:
        makefile_txt = f.read()
        data = fix_makefile(makefile_txt, posixpath.splitext(makefile_path)[0])
        f.seek(0)
        f.truncate()
        f.write(data)

if __name__ == '__main__':
    main()
