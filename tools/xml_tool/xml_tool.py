#!/usr/bin/env python3

# xml_tool.py
# Generates a Riivolution XML from the given directives.

import argparse
import re
import xml.etree.ElementTree as ET

PATCH_END_PATTERN = r'^(\s*)<\/patch>'

def create_xml(file_name: str, gameId: str, patchName: str, patchId: str, patches: list):

    # Create root element
    root = ET.Element('wiidisc', {'version': '1'})

    # Create game id tag
    ET.SubElement(root, 'id', {'game': gameId})

    # Create the option tree
    options = ET.SubElement(root, 'options')
    section = ET.SubElement(options, 'section', {'name': patchName})
    option = ET.SubElement(section, 'option', {'name': patchName})
    choice = ET.SubElement(option, 'choice', {'name': 'Enabled'})
    ET.SubElement(choice, 'patch', {'id': patchId})

    # Create the patch tree and fill it
    patchRoot = ET.SubElement(root, 'patch', {'id': patchId})
    for patchName, attributes in patches:
        patch = ET.SubElement(patchRoot, patchName)
        for attr, value in attributes.items():
            patch.set(attr, value)

    # Prettify the XML
    ET.indent(root, '	')
    pretty_xml = ET.tostring(root, encoding='unicode')

    # Insert the Kamek placeholder at the end of the patch list
    pretty_xml = re.sub(PATCH_END_PATTERN, '\g<1>\g<1>$KF$\n\g<0>', pretty_xml, flags=re.MULTILINE)

    # Flush it to file
    with open(file_name, 'w', encoding='utf-8') as f:
        f.write(pretty_xml)


if __name__ == '__main__':

    # Parse command line args
    parser = argparse.ArgumentParser(description='Generates a Riivolution XML from the given directives')
    parser.add_argument('file_name', help='Name of the XML file to generate')
    parser.add_argument('game_id', help='The game ID for which this XML will apply')
    parser.add_argument('patch_name', help='The name of the patch')
    parser.add_argument('patch_id', help='The ID of the patch')
    parser.add_argument('--patch', nargs='+', help='Define a patch, in the format patch_type attribute1=value1 attribute2=value2', action='append')
    args = parser.parse_args()

    patches = []

    # Parse command-line patches
    if args.patch:
        for patch in args.patch:

            # Remove the name from the list
            patch_name = patch.pop(0)

            # Fill the attribute list
            attributes = {}
            for attr in patch:
                attr_name, attr_value = attr.split('=')
                attributes[attr_name] = attr_value

            # Add the patch
            patches.append((patch_name, attributes))

    # Do not proceed if no patches have been found
    if not patches:
        raise SystemExit('No patches provided!')

    # Create the XML!
    create_xml(args.file_name, args.game_id, args.patch_name, args.patch_id, patches)
