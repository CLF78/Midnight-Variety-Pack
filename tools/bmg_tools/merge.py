#!/usr/bin/env python3

# merge.py
# Merges multiple decoded BMG files into one

from argparse import ArgumentParser
from pathlib import Path

try:
    import json5
except ImportError:
    raise SystemExit('JSON5 not found! Please install it with `python -m pip install json5`')


def main(inputs: list[Path], output: Path):

    # Parse each file
    messages = {}
    for path in inputs:

        # Failsafe
        if not path.is_file():
            print(f'File {path} does not exist! Skipping...')
            continue

        # Load the data
        with path.open(encoding='utf-8') as input:
            data = json5.load(input)

        # Merge it
        for message_id, message in data.items():
            messages[message_id] = {
                'string': message.get('string', ''),
                'font': message.get('font', 'regular'),
            }

    # Sort the messages by ID
    messages = {key:messages[str(key)] for key in sorted(map(int, messages))}

    # Write the output
    with output.open('w', encoding='utf-8') as out:
        json5.dump(messages, out, ensure_ascii=False, indent=4, quote_keys=True)

if __name__ == '__main__':

    # Get arguments
    parser = ArgumentParser(description='Merges multiple BMG JSON files into one')
    parser.add_argument('inputs', type=Path, nargs='+', help='The files to be merged')
    parser.add_argument('-o', '--output', type=Path, required=True, help='The path to the output file')
    args = parser.parse_args()
    main(args.inputs, args.output)
