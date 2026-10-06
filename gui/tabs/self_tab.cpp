#include "pch-il2cpp.h"
#include "self_tab.h"
#include "game.h"
#include "gui-helpers.hpp"
#include "utility.h"
#include "state.hpp"
#include "toasts.hpp"
#include "logger.h"
#include "_hooks.h"

extern void RevealAnonymousVotes(); // in MeetingHud.cpp

namespace SelfTab {
    enum Groups {
        Visuals,
        Utils,
        Roles,
        Randomizers,
        AntiExploit,
        TextEditor
    };

    static bool openVisuals = true; //default to visual tab group
    static bool openUtils = false;
    static bool openRoles = false;
    static bool openRandomizers = false;
    static bool openAntiExploit = false;
    static bool openTextEditor = false;

    static std::string originalText = "";
    static std::string editedText = "";

    static bool italicName = false;
    static bool underlineName = false;
    static bool strikethroughName = false;
    static bool boldName = false;
    static bool nobrName = false;
    static ImVec4 nameColor1 = ImVec4(1.f, 1.f, 1.f, 1.f);
    static ImVec4 nameColor2 = ImVec4(1.f, 1.f, 1.f, 1.f);
    static bool coloredName = false;
    static bool font = false;
    static int fontType = 0;
    static bool resizeName = false;
    static float nameSize = 0.f;
    static bool indentName = false;
    static float indentLevel = 0.f;
    static bool cspaceName = false;
    static float cspaceLevel = 0.f;
    static bool mspaceName = false;
    static float mspaceLevel = 0.f;
    static bool voffsetName = false;
    static float voffsetLevel = 0.f;
    static bool rotateName = false;
    static float rotateAngle = 0.f;

    void CloseOtherGroups(Groups group) {
        openVisuals = group == Groups::Visuals;
        openUtils = group == Groups::Utils;
        openRoles = group == Groups::Roles;
        openRandomizers = group == Groups::Randomizers;
        openAntiExploit = group == Groups::AntiExploit;
        openTextEditor = group == Groups::TextEditor;
    }

    void OpenSubGroup(const std::string& name) {
        if (name == "Visuals") CloseOtherGroups(Groups::Visuals);
        else if (name == "Utils") CloseOtherGroups(Groups::Utils);
        else if (name == "Roles") CloseOtherGroups(Groups::Roles);
        else if (name == "Randomizers") CloseOtherGroups(Groups::Randomizers);
        else if (name == "Anti-Exploit") CloseOtherGroups(Groups::AntiExploit);
        else if (name == "Text Editor") CloseOtherGroups(Groups::TextEditor);
    }

