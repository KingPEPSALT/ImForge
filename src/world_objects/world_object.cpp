#include "world_objects/world_object.hpp"

namespace ImForge::World {
WorldObject::WorldObject(
    const std::string& name, 
    const std::vector<std::string>& art_files
    ): name(name), art_files(art_files) {};
}