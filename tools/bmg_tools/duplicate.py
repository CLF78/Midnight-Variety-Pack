#!/usr/bin/env python3

# duplicate.py
# Copies messages across multiple BMG JSON files

from argparse import ArgumentParser
from pathlib import Path

try:
    import json5
except ImportError:
    raise SystemExit('JSON5 not found! Please install it with `python -m pip install json5`')


def main(input: str, output: str, msg: list[int]):

    # Some constants
    ASSET_DIR = Path(Path(__file__).parent.parent.parent, 'assets', 'menu', 'message')
    LANGS = ['E', 'F', 'G', 'I', 'J', 'K', 'M', 'Q', 'U']

    # Parse file for each language
    for lang in LANGS:

        # Get the source and destination files
        src = Path(ASSET_DIR, f'{input}_{lang}.bmg.json5')
        dest = Path(ASSET_DIR, f'{output}_{lang}.bmg.json5')

        # Check if the files exist
        if src.is_file() and dest.is_file():

            # Open them
            with src.open(encoding='utf-8') as f, dest.open(encoding='utf-8') as g:

                # Load the data from each
                inputData = json5.load(f)
                outputData = json5.load(g)

                # Copy each message to the output files without overwriting them
                for id in msg:
                    if id in inputData and id not in outputData:
                        outputData[id] = inputData[id]

            # Reorder the data and write it to the output file
            outputData = {key:outputData[str(key)] for key in sorted(map(int, outputData))}
            with dest.open('w', encoding='utf-8') as f:
                json5.dump(outputData, f, ensure_ascii=False, indent=4, quote_keys=False)

if __name__ == '__main__':

    # Get arguments
    parser = ArgumentParser(description='Copies messages across multiple BMG JSON files')
    parser.add_argument('input', help='The source file pattern')
    parser.add_argument('output', help='The output file pattern')
    parser.add_argument('messages', nargs='+', help='The messages to copy')
    args = parser.parse_args()
    main(args.input, args.output, args.messages)