    std::string GetTextEditorName(std::string str) {
        str = RemoveHtmlTags(str);

        std::string opener = "", closer = "";

        if (coloredName) {
            str = GetGradientUsername(str, nameColor1, nameColor2);
        }

        if (italicName) {
            opener += "<i>";
            closer += "</i>";
        }

        if (underlineName && !coloredName) {
            opener += "<u>";
            closer += "</u>";
        }

        if (strikethroughName && !coloredName) {
            opener += "<s>";
            closer += "</s>";
        }

        if (boldName) {
            opener += "<b>";
            closer += "</b>";
        }

        if (nobrName) {
            opener += "<nobr>";
            closer += "</nobr>";
        }

        if (font) {
            switch (fontType) {
            case 0: {
                opener += "<font=\"Barlow-Italic SDF\">";
                break;
            }
            case 1: {
                opener += "<font=\"Barlow-Medium SDF\">";
                break;
            }
            case 2: {
                opener += "<font=\"Barlow-Bold SDF\">";
                break;
            }
            case 3: {
                opener += "<font=\"Barlow-SemiBold SDF\">";
                break;
            }
            case 4: {
                opener += "<font=\"Barlow-SemiBold Masked\">";
                break;
            }
            case 5: {
                opener += "<font=\"Barlow-ExtraBold SDF\">";
                break;
            }
            case 6: {
                opener += "<font=\"Barlow-BoldItalic SDF\">";
                break;
            }
            case 7: {
                opener += "<font=\"Barlow-BoldItalic Masked\">";
                break;
            }
            case 8: {
                opener += "<font=\"Barlow-Black SDF\">";
                break;
            }
            case 9: {
                opener += "<font=\"Barlow-Light SDF\">";
                break;
            }
            case 10: {
                opener += "<font=\"Barlow-Regular SDF\">";
                break;
            }
            case 11: {
                opener += "<font=\"Barlow-Regular Masked\">";
                break;
            }
            case 12: {
                opener += "<font=\"Barlow-Regular Outline\">";
                break;
            }
            case 13: {
                opener += "<font=\"Brook SDF\">";
                break;
            }
            case 14: {
                opener += "<font=\"LiberationSans SDF\">";
                break;
            }
            case 15: {
                opener += "<font=\"NotoSansJP-Regular SDF\">";
                break;
            }
            case 16: {
                opener += "<font=\"VCR SDF\">";
                break;
            }
            case 17: {
                opener += "<font=\"CONSOLA SDF\">";
                break;
            }
            case 18: {
                opener += "<font=\"digital-7 SDF\">";
                break;
            }
            case 19: {
                opener += "<font=\"OCRAEXT SDF\">";
                break;
            }
            case 20: {
                opener += "<font=\"DIN_Pro_Bold_700 SDF\">";
                break;
            }
            }
            closer += "</font>";
        }

        if (resizeName) {
            opener += std::format("<size={}%>", nameSize * 100);
            closer += "</size>";
        }

        if (indentName) {
            opener += std::format("<line-indent={}>", indentLevel);
            closer += "</line-indent>";
        }

        if (cspaceName) {
            opener += std::format("<cspace={}>", cspaceLevel);
            closer += "</cspace>";
        }

        if (mspaceName) {
            opener += std::format("<mspace={}>", mspaceLevel);
            closer += "</mspace>";
        }

        if (voffsetName) {
            opener += std::format("<voffset={}>", voffsetLevel);
            closer += "</voffset>";
        }

        if (rotateName) {
            opener += std::format("<rotate={}>", rotateAngle);
            closer += "<rotate=0>";
        }

        return opener + str + closer;
    }

