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
        void (*on_select_callback)(MenuBar* parent);
        void (*on_first_select_callback)(MenuBar* parent);
        bool first_select;
    };

    /* 
        to avoid writing our own const char* comparison struct
        we opt to use std::string for the map's keyval, for why
        we can't use const char*:
            - https://stackoverflow.com/questions/4157687/using-char-as-a-key-in-stdmap
    */
    std::vector<std::string> menus;
    std::map<std::string, std::vector<MenuItem>> menu_items;

    /**
     * @brief attach the MenuBar to the current window
     * 
     * @return true on success;
     * @return false on fail.
     */
    bool attach();

    /**
     * @brief call on_select_callbacks 
     * 
     * @return true on success;
     * @return false on fail. 
     */
    void pollEvents();

    /**
     * @brief add a menu to the MenuBar 
     * 
     * @param label 
     */
    inline void addMenu(std::string label) {
        this->menus.push_back(label);
        this->menu_items.insert(
            std::make_pair(label, std::vector<MenuItem>())
        );
    }

    /**
     * @brief add a vector of menus to the MenuBar
     * 
     * @param menus the vector of menus  
     */
    inline void addMenus(const std::vector<std::string>& menus) {
        for(std::string menu : menus)
            this->addMenu(menu);
    }


    /**
     * @brief adds a MenuItem to a menu on the MenuBar
     * 
     * @param menu the label of the menu to add to
     * @param menu_item the MenuItem to add
     */
    inline void addItem(
        std::string menu,
        MenuItem menu_item
    ){
        if(!this->menu_items.contains(menu))
            return;
        this->menu_items[menu].push_back(menu_item);
    }

    /**
     * @brief adds a MenuItem to a menu on the MenuBar
     * 
     * @param menu the label of the menu to add to
     * @param label the shown label of the MenuItem
     * @param selected whether the MenuItem is selected, set to false by default
     * @param enabled whether the MenuItem is enabled, set to true by default
     * @param shortcut shortcut text - i.e., CTRL-S for Save - set to NULL by default
     * @param on_select_callback a function pointer to a function that is called on select
     * @param on_first_select_callback a function pointer to a function that runs on the first select
     */
    inline void addItem(
        std::string menu,
        std::string label, 
        const char* shortcut = NULL,
        bool selected = false, 
        bool enabled = true,
        void (*on_select_callback)(MenuBar* parent) = nullptr,
        void (*on_first_select_callback)(MenuBar* parent) = nullptr
    ) {
        this->addItem(menu, MenuItem{
            label, shortcut, selected, enabled, on_select_callback, on_first_select_callback, true
        });
    };

    /**
     * @brief adds a MenuItem to a menu on the MenuBar
     * 
     * @param menu the label of the menu to add to
     * @param menu_items the vector of MenuItem to add
     */
    void addItems(
        std::string menu,
        std::vector<MenuItem> menu_items
    );

    /**
     * @brief adds a MenuItem to a menu on the MenuBar
     * 
     * @param menu the label of the menu to add to
     * @param menu_items the vector of labels of the menu items to add
     */
    void addItems(
        std::string menu,
        std::vector<std::string> menu_items
    );


    /**
     * @brief remove a MenuItem from the MenuBar with an iterator
     * 
     * @param menu the label of the menu with the MenuItem
     * @param pos the position of the MenuItem to delete 
     * @return the MenuItem deleted
     */
    MenuItem removeItem(std::string menu, std::vector<MenuItem>::const_iterator pos);

    /**
     * @brief remove a MenuItem from the MenuBar
     * 
     * @param menu the label of the menu with the MenuItem
     * @param label the label of the MenuItem to delete 
     * @return the MenuItem deleted
     */
    MenuItem removeItem(std::string menu, std::string label);

    /**
     * @brief remove a MenuItem from the MenuBar
     * 
     * @param menu the label of the menu with the MenuItem
     * @param index the index of the MenuItem to delete 
     * @return the MenuItem deleted
     */
    inline MenuItem removeItem(std::string menu, size_t index) {
        return this->removeItem(menu, this->menu_items[menu].begin() + index);
    }


    /**
     * @brief removes a menu (and the MenuItems it contained) from the MenuBar
     * 
     * @param menu the label of the menu to remove
     */
    void removeMenu(std::string menu);

    /**
     * @brief removes a menu from the MenuBar but returns the MenuItems to you
     * 
     * @param menu the label of the menu to remove
     */
    std::vector<MenuItem> takeMenu(std::string menu);

    /*
        ========================================
        ============+--------------+============
        ============| CONSTRUCTORS |============
        ============+--------------+============
        ========================================
    */

    /**
     * @brief constructs a new MenuBar object
     */
    MenuBar() : menus(), menu_items() {};
    
    /**
     * @brief constructs a new MenuBar object
     * 
     * @param menus the vector menus of the MenuBar
     */
    MenuBar(const std::vector<std::string>& menus)
        : menus(menus.begin(), menus.end()), menu_items() {
            for(std::string menu : menus)
                this->menu_items.insert(
                    std::make_pair(menu, std::vector<MenuItem>())
                );
        };

    /**
     * @brief constructs a new MenuBar object
     * 
     * @param menus the vector menus of the MenuBar
     * @param menu_items a map of menus to MenuItems, the menu items of the MenuBar
     */
    MenuBar(
        const std::vector<std::string>& menus,
        const std::map<std::string, std::vector<MenuItem>>& menu_items
    ) : menus(menus.begin(), menus.end()), menu_items(menu_items) {};

    /**
     * @brief constructs a new MenuBar object
     * 
     * @param menus the array menus of the MenuBar
     * @param n the number of menus in the array
     */
    MenuBar(
        const std::map<std::string, std::vector<MenuBar::MenuItem>>& menu_items
    ) : menus(), menu_items(menu_items) {
        for(const auto& pair : menu_items)
            menus.push_back(pair.first.c_str());
    };

};

}