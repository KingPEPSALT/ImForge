# ImForge
ImForge is a bloat-free world-building tool with rich-text support and object referencing. This project is currently a WIP with little functionality.

---
## External Libraries
These can be found in [__external__](./external/). If they are linked via other methods, there will be a note of it here.

Library|Description                          
---|---
[__**ImGui**__](https://github.com/ocornut/imgui)   | Immediate GUI library     
[__**SDL**__](https://github.com/libsdl-org/sdl)    | Simple Directmedia Layer  
[__**spdlog**__](https://github.com/gabime/spdlog)  | Fast C++ logging library

---
## Contributors
If you are interested in contributing, check out [__docs/CONTRIBUTING.md__](./docs/CONTRIBUTING.md). For a quick start:
```bash
git clone --recursive https://github.com/KingPEPSALT/ImForge.git
cd ImForge
cmake -S . -B _build 
cmake --build _build --config debug
```
Then, run the executable found in ___build/Debug/__.

---
## License
This project is licensed under [<u>AGPLv3.0</u>](https://www.gnu.org/licenses/agpl-3.0.en.html); you can find this project's license [__here__](./docs/LICENSE.txt). All external code used is licensed in the corresponding folders.