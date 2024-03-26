## RKNetSELECTHandler

### Patched
- calcPhase
- GetPlayerVote
- getStaticInstance
- GetWinningTrack
- ImportNewPackets
- PrepareUSERAndSELECTSendPackets

### To Be Patched
- decideEngineClass
- DecideTrack
- InitializePackets
- prepareSendPacket
- SetSendPacketPlayerDetails

### VotingBackPage
- GetInstructionText (0x80650BE0, update instruction text with custom info)
- SetupRace (0x80650E24, store course and engine class)

### VotingPage
- activate (0x806433CC, store course vote correctly)
- SetupRoulette (0x806441E8, store correct track name)
- StorePlayerVote (0x80643F8C, invalid vote check needs to be updated)
