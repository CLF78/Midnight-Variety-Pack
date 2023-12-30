#!/usr/bin/env python3

# patch_manager.py
# Handles the Riivolution XML generation command.

from pathlib import Path

from .ninja_syntax_ex import Writer as NinjaWriter

class RiivoPatchManager():
    def __init__(self, writer: NinjaWriter, xmlFile: Path, gameId: str, modName: str, patchId: str, external_files: list[Path] = []):
        self.writer = writer
        self.xmlFile = xmlFile
        self.gameId = gameId
        self.modName = modName
        self.patchId = patchId
        self.external_files = [str(x) for x in external_files]
        self.patches = []

    def addPatch(self, type: str, **kwargs):
        self.patches.append((type, kwargs))

    def writeCommand(self):

        # Generate the patch arguments string
        patchString = ''
        for patch_name, attribs in self.patches:
            skd = patch_name
            for key, value in attribs.items():
                skd += f' {key}={value}'
            patchString += f'--patch {skd} '

        # Generate the external arguments string
        externalString = '--external ' + ' --external '.join(self.external_files)

        # Write the command
        self.writer.build('xml_tool',
                          self.xmlFile,
                          self.external_files,
                          gameid=self.gameId,
                          modname=f'"{self.modName}"',
                          patchid=self.patchId,
                          patches=patchString,
                          externals=externalString)
