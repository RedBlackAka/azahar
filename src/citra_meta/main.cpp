// Copyright 2024-2026 Citra Emulator Project / Azahar Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <iostream>

#include "citra_cli/citra_cli.h"
#include "common/detached_tasks.h"
#include "common/scope_exit.h"

#if !defined(ENABLE_QT)
#error "citra_meta is somehow building with no frontend. This should be impossible!"
#endif

#ifdef ENABLE_QT
#include "citra_qt/citra_qt.h"
#endif
#ifdef ENABLE_ROOM
#include "citra_room/citra_room.h"
#endif

#ifdef _WIN32
extern "C" {
// tells Nvidia and AMD drivers to use the dedicated GPU by default on laptops with switchable
// graphics
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

int main(int argc, char* argv[]) {
    Common::DetachedTasks detached_tasks;
    SCOPE_EXIT({ detached_tasks.WaitForAllTasks(); });

    if (CitraCLI::CheckForOptions(CitraCLI::cli_capture_optstring, argc, argv)) {
        return CitraCLI::ParseCommand(argc, argv);
    }

#if ENABLE_ROOM
    bool launch_room = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--room") == 0) {
            launch_room = true;
        }
    }

    if (launch_room) {
        return LaunchRoom(argc, argv, true);
    }
#endif

#if ENABLE_QT
    return LaunchQtFrontend(argc, argv);
#endif
}
