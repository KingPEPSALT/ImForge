#include "core/ui/menu_bar.hpp"
#include <imgui_internal.h>
#include <spdlog/spdlog.h>
#include <string>

namespace ImForge::Core::UI {

bool MenuBar::attach() {

    const char* error_head = "MenuBar::attach() failed:\n";
    
    if(!ImGui::GetCurrentContext()){
        spdlog::error(
            "{:s}\tcalled with no current ImGuiContext available, did you call ImGui::CreateContext?",
            error_head
        );
        return false;
    }

    if(ImGui::BeginMenuBar()){
        for(auto menu : this->menus) {
            if(ImGui::BeginMenu(menu.c_str())){
                for(MenuItem& menu_item : this->menu_items[menu])
                    ImGui::MenuItem(
                        menu_item.name.c_str(),
                        menu_item.shortcut,
                        &menu_item.selected,
                        menu_item.enabled
                    );

                ImGui::EndMenu();
            }
        }
        ImGui::EndMenuBar();
    } else {
        spdlog::error(
            "{:s}\tImGui::BeginMenuBar() failed,"
            " did you pass ImGuiWindowFlags_MenuBar to ImGui::Begin()?",
            error_head
        );
        return false;
    }
    return true;

}

void MenuBar::addItems(
    std::string menu,
    std::vector<MenuBar::MenuItem> menu_items
){
    if(!this->menu_items.contains(menu))
        return;
    this->menu_items[menu].insert(
        this->menu_items[menu].end(),
        menu_items.begin(), menu_items.end()
    );
};

void MenuBar::addItems(
    std::string menu,
    std::vector<std::string> menu_items
){
    if(!this->menu_items.contains(menu))
        return;
    for(std::string label : menu_items)
        this->addItem(menu, label);
};


MenuBar::MenuItem MenuBar::removeItem(std::string menu, std::vector<MenuItem>::const_iterator pos){
    if(!this->menu_items.contains(menu))
        return MenuBar::MenuItem{};
    MenuItem ret = *pos;
    this->menu_items[menu].erase(pos);
    return ret;
}

MenuBar::MenuItem MenuBar::removeItem(std::string menu, std::string label) {
    auto it = this->menu_items[menu].begin();
    while(it != this->menu_items[menu].end() && (it++)->name != label);
    return this->removeItem(menu, it-1);
}

void MenuBar::removeMenu(std::string menu) {
    if(!this->menu_items.contains(menu))
        return;
    auto it = this->menus.begin();
    while(*(it++) != menu);
    this->menus.erase(--it);
    this->menu_items.erase(menu);
}

std::vector<MenuBar::MenuItem> MenuBar::takeMenu(std::string menu){
    if(!this->menu_items.contains(menu))
        return std::vector<MenuBar::MenuItem>();
    const std::vector<MenuBar::MenuItem> menu_items = this->menu_items.at(menu);
    this->removeMenu(menu);
    return menu_items;
}

}