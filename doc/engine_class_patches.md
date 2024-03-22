## Loads

### Patched
- 0x805780AC - obtains the speed multiplier
- 0x805BC780 - for checking if the credits should be played
- 0x805BCA48 - for printing CC text at the GP award scene
- 0x805BCAB8 - for printing CC text at the VS award scene
- 0x805BD0CC - for saving the GP rank for the respective CC
- 0x807E5934 - for checking cup unlock in GP mode
- 0x807FEDC8 - for reading the KMP setting for dc_pillar
- 0x807FEF30 - for reading the KMP setting for dc_pillar_c (sub-component of dc_pillar)
- 0x8084056C - for printing the bottom text in VS course select
- 0x80841454 - for printing the bottom text in VS cup select
- 0x808416A0 - for printing the bottom text in GP cup select
- 0x80855390 - for printing CC text during GP track intro
- 0x808553E8 - for printing CC text during VS track intro

### No Patch Needed
- 0x80536994 - for obtaining the timer from the KRT (cc is always 150)
- 0x806E3AF8 - for reading the KMP setting for volsmk (else case works)
- 0x807E5E24 - for checking cup unlock in GP mode (function orphaned)
- 0x807FEBC4 - for reading the KMP setting for dc_pillar_c (sub-component of dc_pillar) (inline remnant)
- 0x80817EC8 - for reading the KMP setting for VolcanoPiece/bblock (else case works)

### Unconfirmed
- 0x807A48E4 - for adjusting the Bob-Omb explosion timer (possible inline remnant)

### Unpatched
- 0x806462C4 - for printing CC text at the end of friend rooms
- 0x80654EE4 - for writing the engine class into the RACEHEADER_1 packet
- 0x80655310 - for writing the engine class into the RACEHEADER_1 packet (live view version)
- 0x806FAEBC - something related to KartSound
- 0x807399A4 - for initializing AiHolder (MORE INVESTIGATION REQUIRED)
- 0x8073ADB8 - for obtaining the CPU speed/action data
- 0x807A42AC - for adjusting the Bob-Omb explosion timer
- 0x807ADD00 - something related to Blue Shells
- 0x807AE03C - something related to Blue Shells
- 0x807F8A30 - for printing CC text during Online VS sync

## Stores

### Patched
- 0x805E1D0C - stores 150cc for time trial against ghost
- 0x805E1EB0 - stores 150cc for ghost replay
- 0x80609F10 - stores CC from RH1 packet
- 0x8083FAFC - stores CC from CC select page

### No Patch Needed
- 0x805C7EA4 - stores 150cc for ChannelGhostSelect (channel is not accessible)
- 0x805C8298 - stores CC for some TT thing (inline remnant)
- 0x8060A014 - stores 50cc for Battle Mode (no change needed)
- 0x8060DBD0 - stores 150cc for ghost replay (inline remnant)
- 0x8063B1E4 - stores 150cc for Demo Mode (no change needed)
- 0x8063B2C8 - stores 50cc for Battle Demo Mode (no change needed)
- 0x806452F8 - stores 150cc for ghost replay (inline remnant)
- 0x8083B97C - stores 50cc for Battle Mode (no change needed)
- 0x8083DCE0 - stores 150cc when selecting a character before ghost race (feature disabled)
- 0x8084D5C4 - stores CC for offline multiplayer race (overwritten later)
- 0x8084D654 - stores 50cc for Battle Mode multiplayer (no change needed)
- 0x8084D6AC - stores 150cc for Mission Mode multiplayer (feature disabled)
- 0x8084F50C - stores CC before entering TT/Battle mode (overwritten later)

### Unpatched
- 0x805E03EC - stores 150cc for ghost race
- 0x80650E24 - stores CC from SELECT packet
- 0x80665CA8 - stores CC from competition file
- 0x808432AC - stores CC from mission mode file
- 0x80852550 - stores CC from VS rules
