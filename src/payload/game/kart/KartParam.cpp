#include <common/Common.hpp>
#include <game/kart/KartParam.hpp>
#include <game/race/RaceGlobals.hpp>
#include <wiimmfi/Reporting.hpp>

///////////////////////
// Wiimmfi Telemetry //
///////////////////////

// Report Common.szs subfile hashes for potential cheating
// Credits: Wiimmfi
REPLACE_STATIC void KartParam::init() {
    REPLACED();
    if (RaceGlobals::isOnlineRace)
        Wiimmfi::Reporting::ReportCommonSubfiles();
}
