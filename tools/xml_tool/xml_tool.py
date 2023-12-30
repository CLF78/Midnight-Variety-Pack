#!/usr/bin/env python3

# xml_tool.py
# Generates a Riivolution XML from the given directives.

import argparse
import xml.etree.ElementTree as ET
import xml.dom.minidom

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
    # TODO change this to use etree.indent and remove the extra dependency (Python 3.9+)
    xml_string = ET.tostring(root, encoding='utf-8')
    pretty_xml = xml.dom.minidom.parseString(xml_string).toprettyxml(indent='	')

    # Flush it to file
    with open(file_name, 'w', encoding='utf-8') as f:
        f.write(pretty_xml)


def parse_external(file: str, patches: list):
    tree = None

    # Try parsing the XML for the first time
    try:
        tree = ET.parse(file)
    except ET.ParseError:
        pass

    # If this did not work, add a root tag around the file
    # This is required for Kamek-generated XMLs
    if not tree:
        with open(file, 'r+', encoding='utf-8') as f:
            data = f.read()
            f.seek(0)
            f.truncate()
            f.write('<root>')
            f.write(data)
            f.write('</root>')

        # Try parsing it again, bail on failure
        try:
            tree = ET.parse(file)
        except ET.ParseError:
            print(f'Failed to parse {file}!')
            return

    # Parse the XML
    root = tree.getroot()
    for element in root:

        # Generate data
        tag = element.tag
        attributes = {attr: value for attr, value in element.attrib.items()}

        # Deduplicate identical patches
        deduplication_attribs = {
            'file': 'disc',
            'memory': 'offset',
        }

        # Find elements with the same tag with an equal attribute
        duplicated = False
        if tag in deduplication_attribs:
            for patch, attribs in patches:
                attr_to_check = deduplication_attribs[tag]
                if patch == tag and attributes.get(attr_to_check, '') == attribs.get(attr_to_check, ''):
                    duplicated = True
                    break

        # Add the patch if no equivalent was found
        if not duplicated:
            patches.append((tag, attributes))


if __name__ == '__main__':

    # Parse command line args
    parser = argparse.ArgumentParser(description='Generates a Riivolution XML from the given directives')
    parser.add_argument('file_name', help='Name of the XML file to generate')
    parser.add_argument('game_id', help='The game ID for which this XML will apply')
    parser.add_argument('patch_name', help='The name of the patch')
    parser.add_argument('patch_id', help='The ID of the patch')
    parser.add_argument('--patch', nargs='+', help='Define a patch, in the format patch_type attribute1=value1 attribute2=value2', action='append')
    parser.add_argument("--external", nargs="+", help="External file(s) to import patches from")
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

    # Parse external patches
    if args.external:
        for file in args.external:
            parse_external(file, patches)

    # Do not proceed if no patches have been found
    if not patches:
        raise SystemExit('No patches provided!')

    # Create the XML!
    create_xml(args.file_name, args.game_id, args.patch_name, args.patch_id, patches)
