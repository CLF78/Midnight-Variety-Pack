#!/usr/bin/env python3

# patch_manager.py
# Handles the Riivolution XML generation command.

from pathlib import Path

from .ninja_syntax_ex import Writer as NinjaWriter

class RiivoPatchManager():
    def __init__(self, writer: NinjaWriter, xmlFile: Path, gameId: str, modName: str, patchId: str):
        self.writer = writer
        self.xmlFile = xmlFile
        self.gameId = gameId
        self.modName = modName
        self.patchId = patchId
        self.patches = []

    def addPatch(self, type: str, **kwargs):
        self.patches.append((type, kwargs))

    def writeCommand(self):

        # Generate the patch arguments string
        patchString = ''
        for patch_name, attribs in self.patches:
            skd = patch_name
            for key, value in attribs.items():

                # Replace slashes in strings to account for Windows stupidity
                if isinstance(value, str):
                    value = value.replace("\\", "/")

                skd += f' {key}={value}'
            patchString += f'--patch {skd} '

        # Write the command
        self.writer.build('xml_tool',
                          self.xmlFile,
                          [],
                          gameid=self.gameId,
                          modname=f'"{self.modName}"',
                          patchid=self.patchId,
                          patches=patchString)
