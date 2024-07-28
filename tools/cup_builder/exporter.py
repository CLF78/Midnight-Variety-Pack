#!/usr/bin/env python3

# exporter.py
# Transforms the JSON data into a concrete folder structure

import argparse
import json5
import os
import subprocess
from datetime import datetime
from shutil import which

from common import Language, Tracklist, Track, RandomTrack
from parsing import importData, checkData

def enforceAbsPath(path: str):
    if not os.path.isabs(path):
        return os.path.abspath(path)
    return path

def linkPath(src: str, dest: str):
    if os.path.isfile(dest):
        os.remove(dest)

    # This is a fix for Windows' concurrency issues...
    while not os.path.exists(src):
        continue

    os.link(src, dest)


class BMGManager():
    def __init__(self, path: str):
        self.saveDir = path
        self.currentId = 10000
        self.usedIds = {}
        self.textFiles = [{} for _ in Language]

    def update(self, texts: list[str]):

        # Add the text if it's not already in
        if texts[0] not in self.usedIds:
            self.usedIds[texts[0]] = self.currentId
            self.currentId += 1

        # Get the ID to use
        id = self.usedIds[texts[0]]

        # Add the text to the files
        for text, file in zip(texts, self.textFiles):
            file[id] = {'string': text}


    def updateSingle(self, text: str):

        # Add the text if it's not already in
        if text not in self.usedIds:
            self.usedIds[text] = self.currentId
            self.currentId += 1

        # Get the ID to use
        id = self.usedIds[text]

        # Add the text to the files
        for file in self.textFiles:
            file[id] = {'string': text}

    def save(self, fileName: str):
        os.makedirs(self.saveDir, exist_ok=True)
        endings = ['E', 'F', 'S', 'G', 'I', 'U', 'Q', 'M', 'J', 'K']
        for file, ending in zip(self.textFiles, endings):
            path = os.path.join(self.saveDir, f'{fileName}{ending}.bmg.json5')
            with open(path, 'w', encoding='utf-8') as f:
                json5.dump(file, f, ensure_ascii=False, indent=4)


class SZSManager():
    def __init__(self, path: str):
        self.saveDir = path
        self.currentId = 0
        self.savedTracks = {}

    def update(self, track: str):
        if track not in self.savedTracks:
            self.savedTracks[track] = self.currentId
            self.currentId += 1

    def save(self):
        os.makedirs(self.saveDir, exist_ok=True)
        for track, id in self.savedTracks.items():
            linkPath(track, os.path.join(self.saveDir, f'{id}.szs'))


class BRSTMManager():
    def __init__(self, path: str):
        self.saveDir = path
        self.currentId = 0
        self.savedSongs = {}

    def update(self, song: str, fastSong: str):
        if song not in self.savedSongs:
            self.savedSongs[song] = (self.currentId, fastSong)
            self.currentId += 1

    def save(self):
        os.makedirs(self.saveDir, exist_ok=True)
        for track, data in self.savedSongs.items():
            linkPath(track, os.path.join(self.saveDir, f'{data[0]}.brstm'))
            if data[1]:
                linkPath(data[1], os.path.join(self.saveDir, f'{data[0]}_F.brstm'))


class IconManager():
    def __init__(self):
        self.savedIcons = {}

    def addIcon(self, iconPath: str, destPath: str):
        if iconPath not in self.savedIcons:
            subprocess.Popen(['wimgt', 'encode', iconPath, '-D', destPath, '-o', '--n-mm=OFF', '-x', 'TPL.R3'], stdout=subprocess.PIPE)
            self.savedIcons[iconPath] = destPath
        else:
            os.makedirs(os.path.dirname(destPath), exist_ok=True)
            linkPath(self.savedIcons[iconPath], destPath)


