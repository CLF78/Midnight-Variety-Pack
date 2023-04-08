#!/usr/bin/env python3

# parser.py
# Parses the JSON data and applies the exported data (and viceversa)

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

"""
TODO:

deduplicate:
- bmgs (use dict[str: int], with text as key and id as value)
- szs files (use dict[str: int], with original path as key and id as value)
- brstm files (use dict[str: int], with original path as key and id as value)
- icon files (use dict[str: str], with original path as key and destination path as value)

operations:
- write code
- copy and rename track files (using hard link)
- copy and rename BRSTM files (using hard link)
- copy and rename icon files (using hard link)
"""
