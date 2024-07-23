#!/usr/bin/env python3

# symbol_utility.py
# Given a symbol map, sorts the symbols contained within.
# If also given a symbol list, checks if the symbol is in the map; if not present, asks the user for the address.

from argparse import ArgumentParser
from pathlib import Path

def request_sym_addr(sym: str) -> int:
    while True:
        try:
            new_sym_addr = input(f'Address for \'{sym}\' (do not insert anything to skip): ')
            if not new_sym_addr:
                return 0
            return int(new_sym_addr, 16)
        except ValueError:
            print('Invalid symbol address!')


def get_missing_symbols(sym_map: Path, src: Path) -> None:

    # Read symbol map
    syms = {}
    data = sym_map.read_text(encoding='utf-8').splitlines()
    for line in data:
        if line:
            sym, addr = line.split('=')
            syms[sym] = int(addr, 16)

    # Read symbol file, if it exists
    if src and src.is_file():
        data = src.read_text(encoding='utf-8').replace('"', '').splitlines()

        # Ask the user for each missing symbol
        for line in data:
            if line and not line.startswith('@') and line not in syms:
                new_sym_addr = request_sym_addr(line)
                if new_sym_addr:
                    syms[line] = new_sym_addr

    # Sort the resulting symbol map
    sorted_syms = sorted(syms.items(), key=lambda x: x[1])
    ordered_syms = '\n'.join([f'{sym}=0x{addr:X}' for sym, addr in sorted_syms]) + '\n'

    # Write the updated file back
    sym_map.write_text(ordered_syms, encoding='utf-8')


if __name__ == '__main__':

    # Get arguments
    parser = ArgumentParser(description='Symbol checking and reordering utility')
    parser.add_argument('map', type=Path, help='The path to the symbol map')
    parser.add_argument('--source', type=Path, help='A file containing a list of symbols (optional)')
    args = parser.parse_args()

    # Start processing
    get_missing_symbols(args.map, args.source)
