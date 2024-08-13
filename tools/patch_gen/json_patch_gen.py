#!/usr/bin/env python3

# json_patch_gen.py
# Generates a Dolphin game mod preset from the given directives.

import argparse
import json5
import sys
from pathlib import Path

def convert_path(path: Path) -> str:

    # Split the path into parts (with Linux-specific hack)
    parts = list(path.parts)
    if sys.platform != 'win32':
        parts[0] = ''

    # Merge them into the escaped string
    return '/'.join(parts)


def create_json(json_file: Path, xml_file: Path, game_dump: Path, patch_name: str):

    json_data = {
        "base-file": convert_path(game_dump),
        "riivolution": {
            "patches": [
                {
                    "options": [
                    {
                        "choice": 1,
                        "option-name": patch_name,
                        "section-name": patch_name
                    }
                    ],
                    "root": convert_path(xml_file.parent.parent),
                    "xml": convert_path(xml_file)
                }
            ]
        },
        "type": "dolphin-game-mod-descriptor",
        "version": 1
    }

    # Dump data to string first because we need to replace an escape sequence
    data = json5.dumps(json_data, ensure_ascii=False, indent=2, trailing_commas=False, quote_keys=True).replace('/', '\\/')
    json_file.write_text(data, 'utf-8')


if __name__ == '__main__':

    # Parse command line args
    parser = argparse.ArgumentParser(description='Generates a Dolphin game mod preset from the given directives')
    parser.add_argument('json_file', type=Path, help='the JSON file to generate')
    parser.add_argument('xml_file', type=Path, help='the XML file to be referenced')
    parser.add_argument('game_file', type=Path, help='the game to be booted')
    parser.add_argument('patch_name', help='the patch name')
    args = parser.parse_args()

    # Create the JSON!
    create_json(args.json_file, args.xml_file, args.game_file, args.patch_name)