    void Render() {
        ImGui::SameLine(100 * State.dpiScale);
        ImGui::BeginChild("###SelfButtons", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (TabGroup("Visuals", openVisuals)) {
            CloseOtherGroups(Groups::Visuals);
        }
        ImGui::SameLine();
        if (TabGroup("Utils", openUtils)) {
            CloseOtherGroups(Groups::Utils);
        }
        ImGui::SameLine();
        if (TabGroup("Roles", openRoles)) {
            CloseOtherGroups(Groups::Roles);
        }
        ImGui::SameLine();
        if (TabGroup("Randomizers", openRandomizers)) {
            CloseOtherGroups(Groups::Randomizers);
        }
        ImGui::SameLine();
        if (TabGroup("Anti-Exploit", openAntiExploit)) {
            CloseOtherGroups(Groups::AntiExploit);
        }
        ImGui::SameLine();
        if (TabGroup("Text Editor", openTextEditor)) {
            CloseOtherGroups(Groups::TextEditor);
        }

        ImGui::BeginChild("###Self", ImVec2(500 * State.dpiScale, 0), true, ImGuiWindowFlags_NoBackground);
        if (openVisuals) {
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            if (ToggleButton("Disable HUD", &State.DisableHud)) {
                if (!IsInGame()) State.DisableHud = false;
            }

            if (ToggleButton("Show Shadows While Zoomed", &State.EnableZoom_ShowShadows)) {
                State.Save();
            }

            ImGui::Dummy(ImVec2(7, 7) * State.dpiScale);

            if (ToggleButton("Always show Chat Button", &State.ChatAlwaysActive)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Allow Ctrl+(C/V) in Chat", &State.ChatPaste)) { //add copying later
                State.Save();
            }

            if (ToggleButton("Read Messages by Ghosts", &State.ReadGhostMessages)) {
                State.Save();
            }
            // Stealth QoL build: SickoChat toggle removed (custom RPC 101 exposes mod usage).

            if (ToggleButton("Move Match Info Guide HUD Button", &State.MoveMatchInfoGuide)) {
                State.Save();
            }

            if (/*!IsHost() && */State.SafeMode) {
                ImGui::Text("Custom names are purely CLIENT-SIDED!");
            }
            if (ToggleButton("Custom Name", &State.CustomName)) {
                State.Save();
            }

            if (State.CustomName && ImGui::CollapsingHeader("Custom Name Options"))
            {
                if (ToggleButton("Italics", &State.ItalicName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Underline", &State.UnderlineName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Strikethrough", &State.StrikethroughName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Bold", &State.BoldName)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Nobr", &State.NobrName)) {
                    State.Save();
                }

                if (ImGui::ColorEdit4("Starting Gradient Color", (float*)&State.NameColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ImGui::ColorEdit4("Ending Gradient Color", (float*)&State.NameColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Colored", &State.ColoredName)) {
                    State.Save();
                }

                if (ToggleButton("RGB", &State.RgbName)) {
                    State.Save();
                }

                if (CustomListBoxInt("Gradient Method", &State.ColorMethod, { "Static", "Left-to-Right" }, 80.f * State.dpiScale))
                    State.Save();
                ImGui::SameLine();
                if (CustomListBoxInt("RGB Method", &State.RgbMethod, { "All-at-Once", "Left-to-Right" }, 80.f * State.dpiScale))
                    State.Save();

                if (ToggleButton("Enable Prefix and Suffix", &State.UsePrefixAndSuffix)) State.Save();
                if (ToggleButton("New Lines for Prefix and Suffix", &State.PrefixAndSuffixNewLines)) State.Save();

                InputString("Name Prefix", &State.NamePrefix);
                InputString("Name Suffix", &State.NameSuffix);
                if (State.UsePrefixAndSuffix) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Note: Prefix and/or suffix will be cleared from the ends of the name if it contains them."));
                if (State.UsePrefixAndSuffix) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("This is done to prevent name overflowing."));

                if (ToggleButton("Font", &State.Font)) {
                    State.Save();
                }
                if (State.Font) {
                    ImGui::SameLine();
                    if (CustomListBoxInt(" ", &State.FontType, FONTS, 160.f * State.dpiScale)) {
                        State.Save();
                    }
                }
                if (State.Font) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Note: The white nickname will not be visible in the chat"));

                if (ToggleButton("Size", &State.ResizeName)) {
                    State.Save();
                }

                ImGui::SameLine();
                ImGui::InputFloat("Name Size", &State.NameSize);

                if (ToggleButton("Indent", &State.IndentName)) {
                    State.Save();
                }

                ImGui::SameLine();
                ImGui::InputFloat("Name Indent", &State.NameIndent);

                ToggleButton("Cspace", &State.CspaceName);

                ImGui::SameLine();
                ImGui::InputFloat("Name Cspace", &State.NameCspace);

                ToggleButton("Mspace", &State.MspaceName);

                ImGui::SameLine();
                ImGui::InputFloat("Name Mspace", &State.NameMspace);

                ToggleButton("Voffset", &State.VoffsetName);

                ImGui::SameLine();
                ImGui::InputFloat("Name Voffset", &State.NameVoffset);
                if (ToggleButton("Rotate", &State.RotateName)) {
                    State.Save();
                }

                ImGui::SameLine();
                ImGui::InputFloat("Rotation Angle", &State.NameRotate);
                ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            }

            if (ToggleButton("Localize Role Names", &State.LocalizeRoleNames))
            {
                if (State.LocalizeRoleNames) State.AbbreviatedRoleNames = false;
                State.Save();
            }
            if (!State.LocalizeRoleNames) ImGui::SameLine();
            if (!State.LocalizeRoleNames && ToggleButton("Abbreviate Role Names", &State.AbbreviatedRoleNames))
            {
                State.Save();
            }

            if (ToggleButton("Player Colored Dots Next To Names", &State.PlayerColoredDots))
            {
                State.Save();
                State.MIG_ThemeChanged = true;
            }

            if (ToggleButton("Show Player Info in Lobby", &State.ShowPlayerInfo))
            {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Show Lobby Info", &State.ShowLobbyInfo))
            {
                State.Save();
            }

            if (ToggleButton("Hide Whitelisted Players' Info", &State.HideWhitelistedPlayerInfo))
            {
                State.Save();
            }

            if (ToggleButton("Reveal Votes", &State.RevealVotes)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Reveal Anonymous Votes", &State.RevealAnonymousVotes)) {
                State.Save();
                RevealAnonymousVotes();
            }

            if (ToggleButton("Disable Kill Animation", &State.DisableKillAnimation)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Disable Lobby Music", &State.DisableLobbyMusic)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Old Ping Text", &State.OldStylePingText)) State.Save();

            if (ToggleButton("Show Host", &State.ShowHost)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Show Vote Kicks", &State.ShowVoteKicks)) {
                State.Save();
            }

            if (ToggleButton("Show Chat Cooldown", &State.ShowChatTimer)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Extend Chat Character Limit", &State.ExtendChatLimit)) {
                State.Save();
            }

        }

        if (openUtils) {
            if (ToggleButton("Copy Lobby Code on Disconnect", &State.AutoCopyLobbyCode)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("No Seeker Animation", &State.NoSeekerAnim)) State.Save();

            if (ToggleButton("Better Chat Notifications", &State.BetterChatNotifications)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Better Lobby Code Input", &State.BetterLobbyCodeInput)) {
                State.Save();
            }
            
            if (ToggleButton("Better Message Sounds", &State.BetterMessageSounds)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Extended Notifications", &State.ExtendedNotifications)) {
                State.Save();
            }

            if (ToggleButton("Auto Rejoin After Game Ending", &State.AutoRejoin)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Auto-Rejoin on Votekick", &State.AutoRejoinOnKick)) {
                State.Save();
            }

            if (ToggleButton("Disable Shush Animation", &State.DisableShushAnimation)) {
                State.Save();
            }

            if (ToggleButton("Control Pet", &State.ControlPet)) {
                if (*Game::pLocalPlayer == nullptr || (!IsInGame() && !IsInLobby())) State.ControlPet = false;
                if (!State.ControlPet) State.DisableControlPetHand = true;
            }
            if (ToggleButton("Report Body on Murder", &State.ReportOnMurder)) {
                State.Save();
            }
            if (State.ReportOnMurder) {
                ImGui::SameLine();
                if (ToggleButton("Prevent Self-Report", &State.PreventSelfReport)) {
                    State.Save();
                }
            }

        }

        if (openRoles) {
            if (((IsHost() && IsInGame()) || !State.SafeMode) && ToggleButton(IsHost() ? "God Mode" : "Visual Protection", &State.GodMode))
                State.Save();
            if (ToggleButton("No Ladder/Zipline Cooldown", &State.NoLadderZiplineCooldown)) State.Save();
            if (ToggleButton("Do Tasks as Impostor", &State.DoTasksAsImpostor)) State.Save();
        }

        if (openRandomizers) {
            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);

            if (ToggleButton("Cycler", &State.Cycler)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton("Cycle in Meeting", &State.CycleInMeeting)) {
                State.Save();
            }
            ImGui::SameLine();
            if (ToggleButton(State.SafeMode ? "Cycle Between Players' Outfits" : "Cycle Between Players", &State.CycleBetweenPlayers)) {
                State.Save();
            }

            if (SteppedSliderFloat("Cycle Timer", &State.CycleTimer, 0.2f, 1.f, 0.02f, "%.2fs", ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput)) {
                State.PrevCycleTimer = State.CycleTimer;
                State.CycleDuration = State.CycleTimer * 50;
            }

            ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
            if (ImGui::CollapsingHeader("Cycler Options")) {
                ImGui::Dummy(ImVec2(4, 2)* State.dpiScale);
                if (ToggleButton("Cycle Color", &State.RandomColor)) {
                    State.Save();
                }
                ImGui::SameLine();
                if (ToggleButton("Cycle Hat", &State.RandomHat)) {
                    State.Save();
                }
                ImGui::SameLine(240.0f * State.dpiScale);
                if (ToggleButton("Cycle Nameplate", &State.RandomNamePlate)) {
                    State.Save();
                }
                if (ToggleButton("Cycle Visor", &State.RandomVisor)) {
                    State.Save();
                }

                ImGui::SameLine(120.0f * State.dpiScale);
                if (ToggleButton("Cycle Skin", &State.RandomSkin)) {
                    State.Save();
                }

                ImGui::SameLine(240.0f * State.dpiScale);
                if (ToggleButton("Cycle Pet", &State.RandomPet)) {
                    State.Save();
                }
            }

            ImGui::Dummy(ImVec2(4, 4)* State.dpiScale);

            bool isPresetDeleted = false;
            if (ImGui::CollapsingHeader("Cosmetic Presets", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Dummy(ImVec2(4, 2) * State.dpiScale);
                if (ToggleButton("Auto Apply on Join", &State.AutoApplyCosmeticPreset))
                    State.Save();
                ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
                if (!State.CosmeticPresets.empty()) {
                    std::vector<const char*> names;
                    for (auto& p : State.CosmeticPresets) names.push_back(p.Name.c_str());
                    CustomListBoxInt("Preset", &State.SelectedCosmeticPreset, names, 200.0f * State.dpiScale, ImVec4(0, 0, 0, 0), 0);
                    ImGui::SameLine();
                    if (AnimatedButton("Apply##cosmeticpreset")) {
                        ApplyCosmeticPreset(State.CosmeticPresets[std::clamp(State.SelectedCosmeticPreset, 0, (int)State.CosmeticPresets.size() - 1)]);
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Update##cosmeticpreset")) {
                        auto outfit = GetPlayerOutfit(GetPlayerData(*Game::pLocalPlayer));
                        if (outfit != nullptr) {
                            int idx = std::clamp(State.SelectedCosmeticPreset, 0, (int)State.CosmeticPresets.size() - 1);
                            auto& p = State.CosmeticPresets[idx];
                            p.ColorId = outfit->fields.ColorId;
                            p.HatId = outfit->fields.HatId ? convert_from_string(outfit->fields.HatId) : "";
                            p.SkinId = outfit->fields.SkinId ? convert_from_string(outfit->fields.SkinId) : "";
                            p.VisorId = outfit->fields.VisorId ? convert_from_string(outfit->fields.VisorId) : "";
                            p.PetId = outfit->fields.PetId ? convert_from_string(outfit->fields.PetId) : "";
                            p.NamePlateId = outfit->fields.NamePlateId ? convert_from_string(outfit->fields.NamePlateId) : "";
                            State.Save();
                        }
                    }
                    ImGui::SameLine();
                    if (AnimatedButton("Delete##cosmeticpreset")) {
                        int idx = std::clamp(State.SelectedCosmeticPreset, 0, (int)State.CosmeticPresets.size() - 1);
                        State.CosmeticPresets.erase(State.CosmeticPresets.begin() + idx);
                        if (State.CosmeticPresets.size() != 0)
                            State.SelectedCosmeticPreset = std::clamp(State.SelectedCosmeticPreset, 0, (int)State.CosmeticPresets.size() - 1);
                        isPresetDeleted = true;
                        State.Save();
                    }
                }
                else {
                    ImGui::TextDisabled("No cosmetic presets saved.");
                }

                if (!isPresetDeleted) {
                    ImGui::Dummy(ImVec2(4, 4) * State.dpiScale);
                    static std::string newCosmeticName = "My Outfit";
                    ImGui::SetNextItemWidth(160 * State.dpiScale);
                    InputString("Preset Name##cosmetic", &newCosmeticName);
                    ImGui::SameLine();
                    if (AnimatedButton("Save Current##cosmeticpreset")) {
                        auto outfit = GetPlayerOutfit(GetPlayerData(*Game::pLocalPlayer));
                        if (outfit != nullptr) {
                            Settings::CosmeticPreset p;
                            p.Name = newCosmeticName.empty() ? "Preset" : newCosmeticName;
                            p.ColorId = outfit->fields.ColorId;
                            p.HatId = outfit->fields.HatId ? convert_from_string(outfit->fields.HatId) : "";
                            p.SkinId = outfit->fields.SkinId ? convert_from_string(outfit->fields.SkinId) : "";
                            p.VisorId = outfit->fields.VisorId ? convert_from_string(outfit->fields.VisorId) : "";
                            p.PetId = outfit->fields.PetId ? convert_from_string(outfit->fields.PetId) : "";
                            p.NamePlateId = outfit->fields.NamePlateId ? convert_from_string(outfit->fields.NamePlateId) : "";
                            State.CosmeticPresets.push_back(p);
                            State.SelectedCosmeticPreset = (int)State.CosmeticPresets.size() - 1;
                            State.Save();
                        }
                    }
                }
            }
        }

        if (openAntiExploit) {
            if (ToggleButton("No Disconnect Penalties", &State.AntiExploit_DisconnectPenalties)) State.Save();
            if (ToggleButton("Resist Targeted Sabotages (Non-Host)", &State.AntiExploit_UnauthorizedSabotages)) State.Save();
            if (ToggleButton("Resist Unauthorized Teleports", &State.AntiExploit_UnauthorizedTeleports)) State.Save();
            if (ToggleButton("Resist Unauthorized Ziplines", &State.AntiExploit_UnauthorizedZiplines)) State.Save();
            if (ToggleButton("Resist Attempt to Ban", &State.AntiExploit_AttemptToBan)) State.Save();

            ImGui::NewLine();
            ImGui::Text("Anti-Exploits for Hosts");
            if (ToggleButton("Resist Votekicks Against Self", &State.AntiExploit_VotekicksAgainstSelfHost)) State.Save();
            if (ToggleButton("Prevent Attempt to Crash Lobby", &State.AntiExploit_CrashLobbyHost)) State.Save();
        }

        if (openTextEditor) {
            InputString("Input", &originalText);
            editedText = GetTextEditorName(originalText);
            InputString("Output", &editedText);
            ImGui::SameLine();
            if (AnimatedButton("Copy")) ClipboardHelper_PutClipboardString(convert_to_string(editedText), NULL);

            ToggleButton("Italics", &italicName);
            ImGui::SameLine();
            ToggleButton("Underline", &underlineName);
            ImGui::SameLine();
            ToggleButton("Strikethrough", &strikethroughName);
            ImGui::SameLine();
            ToggleButton("Bold", &boldName);
            ImGui::SameLine();
            ToggleButton("Nobr", &nobrName);

            ImGui::ColorEdit4("Starting Gradient Color", (float*)&nameColor1, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
            ImGui::SameLine();
            ImGui::ColorEdit4("Ending Gradient Color", (float*)&nameColor2, ImGuiColorEditFlags__OptionsDefault | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview);
            ImGui::SameLine();
            ToggleButton("Colored", &coloredName);

            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);

            ToggleButton("Font", &font);
            ImGui::SameLine();
            CustomListBoxInt(" ", &fontType, FONTS, 160.f * State.dpiScale);
            ImGui::Dummy(ImVec2(-5, -5) * State.dpiScale);
            if (State.Font) ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ("Note: The white nickname will not be visible in the chat"));

            ImGui::Dummy(ImVec2(2, 2) * State.dpiScale);

            ImGui::Dummy(ImVec2(10, 10) * State.dpiScale);
            ToggleButton("Size", &resizeName);

            ImGui::SameLine();
            ImGui::InputFloat("Name Size", &nameSize);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Indent", &indentName);

            ImGui::SameLine();
            ImGui::InputFloat("Name Indent", &indentLevel);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Cspace", &cspaceName);

            ImGui::SameLine();
            ImGui::InputFloat("Name Cspace", &cspaceLevel);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Mspace", &mspaceName);

            ImGui::SameLine();
            ImGui::InputFloat("Name Mspace", &mspaceLevel);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Voffset", &voffsetName);

            ImGui::SameLine();
            ImGui::InputFloat("Name Voffset", &voffsetLevel);

            ImGui::Dummy(ImVec2(5, 5) * State.dpiScale);
            ToggleButton("Rotate", &rotateName);

            ImGui::SameLine();
            ImGui::InputFloat("Rotation Angle", &rotateAngle);
        }
        ImGui::EndChild();
        ImGui::EndChild();
    }
}
