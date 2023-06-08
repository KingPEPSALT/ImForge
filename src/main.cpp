/* 
    this is a modified version of ImGui's SDL3 
    + SDLRenderer3 example that can be found in 
    this project's submodules:
        external/imgui/examples/example_sdl3_sdlrenderer3/main.cpp
    or at:
        https://github.com/ocornut/imgui/blob/master/examples/example_sdl3_sdlrenderer3/main.cpp
*/
#include <stdio.h>

#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>

#include <SDL3/SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif
#include <spdlog/spdlog.h>

#include "core/ui.hpp"

int main(int, char**)
{
    // setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        spdlog::critical(
            "SDL_INIT(Uint32 flags) failed on SDL_INIT_VIDEO | SDL_INIT_TIMER:\n"
            "\tSDL_ERROR: {:s}\n", SDL_GetError()
        );
        return -1;
    }

    // Enable native IME.
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

    // Create window with SDL_Renderer graphics context
    SDL_Window* window = SDL_CreateWindow(
        "ImForge [TEST]", 1280, 720,
         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
    );
    if (!window) {
        spdlog::critical(
            "SDL_CreateWindow(const char* title, int w, int h, Uint32 flags) failed:\n"
            "\tSDL_ERROR: {:s}\n", SDL_GetError()
        );
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, NULL, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
    );
    if (!renderer) {
        spdlog::critical(
            "SDL_CreateRenderer(SDL_Window* window, const char* name, Uint32 flags) failed:\n"
            "\tSDL_ERROR: {:s}\n", SDL_GetError()
        );
        return -1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // setup ImGui 
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    // setup renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_colour = ImVec4(.2f, .7f, 1.f, 1.f);

    // MenuBar stress test
    ImForge::Core::UI::MenuBar menubar({"01", "02", "0x03!"});
    menubar.addItems("01", {
        "this",
        "is",
        "a",
        "menu"
    });
    menubar.addItems("02", {
        "another"
    });
    menubar.addItems("0x03!", {
        "hello",
        "click me to delete me...",
        "or don't!"
    });
    
    menubar.removeMenu("02");

    // toggle variable to ensure we only run the line 116 if-statement once 
    bool item_created = false; 
    bool item_deleted = false;
    bool done = false;

    menubar.menu_items["0x03!"].at(0).on_first_select_callback = 
        [](auto parent) {
            parent->menu_items["0x03!"].at(0).name = "goodbye";
            parent->menu_items["0x03!"].at(0).enabled = false;
        };

    menubar.menu_items["0x03!"].at(1).on_first_select_callback = 
        [](auto parent) {
            parent->addItem("0x03!", "I lied.");
        };

    menubar.menu_items["0x03!"].at(2).on_first_select_callback = 
        [](auto parent) {
            parent->removeItem("0x03!", "or don't");
        };
    
    while (!done)
    {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            done = (event.type == SDL_EVENT_QUIT)
                || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED 
                && event.window.windowID == SDL_GetWindowID(window));
        }
        menubar.pollEvents();

        // start ImGui frame
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        {
            if(!ImGui::Begin("Background picker", nullptr, ImGuiWindowFlags_MenuBar)){
                ImGui::End();
            }
            menubar.attach();

            ImGui::Text("Choose a background colour");
            ImGui::ColorEdit3("clear color", (float*)&clear_colour); 

            ImGui::Text("FPS: %.1f (%.3fms)", io.Framerate,  1000.f / io.Framerate);
            ImGui::End();
        }

        // rendering
        ImGui::Render();
        SDL_SetRenderDrawColor(
            renderer, 
            (uint8_t)(clear_colour.x*255), 
            (uint8_t)(clear_colour.y*255), 
            (uint8_t)(clear_colour.z*255), 
            (uint8_t)(clear_colour.w*255)
        );
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);

    }

    // cleanup
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
