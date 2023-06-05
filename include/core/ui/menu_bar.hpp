#pragma once

#include <imgui.h>
#include <vector>
#include <map>

namespace ImForge::Core::UI {

struct MenuBar {
    struct MenuItem {
        const char* name; 
        const char* shortcut;
        bool selected;
        bool enabled;
    };

    std::vector<const char*> options;
    std::map<const char*, std::vector<MenuItem>> suboptions;

    bool attach();
    void addMenu(const char* label);
    void addItem(
        const char* menu,
        const char* label, 
        const char* shortcut = NULL, 
        bool selected = false, 
        bool enabled = true
    );

    MenuItem removeItem(const char* menu, std::vector<MenuItem>::iterator iterator);
    MenuItem removeItem(const char* menu, const char* label);
    MenuItem removeItem(const char* menu, size_t index);
    const char* removeMenu(const char* menu);

    MenuBar();
    ~MenuBar();
};

}