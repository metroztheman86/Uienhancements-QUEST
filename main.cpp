#include "main.hpp"
#include "ModConfig.hpp"

#include "bsml/shared/BSML.hpp"

#include "UnityEngine/Transform.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "GlobalNamespace/LevelCollectionViewController.hpp"
#include "GlobalNamespace/LevelSelectionNavigationController.hpp"

#include <cstdio>
#include <cctype>
#include <string>

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

DEFINE_CONFIG(ModConfig);

enum class Screen { SongList, GameplaySettings, LevelSelection, Leaderboard };

// Positions/sizes copied from the PC mod's BSML files.
struct Layout { float x, y, outerW, outerH, innerW, innerH; };
static Layout GetLayout(Screen s) {
    switch (s) {
        case Screen::SongList:         return {0, -21, 65, 59, 62, 56};
        case Screen::GameplaySettings: return {1, -6, 125, 98, 122, 95};
        case Screen::LevelSelection:   return {2, -9, 160, 88, 157, 85};
        case Screen::Leaderboard:      return {-2, 12, 103, 103, 100, 100};
    }
    return {0, 0, 100, 100, 97, 97};
}

struct Style { bool enabled; float alpha; std::string color; float borderAlpha; std::string borderColor; };

static std::string CleanHex(std::string const& in, char const* fallback) {
    if (in.size() != 7 && in.size() != 9) return fallback;
    if (in[0] != '#') return fallback;
    for (size_t i = 1; i < in.size(); i++) if (!isxdigit(static_cast<unsigned char>(in[i]))) return fallback;
    return in;
}

#define STYLE_FROM(P)                                                                         \
    {                                                                                         \
        auto& c = getModConfig();                                                             \
        bool sel = c.P##Selective.GetValue();                                                 \
        bool border = sel ? c.P##BorderEnabled.GetValue() : c.GlobalBorderEnabled.GetValue(); \
        return Style{                                                                         \
            c.P##Enabled.GetValue(),                                                          \
            sel ? c.P##Alpha.GetValue() : c.GlobalAlpha.GetValue(),                           \
            CleanHex(sel ? c.P##Color.GetValue() : c.GlobalColor.GetValue(), "#000000"),      \
            border ? (sel ? c.P##BorderAlpha.GetValue() : c.GlobalBorderAlpha.GetValue()) : 0.0f, \
            CleanHex(sel ? c.P##BorderColor.GetValue() : c.GlobalBorderColor.GetValue(), "#FFFFFF")}; \
    }

static Style GetStyle(Screen s) {
    switch (s) {
        case Screen::SongList:         STYLE_FROM(SongList)
        case Screen::GameplaySettings: STYLE_FROM(GameplaySettings)
        case Screen::LevelSelection:   STYLE_FROM(LevelSelection)
        case Screen::Leaderboard:      STYLE_FROM(Leaderboard)
    }
    return Style{false, 0, "#000000", 0, "#FFFFFF"};
}

// Same structure as the PC mod: an outer "border" panel with an inner panel on top.
static std::string BuildXml(Layout const& l, Style const& st) {
    char buf[2048];
    snprintf(buf, sizeof buf,
        "<vertical anchor-pos-x=\"%g\" anchor-pos-y=\"%g\" ignore-layout=\"true\" "
        "child-control-height=\"false\" child-control-width=\"false\" horizontal-fit=\"PreferredSize\">"
        "<horizontal horizontal-fit=\"PreferredSize\" pref-width=\"%g\" pref-height=\"%g\" "
        "bg=\"round-rect-panel\" bg-alpha=\"%g\" bg-color=\"%s\" child-align=\"MiddleCenter\">"
        "<horizontal horizontal-fit=\"PreferredSize\" pref-width=\"%g\" pref-height=\"%g\" "
        "bg=\"round-rect-panel\" bg-alpha=\"%g\" bg-color=\"%s\"/>"
        "</horizontal></vertical>",
        l.x, l.y, l.outerW, l.outerH, st.borderAlpha, st.borderColor.c_str(),
        l.innerW, l.innerH, st.alpha, st.color.c_str());
    return buf;
}

static void Construct(std::string const& xml, UnityEngine::Transform* parent) {
    BSML::parse_and_construct(xml, parent);
}

static void Attach(UnityEngine::Transform* parent, Screen screen) {
    if (!parent) return;
    Style st = GetStyle(screen);
    if (!st.enabled) return;

    int before = parent->get_childCount();
    Construct(BuildXml(GetLayout(screen), st), parent);
    int after = parent->get_childCount();
    if (after > before) {
        // send the panel behind the screen's real content
        parent->GetChild(after - 1)->SetAsFirstSibling();
    }
}

#define ATTACH_HOOK(HookName, Type, Scr)                                                   \
    MAKE_HOOK_MATCH(HookName, &GlobalNamespace::Type::DidActivate, void,                  \
                    GlobalNamespace::Type* self, bool firstActivation,                     \
                    bool addedToHierarchy, bool screenSystemEnabling) {                    \
        HookName(self, firstActivation, addedToHierarchy, screenSystemEnabling);           \
        if (firstActivation) Attach(self->get_transform(), Scr);                           \
    }

ATTACH_HOOK(SongList_DidActivate, LevelCollectionViewController, Screen::SongList)
ATTACH_HOOK(Gameplay_DidActivate, GameplaySetupViewController, Screen::GameplaySettings)
ATTACH_HOOK(LevelSel_DidActivate, LevelSelectionNavigationController, Screen::LevelSelection)
ATTACH_HOOK(Leaderboard_DidActivate, PlatformLeaderboardViewController, Screen::Leaderboard)

MOD_EXTERN_FUNC void setup(CModInfo* info) noexcept {
    *info = modInfo.to_c();
    getModConfig().Init(modInfo);
    PaperLogger.info("UIEnhancements setup done");
}

MOD_EXTERN_FUNC void late_load() noexcept {
    il2cpp_functions::Init();
    BSML::Init();

    INSTALL_HOOK(PaperLogger, SongList_DidActivate);
    INSTALL_HOOK(PaperLogger, Gameplay_DidActivate);
    INSTALL_HOOK(PaperLogger, LevelSel_DidActivate);
    INSTALL_HOOK(PaperLogger, Leaderboard_DidActivate);
    PaperLogger.info("UIEnhancements hooks installed");
}
