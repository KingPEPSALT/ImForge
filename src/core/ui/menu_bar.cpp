#include "core/ui/menu_bar.hpp"
#include <imgui_internal.h>
#include <spdlog/spdlog.h>
#include <string>

namespace ImForge::Core::UI {

bool MenuBar::attach() {
    // if(!dockWindow){
    //     spdlog::error(
    //         "MenuBar::attach(ImGuiWindow* dockWindow) failed:\n"
    //         "\tdockWindow is nullptr"
    //     );
    //     return false;
    // }
    
    const char* error_head = "MenuBar::attach() failed:\n";
    
    if(!ImGui::GetCurrentContext()){
        spdlog::error(
            "%s\tcalled with no current ImGuiContext available, did you call ImGui::CreateContext?",
            error_head
        );
        return false;
    }

    // if(!dockWindow->WasActive){
    //     spdlog::error(
    //         "%s\tcalled with dockWindow inactive, did you call ImGui::Begin?",
    //         rich_error_text
    //     );
    //     return false;
    // }

    ImGui::BeginMenuBar();
    for(const char* option : options){
        if(!ImGui::BeginMenu(option)){
            spdlog::error(
                "%s\tImGui::BeginMenu(const char* label) failed on \"%s\"",
                error_head,
                option
            );
            return false;
        }
        for(MenuItem suboption : this->suboptions[option]){
            if(!ImGui::MenuItem(suboption.name, suboption.shortcut, &suboption.selected, suboption.enabled)){
                spdlog::error(
                    "%s\tImGui::MenuItem(const char* label, ...) with label=\"%s\" failed",
                    error_head,
                    suboption
                );
                return false;
            }
        }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
}

void MenuBar::addMenu(const char* label) {
    this->options.push_back(label);
}

void MenuBar::addItem(
    const char* menu,
    const char* label, 
    const char* shortcut, 
    bool selected, 
    bool enabled
) {
    this->suboptions[menu].push_back(MenuItem{
        label,
        shortcut,
        selected,
        enabled
    });
}

void MenuBar::removeItem(const char* menu, const char* label) {
    // get iterator from const char* label

    // call same func with iterator param
}
MenuBar::MenuBar() {

}

MenuBar::~MenuBar() {

}

}