#!/usr/bin/env python3

# mwcceppc_windows_wrapper.py
# A wrapper that accounts for CW's stupidity when writing makefiles

import os.path
import sys
import subprocess
from pathlib import Path

def fix_makefile(text: str) -> str:
    """
    Converts a relative path in a CW makefile so that it is usable by Ninja.
    """

    # Split the first line
    lines = text.split('\n', 1)
    line = lines[0]

    # Fix up the portion we need
    split_point = line.index(': ')
    lines[0] = f'{os.path.abspath(line[:split_point])}{line[split_point:]}'

    # Merge the lines back
    return '\n'.join(lines)

def main():
    argv = sys.argv

    if len(argv) < 2:
        print(f'usage: {argv[0]} /path/to/mwcceppc.exe [arguments to CodeWarrior]...')
        return

    # Ignore this Python script's own filename
    argv.pop(0)

    # First argument is the path to CodeWarrior
    cw_exe = Path(argv.pop(0))

    # Get the makefile path
    makefile_path = Path()
    for i, arg in enumerate(argv):
        prev_arg = argv[i - 1] if i > 0 else None
        if prev_arg in ['-Mfile', '-MMfile', '-MDfile', '-MMDfile']:
            makefile_path = Path(arg)
            break

    # Invoke CodeWarrior
    proc = subprocess.run([cw_exe, *argv])
    if proc.returncode != 0:
        exit(proc.returncode)

    # Fix up the generated makefile
    with makefile_path.open('r+', encoding='utf-8') as f:
        data = fix_makefile(f.read())
        f.seek(0)
        f.truncate()
        f.write(data)

if __name__ == '__main__':
    main()
