#!/usr/bin/env python3

# common.py
# Common structures and program logic.

import os.path
from enum import Enum

def savePath(filePath: str, jsonPath: str) -> str:
    if not filePath:
        return filePath
    return os.path.relpath(filePath, jsonPath)

def openPath(filePath: str, jsonPath: str) -> str:
    if not filePath:
        return filePath
    return os.path.normpath(os.path.join(jsonPath, filePath))

class Slot(Enum):
    def __str__(self) -> str:
        splitstr = self.name.split('_')
        for i, j in enumerate(splitstr):
            if j == 'SNES' or j == 'DS' or j == 'GCN' or j == 'N64' or j == 'GBA' or j == 'DK':
                continue
            splitstr[i] = j.title()
        return ' '.join(splitstr)

    def idx(self) -> int:
        return list(Slot).index(self)

    @staticmethod
    def fromIdx(i: int) -> int:
        return list(Slot)[i]

    @staticmethod
    def getAll() -> list[str]:
        return [str(i) for i in Slot]

    LUIGI_CIRCUIT = 8
    MOO_MOO_MEADOWS = 1
    MUSHROOM_GORGE = 2
    TOAD_FACTORY = 4
    MARIO_CIRCUIT = 0
    COCONUT_MALL = 5
    DK_SUMMIT = 6
    WARIO_GOLD_MINE = 7
    DAISY_CIRCUIT = 9
    KOOPA_CAPE = 15
    MAPLE_TREEWAY = 11
    GRUMBLE_VOLCANO = 3
    DRY_DRY_RUINS = 14
    MOONVIEW_HIGHWAY = 10
    BOWSER_CASTLE = 12
    RAINBOW_ROAD = 13
    GCN_PEACH_BEACH = 16
    DS_YOSHI_FALLS = 20
    SNES_GHOST_VALLEY_2 = 25
    N64_MARIO_RACEWAY = 26
    N64_SHERBET_LAND = 27
    GBA_SHY_GUY_BEACH = 31
    DS_DELFINO_SQUARE = 23
    GCN_WALUIGI_STADIUM = 18
    DS_DESERT_HILLS = 21
    GBA_BOWSER_CASTLE_3 = 30
    N64_DK_JUNGLE_PARKWAY = 29
    GCN_MARIO_CIRCUIT = 17
    SNES_MARIO_CIRCUIT_3 = 24
    DS_PEACH_GARDENS = 22
    GCN_DK_MOUNTAIN = 19
    N64_BOWSER_CASTLE = 28

    BLOCK_PLAZA = 33
    DELFINO_PIER = 32
    FUNKY_STADIUM = 35
    CHAIN_CHOMP_WHEEL = 34
    THWOMP_DESERT = 36
    SNES_BATTLE_COURSE_4 = 39
    GBA_BATTLE_COURSE_3 = 40
    N64_SKYSCRAPER = 41
    GCN_COOKIE_LAND = 37
    DS_TWILIGHT_HOUSE = 38

    GALAXY_COLOSSEUM = 54

class Language(Enum):
    ENG_EU = 'English (EU)'
    FRA_EU = 'French (EU)'
    SPA_EU = 'Spanish (EU)'
    GER_EU = 'German (EU)'
    ITA_EU = 'Italian (EU)'
    ENG_US = 'English (US)'
    FRA_US = 'French (US)'
    SPA_US = 'Spanish (US)'
    JAP = 'Japanese'
    KOR = 'Korean'

    def __str__(self) -> str:
        return self.name.lower()

    @staticmethod
    def getAll() -> list[str]:
        return [str(i) for i in Language]

    @staticmethod
    def getAllPretty() -> list[str]:
        return [i.value for i in Language]


class Tracklist(Enum):
    VS_MODERN = 'Modern'
    VS_RETRO = 'Retro'
    VS_VARIETY = 'Variety'
    BT = 'Battle'

    def __str__(self) -> str:
        return self.name.lower()

    @staticmethod
    def getAll() -> list[str]:
        return [str(i) for i in Tracklist]

    @staticmethod
    def getAllPretty() -> list[str]:
        return [i.value for i in Tracklist]

    def getTrackCount(self) -> int:
        return 5 if self == Tracklist.BT else 4