def main(jsonFile: str, bmgFolder: str, szsFolder: str, brstmFolder: str, cupFolder: str, codeFolder: str, logFile: str):

    # Call the JSON importer
    tracks, randTracks, cupLists = importData(jsonFile)

    # Check for errors and bail if any is found
    if errors := checkData(cupLists, [], []):
        print('The provided cup data is invalid! The following errors were found:')
        for error in errors:
            print(error)
        raise SystemExit('Exiting...')

    # Summon the managers like the Karens that we are
    bmgMng = BMGManager(bmgFolder)
    szsMng = SZSManager(szsFolder)
    brstmMng = BRSTMManager(brstmFolder)
    cupIconMng = IconManager()

    # Add the track metadata
    for track in tracks:
        szsMng.update(track.path)
        brstmMng.update(track.musicFile, track.musicFileFast)
        bmgMng.update(track.names)
        bmgMng.updateSingle(track.trackAuthor)
        bmgMng.updateSingle(track.musicAuthor)
        bmgMng.updateSingle(track.musicName)
        if track.musicFileFast:
            bmgMng.updateSingle(track.musicAuthorFast)
            bmgMng.updateSingle(track.musicNameFast)

    # Add the random track metadata
    for randTrack in randTracks:
        bmgMng.update(randTrack.names)

    # Add the cup metadata and save the icons
    for cupList, cupDir in zip(cupLists, Tracklist.getAllPretty()):
        bmgMng.updateSingle(cupDir)
        for i, cup in enumerate(cupList):
            bmgMng.update(cup.names)
            cupIconMng.addIcon(cup.iconFile, os.path.join(cupFolder, cupDir.lower(), f'{i}.tpl'))

    # Save the other file data
    bmgMng.save('CupData')
    szsMng.save()
    brstmMng.save()

    # Create the code files
    os.makedirs(codeFolder, exist_ok=True)
    cupDataFile = open(os.path.join(codeFolder, 'CupData.cpp'), 'w', encoding='utf-8')
    cupCountFile = open(os.path.join(codeFolder, 'CupCounts.h'), 'w', encoding='utf-8')

    # Set up the warning string
    warningString = '\n// %s //\n' % 'This file is autogenerated. DO NOT EDIT MANUALLY.'
    warningStringWrap = '/' * (len(warningString) - 2)

    # Write the required include
    cupDataFile.write('#pragma once\n')
    cupDataFile.write('#include <mvp/cup/CupData.hpp>\n\n')

    # Write the warning string
    cupDataFile.write(warningStringWrap)
    cupDataFile.write(warningString)
    cupDataFile.write(warningStringWrap)

    # Write the track data
    cupDataFile.write('\n\nCupData::Track CupData::tracks[] = {\n')
    for track in tracks:
        songData = brstmMng.savedSongs[track.musicFile]
        newline = '    {%d, %d, %d, %d, %d, %d, %d, %d, %d},\n' % (
                track.specialSlot.value,
                track.musicSlot.value,
                bmgMng.usedIds[track.names[0]],
                bmgMng.usedIds[track.trackAuthor],
                songData[0],
                bmgMng.usedIds[track.musicName],
                bmgMng.usedIds[track.musicAuthor],
                bmgMng.usedIds[track.musicNameFast] if songData[1] else 0,
                bmgMng.usedIds[track.musicAuthorFast] if songData[1] else 0)
        cupDataFile.write(newline)
    cupDataFile.write('};\n\n')

    # Write the chances and variants for each random track
    for i, randTrack in enumerate(randTracks, 1):
        newline = 'const u16 rnd_%d_trx[] = {%s};\n' % (
                i, ', '.join([str(tracks.index(track)) for track, _ in randTrack.tracks]))
        newline2 = 'const u8 rnd_%d_chn[] = {%s};\n\n' % (
                i, ', '.join([str(chance) for _, chance in randTrack.tracks]))
        cupDataFile.write(newline)
        cupDataFile.write(newline2)

    # Write the random tracks themselves
    cupDataFile.write('const CupData::RandomTrack CupData::randomTracks[] = {\n')
    for i, randTrack in enumerate(randTracks, 1):
        newline = '    {ARRAY_SIZE(rnd_%d_trx), %d, rnd_%d_trx, rnd_%d_chn},\n' % (
                i, bmgMng.usedIds[randTrack.names[0]], i, i)
        cupDataFile.write(newline)
    cupDataFile.write('};\n\n')

    # Write the cup lists
    for cupList, cupDir in zip(cupLists, Tracklist.getAllPretty()):
        cupDataFile.write('const CupData::Cup cups%s[] = {\n' % cupDir)
        for cup in cupList:
            trackIndexes = []
            for track in cup.tracks:
                if isinstance(track, Track):
                    trackIndexes.append(str(tracks.index(track)))
                elif isinstance(track, RandomTrack):
                    trackIndexes.append(f'CupData::IS_RANDOM | {randTracks.index(track)}')
            cupDataFile.write('    {%d, {%s}},\n' % (
                            bmgMng.usedIds[cup.names[0]],
                            ', '.join(trackIndexes)))
        cupDataFile.write('};\n\n')

    # Write the cup holder
    cupDataFile.write('const CupData::CupList CupData::cupLists[] = {\n')
    for cupDir in Tracklist:
        newline = '    {%d, ARRAY_SIZE(cups%s), cups%s, CUP_ICON_DIR_%s "/%%d.tpl", ONLINE_REGION_%s},\n' % (
                    bmgMng.usedIds[cupDir.value], cupDir.value, cupDir.value, cupDir.name, cupDir.name)
        cupDataFile.write(newline)
    cupDataFile.write('};\n')

    # Write the warning in the cup count file
    cupCountFile.write(warningStringWrap)
    cupCountFile.write(warningString)
    cupCountFile.write(warningStringWrap)

    # Write the counts
    cupCountFile.write('\n\n#define TRACK_COUNT %d\n' % len(tracks))
    cupCountFile.write('#define RANDOM_TRACK_COUNT %d\n' % len(randTracks))
    cupCountFile.write('#define VS_CUP_COUNT %d\n' % max([len(cupList) for cupList in cupLists[:-1]]))
    cupCountFile.write('#define BATTLE_CUP_COUNT %d\n' % len(cupLists[-1]))

    # Close the files
    cupDataFile.close()
    cupCountFile.close()

    # Write the log file
    with open(logFile, 'w', encoding='utf-8') as f:

        # Write the intro line
        f.write(f'{os.path.join(codeFolder, "CupData.cpp")}: ')

        # Get all the paths from each manager and write them down
        lines = [key for d in (szsMng.savedTracks, brstmMng.savedSongs, cupIconMng.savedIcons) for key in d.keys()]
        f.write(' \\\n\t'.join(lines))
        f.write(' ')

    # Write the cup icon timestamp file
    timestampFile = os.path.join(cupFolder, '.extracted')
    with open(timestampFile, 'w', encoding='utf-8') as f:
        f.write(str(datetime.now()))


