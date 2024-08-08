#!/usr/bin/env python3

# run_tidy.py
# Runs clang-tidy on each source and header file

from pathlib import Path
import os
import re
import subprocess
import shutil
import sys

# Directories
ROOT_DIR = Path(__file__).parent.parent
SRC_DIR = Path(ROOT_DIR, 'src')
PAYLOAD_CODE_DIR = Path(SRC_DIR, 'payload')
LOADER_CODE_DIR = Path(SRC_DIR, 'loader')

# Files
CLANG_FLAGS_FILE = Path(ROOT_DIR, 'compile_flags.txt')

# Tools
CLANG_TIDY = shutil.which('clang-tidy')

# Output filters
ASM_LABEL_REGEX = re.compile("\\b.*label '[rf]\\d+'.*\\n.*\\n.*\\n")
ASM_TOKEN_REGEX = re.compile("\\b.*unknown token in expression.*\\n.*\\n.*\\n")

# Ensure clang-tidy is available
if not CLANG_TIDY:
    raise SystemExit("clang-tidy not found! Make sure it is installed and present on PATH!")

# Get the compile flags
flags = CLANG_FLAGS_FILE.read_text('utf-8').splitlines()

# Scan for files
files = []
file_patterns = ['*.cpp', '*.c']
directories = [PAYLOAD_CODE_DIR, LOADER_CODE_DIR]
for directory in directories:
    for pattern in file_patterns:
        files += directory.rglob(pattern)

# Run clang-tidy on the detected files
args = [CLANG_TIDY, *files, '-header-filter=.*', '-system-headers', '--', *flags]
result = subprocess.run(args, capture_output=True, text=True, cwd=ROOT_DIR)

# Process the output to remove nonexistant errors caused by inline assembly
report = result.stdout
patterns = [ASM_LABEL_REGEX, ASM_TOKEN_REGEX]
for pattern in patterns:
    report = re.sub(pattern, '', report)

# Remove the root directory of the project from the filenames since clang-tidy isn't consistent
report = report.replace(f'{ROOT_DIR}{os.sep}', '')

# Print the filtered data
print(report)
