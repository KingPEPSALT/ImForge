#pragma once

#include <imgui.h>
#include <vector>
#include <map>
#include <spdlog/spdlog.h>
#include <string>
namespace ImForge::Core::UI {

/**
 * @brief A menu bar that would be displayed at the top of
 * a window. 
 */
struct MenuBar {
    
    /**
     * @brief A menu item contained within a menu
     */
    struct MenuItem {
        std::string name; 
        const char* shortcut;
        bool selected;
        bool enabled;
        void (*on_select_callback)(MenuBar& parent);
        void (*on_first_select_callback)(MenuBar& parent);
        bool first_select;    
        
        MenuItem(
            std::string name,
            const char* shortcut = NULL,
            bool selected = false,
            bool enabled = true,
            void (*on_select_callback)(MenuBar& parent) = nullptr,
            void (*on_first_select_callback)(MenuBar& parent) = nullptr,
            bool first_select = true
        ) : name(name)
            , shortcut(shortcut)
            , selected(selected)
            , enabled(enabled)
            , on_select_callback(on_select_callback)
            , on_first_select_callback(on_first_select_callback)
            , first_select(first_select) {};
        
        MenuItem(
            const char* name,
            const char* shortcut = NULL,
            bool selected = false,
            bool enabled = true,
            void (*on_select_callback)(MenuBar& parent) = nullptr,
            void (*on_first_select_callback)(MenuBar& parent) = nullptr,
            bool first_select = true
        ) : name(name) 
            , shortcut(shortcut)
            , selected(selected)
            , enabled(enabled)
            , on_select_callback(on_select_callback)
            , on_first_select_callback(on_first_select_callback)
            , first_select(first_select) {};

        MenuItem() {};
    };


    /* 
        maps don't maintain insertion order and so without using
        another library, (as there is nothing in the std of the
        like), we just use a vector.

        to avoid writing our own const char* comparison struct
        we opt to use std::string for the map's keyval, for why
        we can't use const char*:
            - https://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap
    */
    std::vector<std::string> menu_names;
    std::map<std::string, std::vector<MenuItem>> menus;

    bool attach();

    void pollEvents();

    inline MenuItem* getItemMut(std::string menu_name, std::string item_name){
        auto pos = this->menus[menu_name].begin();
        while(pos != this->menus[menu_name].end() && (pos++)->name != item_name);
        return (--pos)._Ptr;
    }

    inline MenuItem getItem(std::string menu_name, std::string item_name){
        return *this->getItemMut(menu_name, item_name);
    }

    inline std::vector<MenuItem>* getMenuMut(std::string name){
        return &this->menus[name];
    }

    inline std::vector<MenuItem> getMenu(std::string name){
        // we need to throw an exception if it doesn't contain it 
        return this->menus[name];
    }

    inline MenuBar& addMenu(
        std::string menu_name,
        std::vector<MenuItem> menu_items = std::vector<MenuItem>()
    ) {
        this->menu_names.push_back(menu_name);
        this->menus.insert(
            std::make_pair(menu_name, menu_items)
        );
        return *this;
    }

    inline MenuBar& insertMenu(
        std::vector<std::string>::const_iterator pos, 
        std::string menu_name, 
        std::vector<MenuItem> menu_items = std::vector<MenuItem>()
    ) {
        this->menu_names.insert(pos, menu_name);
        this->menus.insert(
            std::make_pair(menu_name, menu_items)
        );
        return *this;
    }

    inline MenuBar& insertMenu(
        size_t index, 
        std::string menu_name,
        std::vector<MenuItem> menu_items = std::vector<MenuItem>()
    ){
        this->insertMenu(this->menu_names.begin() + index, menu_name, menu_items);
        return *this;
    }

    MenuBar& insertMenuAfter(
        std::string menu_before, 
        std::string menu_name,
        std::vector<MenuItem> menu_items = std::vector<MenuItem>()
    );

    MenuBar& insertMenuBefore(
        std::string menu_after, 
        std::string menu_name,
        std::vector<MenuItem> menu_items = std::vector<MenuItem>()
    );

    inline MenuBar& addMenus(
        std::vector<std::string> menu_names,
        std::map<std::string, std::vector<MenuItem>> menus =
            std::map<std::string, std::vector<MenuItem>>() 
    ) {
        for(std::string menu_name : menu_names)
            this->addMenu(
                menu_name,
                menus.contains(menu_name) 
                    ? menus.at(menu_name) 
                    : std::vector<MenuItem>()    
            );
        return *this;
    }

    inline MenuBar& addItem(
        std::string menu_name,
        MenuItem item
    ){
        if(!this->menus.contains(menu_name))
            return *this; // may want to throw an exception here
        this->menus[menu_name].push_back(item);
        return *this;
    }

    MenuBar& addItems(
        std::string menu_name,
        std::vector<MenuItem> menu_items
    );

    inline MenuBar& insertItem(
        std::string menu_name,
        MenuItem item,
        std::vector<MenuItem>::const_iterator pos
    ) {
        this->menus[menu_name].insert(pos, item);
        return *this;
    };

    inline MenuBar& insertItem(
        std::string menu_name,
        MenuItem item,
        size_t index
    ) {
        this->menus[menu_name].insert(
            this->menus[menu_name].begin() + index, 
            item
        );
        return *this;
    };

    MenuBar& insertItemAfter(
        std::string menu_name, 
        std::string item_before,
        MenuItem item
    );

    MenuBar& insertItemBefore(
        std::string menu_name, 
        std::string item_after,
        MenuItem item
    );

    MenuItem removeItem(std::string menu_name, std::vector<MenuItem>::const_iterator pos);

    MenuItem removeItem(std::string menu_name, std::string item_name);

    inline MenuItem removeItem(std::string menu_name, size_t index) {
        return this->removeItem(menu_name, this->menus[menu_name].begin() + index);
    }

    void removeMenu(std::string menu_name);

    void removeMenu(std::vector<std::string>::const_iterator pos);

    std::vector<MenuItem> takeMenu(std::string menu_name);

    std::vector<MenuItem> takeMenu(std::vector<std::string>::const_iterator pos);

    /*
        ========================================
        ============+--------------+============
        ============| CONSTRUCTORS |============
        ============+--------------+============
        ========================================
    */

    MenuBar(
        std::vector<std::string> menu_names
            = std::vector<std::string>(),
        std::map<std::string, std::vector<MenuItem>> menus 
            = std::map<std::string, std::vector<MenuItem>>()
    ) : menu_names(menu_names), menus(menus) {};

    MenuBar(
        std::map<std::string, std::vector<MenuItem>> menu
    ) : menu_names(), menus(menus) {
        for(const auto& menu : menus)
            menu_names.push_back(menu.first.c_str());
    };

};

}