if __name__ == '__main__':

    # Verify WSZST is installed
    if not which('wimgt'):
        raise SystemExit("Wiimm's SZS Tools are not installed! Make sure they're installed and present on PATH!")

    # Get args
    parser = argparse.ArgumentParser(description='Turns a cup file into a concrete folder structure')
    parser.add_argument('cupFile', help='The cup file')
    parser.add_argument('bmgDir', help='The directory the game text files will be exported to')
    parser.add_argument('szsDir', help='The directory the track files will be exported to')
    parser.add_argument('brstmDir', help='The directory the music files will be exported to')
    parser.add_argument('iconDir', help='The directory the cup icon files will be exported to')
    parser.add_argument('codeDir', help='The directory the generated code will be exported to')
    parser.add_argument('log_file', help='The file to export the log to')
    args = parser.parse_args()

    # Verify the cup file exists
    cupFile = enforceAbsPath(args.cupFile)
    if not os.path.isfile(cupFile):
        raise SystemExit('The cup file does not exist!')

    # Ensure the paths are absolute
    bmgDir = enforceAbsPath(args.bmgDir)
    szsDir = enforceAbsPath(args.szsDir)
    brstmDir = enforceAbsPath(args.brstmDir)
    iconDir = enforceAbsPath(args.iconDir)
    codeDir = enforceAbsPath(args.codeDir)
    logFile = enforceAbsPath(args.log_file)

    # Run main
    main(cupFile, bmgDir, szsDir, brstmDir, iconDir, codeDir, logFile)
