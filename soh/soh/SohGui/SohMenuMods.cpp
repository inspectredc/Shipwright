#include "SohMenu.h"
#include "SohGui.hpp"
#include "soh/OTRGlobals.h"
#include "ship/scripting/ScriptLoader.h"
#include "ship/utils/StringHelper.h"

namespace SohGui {

extern std::shared_ptr<SohMenu> mSohMenu;

void SohMenu::AddMenuMod() {
    auto mods = Ship::Context::GetInstance()->GetResourceManager()->GetArchiveManager()->GetArchives();
    AddMenuEntry("Mods", CVAR_SETTING("Menu.ModsSidebarSection"));

    WidgetPath path = { "Mods", "General", SECTION_COLUMN_1 };

    AddSidebarEntry(path.sectionName, path.sidebarName, 1);
    AddWidget(path, "Reload Scripts", WIDGET_BUTTON)
        .Options(UIWidgets::ButtonOptions().Tooltip("Reloads all scripts from disk.").Color(UIWidgets::Colors::Orange))
        .Callback([](WidgetInfo& info) {
            Ship::Context::GetInstance()->GetScriptLoader()->UnloadAll();
            OTRGlobals::Instance->LoadScripts();
        });

    auto keystore = Ship::Context::GetInstance()->GetKeystore();
    auto allKeys = keystore->GetAllKeys();

    for (const auto& entry : *mods) {
        const auto& info = entry->GetManifest();
        if (info.Name.empty()) {
            continue;
        }

        std::string cardTitle = info.Name;

        if (!info.Icon.empty()) {
            cardTitle = info.Icon + " " + cardTitle;
        }

        if (!info.Main.empty() || !info.Binaries.empty()) {
            cardTitle += " (Code Mod)";
        }

        AddWidget(path, cardTitle, WIDGET_SEPARATOR_TEXT).Options(UIWidgets::TextOptions{});

        std::string metadata = "Author: " + (info.Author.empty() ? "Unknown" : info.Author);

        if (!info.Version.empty()) {
            metadata += "  |  Version: " + info.Version;
        }
        if (!info.License.empty()) {
            metadata += "  |  License: " + info.License;
        }

        AddWidget(path, metadata, WIDGET_TEXT).Options(UIWidgets::TextOptions{});
        Ship::KeyOrigin origin = Ship::KeyOrigin::User;
        for (const auto& key : allKeys) {
            if (key.Data == StringHelper::HexToBytes(info.PublicKey)) {
                origin = key.Origin;
                break;
            }
        }

        std::string securityText;
        if (entry->IsSigned()) {
            securityText = std::string(ICON_FA_CHECK_CIRCLE) + " Security: Signed (Trusted)";
            std::string originText;
            UIWidgets::Colors color = UIWidgets::Colors::Green;
            switch (origin) {
                case Ship::KeyOrigin::User:
                    originText = "[User Approved]";
                    color = UIWidgets::Colors::Yellow;
                    break;
                case Ship::KeyOrigin::Game:
                    originText = "[Game]";
                    color = UIWidgets::Colors::Purple;
                    break;
                case Ship::KeyOrigin::System:
                    originText = "[System]";
                    color = UIWidgets::Colors::Red;
                    break;
            }

            AddWidget(path, securityText, WIDGET_TEXT).Options(UIWidgets::TextOptions{ .color = UIWidgets::Colors::Green });
            AddWidget(path, originText, WIDGET_TEXT).SameLine(true).Options(UIWidgets::TextOptions{ .color = color });
        } else if (entry->IsChecksumValid()) {
            securityText = std::string(ICON_FA_EXCLAMATION_TRIANGLE) + " Security: Unsigned (Caution)";
            AddWidget(path, securityText, WIDGET_TEXT).Options(UIWidgets::TextOptions{ .color = UIWidgets::Colors::Orange });
        } else {
            securityText = std::string(ICON_FA_EXCLAMATION_TRIANGLE) + " Security: Untrusted";
            AddWidget(path, securityText, WIDGET_TEXT).Options(UIWidgets::TextOptions{ .color = UIWidgets::Colors::Red });
        }

        if (!info.Dependencies.empty()) {
            std::string depsString = "Dependencies: ";
            for (size_t i = 0; i < info.Dependencies.size(); ++i) {
                depsString += info.Dependencies[i];
                if (i < info.Dependencies.size() - 1)
                    depsString += ", ";
            }

            AddWidget(path, depsString, WIDGET_TEXT).Options(UIWidgets::TextOptions{});
        }

        if (!info.Description.empty()) {
            AddWidget(path, info.Description, WIDGET_TEXT).Options(UIWidgets::TextOptions{});
        }

        if (!info.Website.empty()) {
            AddWidget(path, "Open Webpage##" + info.Name, WIDGET_BUTTON)
                .Options(UIWidgets::ButtonOptions{})
                .Callback([info](WidgetInfo&) { SDL_OpenURL(info.Website.c_str()); });
        }

        AddWidget(path, "##Spacer_" + info.Name, WIDGET_SEPARATOR).Options(UIWidgets::WidgetOptions{});
    }
};
}
