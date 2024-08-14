#!/bin/bash
# Original script: https://github.com/stblr/ddd/blob/main/tools/prepare.sh

7z x -o./ CW55xx_v2_10_SE.exe /Data1.cab
7z x -o../tools/cw Data1.cab _44CD8EA541F44E56801091477F3DC9AA
7z x -o../tools/cw Data1.cab _274B47372A7D4391B92F01E51B171A58
7z x -o../tools/cw Data1.cab _4C8ADA37887647F5955B4FB0F716277F

mv ../tools/cw/_44CD8EA541F44E56801091477F3DC9AA ../tools/cw/license.dat
mv ../tools/cw/_274B47372A7D4391B92F01E51B171A58 ../tools/cw/lmgr11.dll
mv ../tools/cw/_4C8ADA37887647F5955B4FB0F716277F ../tools/cw/mwcceppc.exe

rm Data1.cab