class Track:
    def __init__(self, path: str):
        self.path = path

        self.specialSlot = Slot.LUIGI_CIRCUIT
        self.musicSlot = Slot.LUIGI_CIRCUIT

        self.names = [os.path.basename(os.path.splitext(path)[0]) for _ in Language]
        self.trackAuthor = ''

        self.musicFile = ''
        self.musicName = ''
        self.musicAuthor = ''

        self.musicFileFast = ''
        self.musicNameFast = ''
        self.musicAuthorFast = ''

    def check(self) -> set[str]:
        errors = set()
        langs = Language.getAllPretty()
        trackName = self.names[0]
        for i, name in enumerate(self.names):
            if not name:
                errors.add(f'Invalid {langs[i]} track name for {trackName}!')

        if not self.musicName:
            errors.add(f'Music name not set for {trackName}!')

        if not os.path.isfile(self.path):
            errors.add(f'Track file for {trackName} not found!')

        if not os.path.isfile(self.musicFile):
            errors.add(f'Music file for {trackName} not found!')

        if self.specialSlot not in Slot:
            errors.add(f'Invalid track slot for {trackName}!')

        if self.musicSlot not in Slot:
            errors.add(f'Invalid music slot for {trackName}!')

        if not self.trackAuthor:
            errors.add(f'Track author not set for {trackName}!')

        if not self.musicAuthor:
            errors.add(f'Music author not set for {trackName}!')

        if self.musicFileFast:
            if not os.path.isfile(self.musicFileFast):
                errors.add(f'Fast music file for {trackName} not found!')

            if not self.musicNameFast:
                errors.add(f'Fast music name not set for {trackName}!')

            if not self.musicAuthorFast:
                errors.add(f'Fast music author not set for {trackName}!')

        return errors

    def asDict(self, jsonPath: str) -> dict:
        ret = {}

        # Save the English EU name, skip all the languages with the same text
        langs = Language.getAll()
        defaultKey = f'track_name_{langs[0]}'
        for lang, name in zip(langs, self.names):
            langKey = f'track_name_{lang}'
            if langKey == defaultKey or name != self.names[0]:
                ret[langKey] = name

        # Save all the other parameters
        ret['track_file'] = savePath(self.path, jsonPath)
        ret['track_author'] = self.trackAuthor
        ret['track_slot'] = self.specialSlot.value
        ret['music_slot'] = self.musicSlot.value
        ret['music_file'] = savePath(self.musicFile, jsonPath)
        ret['music_name'] = self.musicName
        ret['music_artist'] = self.musicAuthor

        # Only save the fast music path and author if they're not empty
        if fastPath := savePath(self.musicFileFast, jsonPath):
            ret['music_file_fast'] = fastPath
            ret['music_name_fast'] = self.musicNameFast
            ret['music_author_fast'] = self.musicAuthorFast

        return ret

    @staticmethod
    def fromDict(input: dict, jsonPath: str) -> 'Track':

        # Initialize path with failsafe
        ret = Track(openPath(input.get('track_file', ''), jsonPath))

        # Load all the data with failsaves
        ret.trackAuthor = input.get('track_author', ret.trackAuthor)
        ret.specialSlot = Slot(input.get('track_slot', Slot.LUIGI_CIRCUIT.value))
        ret.musicSlot = Slot(input.get('music_slot', Slot.LUIGI_CIRCUIT.value))
        ret.musicFile = openPath(input.get('music_file', ''), jsonPath)
        ret.musicName = input.get('music_name', '')
        ret.musicAuthor = input.get('music_author', '')
        ret.musicFileFast = openPath(input.get('music_file_fast', ''), jsonPath)
        ret.musicNameFast = input.get('music_name_fast', '')
        ret.musicAuthorFast = input.get('music_author_fast', '')

        # Get all names with failsafe
        langs = Language.getAll()
        defaultInput = input.get(f'track_name_{langs[0]}', 'Unknown Track Name')
        ret.names = [input.get(f'track_name_{lang}', defaultInput) for lang in langs]

        return ret


