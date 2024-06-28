#!/usr/bin/env python3

# check_file_size.py
# Ensures the given file fits the provided size constraint.

from argparse import ArgumentParser
from pathlib import Path


def main(file_path: Path, max_size: int):
    if not file_path.is_file():
        raise SystemExit(f"File '{file_path}' not found.")

    file_size = file_path.stat().st_size
    if file_size > max_size:
        raise SystemExit(f"File '{file_path}' exceeds the size limit of {max_size} bytes (actual size: {file_size} bytes).")


if __name__ == "__main__":
    parser = ArgumentParser(description='Ensures the given file fits the provided size constraint')
    parser.add_argument("file_path", type=Path, help="Path to the file to check.")
    parser.add_argument("max_size", type=int, help="Maximum allowed file size in bytes.")
    args = parser.parse_args()
    main(args.file_path, args.max_size)
