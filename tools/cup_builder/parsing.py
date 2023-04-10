#!/usr/bin/env python3

# parsing.py
# Parses the JSON data and checks it

import json

from common import Tracklist, Cup, RandomTrack, Track

def importData(file: str):

    # Open the file
    with open(file, encoding='utf-8') as f:
        data = json.load(f)

    # Keep track of loaded tracks to make sure no duplicates occur
    tracks = {}
    randTracks = {}
    cups = []

    # Parse the dict
    for key in Tracklist.getAll():

        # Get cup list with failsafe
        cupList = data.get(key, [])
        parsedCupList = []

        # Initialize cup list
        for cup in cupList:

            # Convert the cup list from dict
            newCup = Cup.fromDict(cup, file)
            parsedCupList.append(newCup)

            # Parse the tracks
            for i, track in enumerate(newCup.tracks):

                # Deduplicate tracks by path
                if isinstance(track, Track):
                    if existingTrack := tracks.get(track.path, ''):
                        newCup.tracks[i] = existingTrack
                    else:
                        tracks[track.path] = track

                # Deduplicate variants by content
                elif isinstance(track, RandomTrack):

                    # Deduplicate contained variants (and add exclusive tracks)
                    for j, variant in enumerate(track.tracks):
                        if existingTrack := tracks.get(variant[0].path, None):
                            track.tracks[j] = (existingTrack, track.tracks[j][1])
                        else:
                            tracks[variant[0].path] = variant[0]

                    # Deduplicate the random track
                    if existingTrack := randTracks.get(tuple(track.tracks), None):
                        newCup.tracks[i] = existingTrack
                    else:
                        randTracks[tuple(track.tracks)] = track

        # Add cup list to all the cups
        cups.append(parsedCupList)

    # Return all the obtained data
    return tracks.values(), randTracks.values(), cups


def checkData(cupLists: list[list[Cup]], randTracks: list[RandomTrack], tracks: list[Track]) -> list[str]:

    # Initialize error list
    errors = set()

    # Check the cup lists
    for cupList, cupListName in zip(cupLists, Tracklist):
        if len(cupList) == 0:
            errors.add(f'The {str(cupListName)} cup list is empty!')

        for cup in cupList:
            errors.update(cup.check(cupListName))

    # Check any other track that was left out
    for randTrack in randTracks:
        errors.update(randTrack.check())

    for track in tracks:
        errors.update(track.check())

    errors = list(errors)
    errors.sort()
    return errors