class RandomTrack:
    def __init__(self):
        self.names = ['New Random Track' for _ in Language]
        self.tracks = []

    def check(self) -> set[str]:
        errors = set()
        langs = Language.getAllPretty()
        trackName = self.names[0]
        for i, name in enumerate(self.names):
            if not name:
                errors.add(f'Invalid {langs[i]} track name for {trackName}!')

        totalChances = 0
        for track, chance in self.tracks:
            errors.update(track.check())
            totalChances += chance

        if totalChances != 256:
            errors.add(f'Invalid variant chances for {trackName}!')

        return errors

    def asDict(self, jsonPath: str) -> dict:
        ret = {}

        # Save the English EU name, skip all the languages with the same text
        langs = Language.getAll()
        defaultKey = f'variant_name_{langs[0]}'
        for lang, name in zip(langs, self.names):
            langKey = f'variant_name_{lang}'
            if langKey == defaultKey or name != self.names[0]:
                ret[langKey] = name

        # Save all variants
        ret['variants'] = [{'chance': chance, 'track': track.asDict(jsonPath)} for track, chance in self.tracks]
        return ret

    @staticmethod
    def fromDict(input: dict, jsonPath: str) -> 'RandomTrack':
        ret = RandomTrack()

        # Get all names with failsafe
        langs = Language.getAll()
        defaultInput = input.get(f'variant_name_{langs[0]}', 'Unknown Variant Name')
        ret.names = [input.get(f'variant_name_{lang}', defaultInput) for lang in langs]

        # Get all contained tracks, with failsafe
        for variant in input['variants']:
            if trackDict := variant.get('track', {}):
                track = Track.fromDict(trackDict, jsonPath)
            else:
                track = Track('')
            chance = variant.get('chance', 1)
            ret.tracks.append((track, chance))

        return ret


class Cup:
    def __init__(self):
        self.names = ['New Cup' for _ in Language]
        self.iconFile = ''
        self.tracks = []

    def check(self, trlist: Tracklist) -> set[str]:
        errors = set()
        langs = Language.getAllPretty()
        cupName = self.names[0]
        for i, name in enumerate(self.names):
            if not name:
                errors.add(f'Invalid {langs[i]} cup name for {cupName}!')

        if not os.path.isfile(self.iconFile):
            errors.add(f'Cup icon file for {cupName} not found!')

        if len(self.tracks) != trlist.getTrackCount():
            errors.add(f'Invalid track count for {cupName}!')

        for track in self.tracks:
            errors.update(track.check())

        return errors

    def asDict(self, jsonPath: str) -> dict:
        ret = {}

        # Save the English EU name, skip all the languages with the same text
        langs = Language.getAll()
        defaultKey = f'cup_name_{langs[0]}'
        for lang, name in zip(langs, self.names):
            langKey = f'cup_name_{lang}'
            if langKey == defaultKey or name != self.names[0]:
                ret[langKey] = name

        # Save the icon file path
        ret['icon_file'] = savePath(self.iconFile, jsonPath)

        # Save each contained track
        ret['tracks'] = [track.asDict(jsonPath) for track in self.tracks]
        return ret

    @staticmethod
    def fromDict(input: dict, jsonPath: str) -> 'Cup':
        ret = Cup()

        # Get all names with failsafe
        langs = Language.getAll()
        defaultInput = input.get(f'cup_name_{langs[0]}', 'Unknown Cup Name')
        ret.names = [input.get(f'cup_name_{lang}', defaultInput) for lang in langs]

        # Get icon file path, with failsafe
        ret.iconFile = openPath(input.get('icon_file', ''), jsonPath)

        # Get all tracks inside the cup, with failsafe
        for track in input.get('tracks', []):
            if 'variants' in track:
                ret.tracks.append(RandomTrack.fromDict(track, jsonPath))
            elif 'track_file' in track:
                ret.tracks.append(Track.fromDict(track, jsonPath))

        return ret
