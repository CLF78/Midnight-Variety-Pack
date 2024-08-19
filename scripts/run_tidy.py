#!/usr/bin/env python3

# run_tidy.py
# Runs clang-tidy on a given source and header files

from argparse import ArgumentParser
from pathlib import Path
import os
import re
import subprocess
import shutil

# Directories
ROOT_DIR = Path(__file__).parent.parent
SRC_DIR = Path(ROOT_DIR, 'src')
PAYLOAD_CODE_DIR = Path(SRC_DIR, 'payload')
LOADER_CODE_DIR = Path(SRC_DIR, 'loader')
CODE_DIRS = [PAYLOAD_CODE_DIR, LOADER_CODE_DIR]

# Files
CLANG_FLAGS_FILE = Path(ROOT_DIR, 'compile_flags.txt')

# Tools
CLANG_TIDY = shutil.which('clang-tidy')

# Output filters
ASM_LABEL_REGEX = re.compile("\\b.*label '[rf]\\d+'.*\\n.*\\n.*\\n")
ASM_TOKEN_REGEX = re.compile("\\b.*unknown token in expression.*\\n.*\\n.*\\n")
ASM_REGISTER_REGEX = re.compile("\\b.*register %[rf]\\d+ is only available.*\\n.*\\n.*\\n")
OUTPUT_FILTERS = [ASM_LABEL_REGEX, ASM_TOKEN_REGEX, ASM_REGISTER_REGEX]


def run_tidy(file: Path = None):

    # Ensure clang-tidy is available
    if not CLANG_TIDY:
        raise SystemExit('clang-tidy not found! Make sure it is installed and present on PATH!')

    # Ensure the compile flag file is available
    if not CLANG_FLAGS_FILE.is_file():
        raise SystemExit(f'{CLANG_FLAGS_FILE.name} not found! Make sure it is present at the specified path!')

    # Get the compile flags
    flags = CLANG_FLAGS_FILE.read_text('utf-8').splitlines()

    # If the file is given, scan only that file, unless it does not exist or it isn't a source code file
    files = []
    if file:
        if not file.is_file():
            raise SystemExit(f'The file {file.name} could not be found!')
        if file.suffix not in ['.cpp', '.c', '.hpp', '.h']:
            raise SystemExit(f'The file {file.name} is not a valid source code or header file!')
        files.append(file)

    # Scan for files if none were provided
    if not files:
        file_patterns = ['*.cpp', '*.c']
        for directory in CODE_DIRS:
            for pattern in file_patterns:
                files += directory.rglob(pattern)

    # Run clang-tidy on the detected files
    args = [CLANG_TIDY, *files, '-header-filter=.*', '-system-headers', '--', *flags]
    result = subprocess.run(args, capture_output=True, text=True, cwd=ROOT_DIR)

    # Process the output to remove nonexistant errors caused by inline assembly
    report = result.stdout
    for pattern in OUTPUT_FILTERS:
        report = re.sub(pattern, '', report)

    # Remove the root directory of the project from the filenames since clang-tidy isn't consistent
    report = report.replace(f'{ROOT_DIR}{os.sep}', '')

    # Print the filtered data
    print(report)


if __name__ == '__main__':

    # Get arguments
    parser = ArgumentParser(description='Runs clang-tidy on a given file and associated header files')
    parser.add_argument('-file', type=Path, help='The file to be analyzed (don\'t pass to scan the entire project)')
    args = parser.parse_args()

    # Start processing
    run_tidy(args.file)
