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
        for(std::string menu_name : this->menu_names) {
            if(ImGui::BeginMenu(menu_name.c_str())){
                for(MenuItem& item : this->menus[menu_name])
                    ImGui::MenuItem(
                        item.name.c_str(),
                        item.shortcut,
                        &item.selected,
                        item.enabled
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

void MenuBar::pollEvents() {
    for(auto& menu: this->menus)
        for(auto& item: menu.second){
            if(!item.selected)
                continue;
            if(item.first_select && item.on_first_select_callback) {
                item.on_first_select_callback(*this);
                item.first_select = false;
                continue;
            }
            if(!item.on_select_callback)
                continue;
            item.on_select_callback(*this);
        }
}

// we may wish to throw exceptions here if nothing is inserted/before isn't found
MenuBar& MenuBar::insertMenuAfter(
    std::string before, 
    std::string menu_name,
    std::vector<MenuItem> menu_items
){
    auto pos = this->menu_names.begin();
    while(pos != this->menu_names.end() && *(pos++) != before);
    if(*(pos-1) == before)
        this->insertMenu(pos, menu_name, menu_items);
    return *this;
}

MenuBar& MenuBar::insertMenuBefore(
    std::string after, 
    std::string menu_name,
    std::vector<MenuItem> menu_items
){
    auto pos = this->menu_names.begin();
    while(pos != this->menu_names.end() && *(pos++) != after);
    if(*(--pos) == after)
        this->insertMenu(pos, menu_name, menu_items);
    return *this;
    
}

MenuBar& MenuBar::addItems(
    std::string menu_name,
    std::vector<MenuBar::MenuItem> menu_items
){
    if(!this->menus.contains(menu_name))
        return *this; // may want an exception
    this->menus[menu_name].insert(
        this->menus[menu_name].end(),
        menu_items.begin(), menu_items.end()
    );
    return *this;
};

// same as earlier (line 63), we may wish to throw exceptions here
MenuBar& MenuBar::insertItemAfter(
    std::string menu_name,
    std::string item_before,
    MenuItem item
){ 
    auto pos = this->menus[menu_name].begin();
    while(pos != this->menus[menu_name].end() && (pos++)->name != item_before);
    if((pos-1)->name == item_before)
        this->insertItem(menu_name, item, pos);
    return *this;
}

MenuBar& MenuBar::insertItemBefore(
    std::string menu_name,
    std::string item_after,
    MenuItem item
){
    auto pos = this->menus[menu_name].begin();
    while(pos != this->menus[menu_name].end() && (pos++)->name != item_after);
    if((--pos)->name == item_after)
        this->insertItem(menu_name, item, pos);
    return *this;
}

MenuBar::MenuItem MenuBar::removeItem(std::string menu_name, std::vector<MenuItem>::const_iterator pos){
    if(!this->menus.contains(menu_name))
        return MenuBar::MenuItem{};
    MenuItem ret = *pos;
    this->menus[menu_name].erase(pos);
    return ret;
}

MenuBar::MenuItem MenuBar::removeItem(std::string menu_name, std::string item_name) {
    auto it = this->menus[menu_name].begin();
    while(it != this->menus[menu_name].end() && (it++)->name != item_name);
    return this->removeItem(menu_name, it-1);
}

void MenuBar::removeMenu(std::string menu_name) {
    if(!this->menus.contains(menu_name))
        return;
    auto it = this->menu_names.begin();
    while(it != this->menu_names.end() && *(it++) != menu_name);
    this->menu_names.erase(--it);
    this->menus.erase(menu_name);
}

void MenuBar::removeMenu(std::vector<std::string>::const_iterator pos) {
    if(!this->menus.contains(*pos))
        return;
    this->menu_names.erase(pos);
    this->menus.erase(*pos);
}

std::vector<MenuBar::MenuItem> MenuBar::takeMenu(std::string menu_name){
    if(!this->menus.contains(menu_name))
        return std::vector<MenuBar::MenuItem>();
    const std::vector<MenuBar::MenuItem> menu_items = this->menus.at(menu_name);
    this->removeMenu(menu_name);
    return menu_items;
}
std::vector<MenuBar::MenuItem> MenuBar::takeMenu(std::vector<std::string>::const_iterator pos){
    if(!this->menus.contains(*pos))
        return std::vector<MenuBar::MenuItem>();
    const std::vector<MenuBar::MenuItem> menu_items = this->menus.at(*pos);
    this->removeMenu(pos);
    return menu_items;
}

}