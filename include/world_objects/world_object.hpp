#pragma once

#include <string>
#include <vector>

namespace ImForge::World {
struct WorldObject {
    std::string name;
    std::vector<std::string> art_files;
    
    WorldObject(const std::string& name, const std::vector<std::string>& art_files);
};
}