## RKNetSELECTHandler
### Patched
- calcPhase
- GetPlayerVote
- getStaticInstance
- GetWinningTrack
- ImportNewPackets
- PrepareUSERAndSELECTSendPackets
- SetPlayerData

### To Be Patched
- decideEngineClass
- DecideTrack
- prepareSendPacket

## VotingBackPage
### To Be Patched
- GetInstructionText (0x80650BE0, update instruction text with custom info)
- SetupRace (0x80650E24, store course and engine class)

## VotingPage
### Patched
- StorePlayerVote

### To Be Patched
- SetupRoulette (0x806441E8, store correct track name)
