#!/usr/bin/env python3

# run_tidy.py
# Runs clang-tidy on each source and header file

from pathlib import Path
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
CLANG_TIDY = Path(shutil.which('clang-tidy'))

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
result = subprocess.run(args, stdout=sys.stdout, stderr=sys.stderr, cwd=ROOT_DIR)
exit(result.returncode)
