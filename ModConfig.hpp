#pragma once
#include "config-utils/shared/config-utils.hpp"

// One block of settings per screen. "Selective" = use this screen's own
// look instead of the global one (same idea as the PC mod).
#define SCREEN_CONFIG(P, ENABLED)                                  \
    CONFIG_VALUE(P##Enabled, bool, #P "Enabled", ENABLED);         \
    CONFIG_VALUE(P##Selective, bool, #P "Selective", false);       \
    CONFIG_VALUE(P##Alpha, float, #P "Alpha", 0.5f);               \
    CONFIG_VALUE(P##Color, std::string, #P "Color", "#000000");    \
    CONFIG_VALUE(P##BorderEnabled, bool, #P "BorderEnabled", false);\
    CONFIG_VALUE(P##BorderColor, std::string, #P "BorderColor", "#FFFFFF"); \
    CONFIG_VALUE(P##BorderAlpha, float, #P "BorderAlpha", 0.5f);

DECLARE_CONFIG(ModConfig) {
    // Global look
    CONFIG_VALUE(GlobalAlpha, float, "GlobalAlpha", 0.5f);
    CONFIG_VALUE(GlobalColor, std::string, "GlobalColor", "#000000");
    CONFIG_VALUE(GlobalBorderEnabled, bool, "GlobalBorderEnabled", false);
    CONFIG_VALUE(GlobalBorderColor, std::string, "GlobalBorderColor", "#FFFFFF");
    CONFIG_VALUE(GlobalBorderAlpha, float, "GlobalBorderAlpha", 0.5f);

    // Per screen
    SCREEN_CONFIG(SongList, true)
    SCREEN_CONFIG(GameplaySettings, true)
    SCREEN_CONFIG(LevelSelection, true)
    SCREEN_CONFIG(Leaderboard, true)
};
