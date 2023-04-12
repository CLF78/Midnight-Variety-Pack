#!/usr/bin/env python3

# merge.py
# Merges multiple BMG JSON files into one

from argparse import ArgumentParser
import os.path

import json5

# Get arguments
parser = ArgumentParser(description='Merges multiple BMG JSON files into one')
parser.add_argument('inputs', nargs='+', help='The files to be merged')
parser.add_argument('-o', '--output', required=True, help='The path to the output file')
args = parser.parse_args()
messages = {}

# Parse each file
for path in args.inputs:

    # Failsafe
    if not os.path.isfile(path):
        print(f'File {path} does not exist! Skipping...')
        continue

    # Load the data
    with open(path, encoding='utf-8') as input:
        data = json5.load(input)

    # Merge it
    for message_id, message in data.items():
        messages[message_id] = {
            'string': message.get('string', ''),
            'font': message.get('font', 'regular'),
        }

# Sort the messages by ID
messages = dict(sorted(messages.items(), key=lambda item: int(item[0])))

# Write the output
with open(args.output, 'w', encoding='utf-8') as out:
    json5.dump(messages, out, ensure_ascii=False, indent=4, quote_keys=True)
