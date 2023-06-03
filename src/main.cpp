#include <SDL3/SDL.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <iostream>

#define _ImForge_Print_SDL_Err(func, ... )              \
    if(func(__VA_ARGS__))                               \
        std::cout << "[ERR] " << #func                  \
            << " failed: " << SDL_GetError()            \
            << std::endl;                               \

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cout << "[ERR] SDL_Init failed:" << SDL_GetError() << std::endl;
        return -1;
    }

    /* OpenGL setup: using GL 4.6 and GLSL 460 */
    const char* GLSL_version = "#version 460";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    /* OpenGL attributes */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    /* window setup */
    SDL_Window* window = SDL_CreateWindow(
        "ImForge [INITIAL]", 1280, 720, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
    );
    if(!window){
        std::cout << "[ERR] SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    /* OpenGL context setup */
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if(!gl_context){
        std::cout << "[ERR] SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    /* connect window & OpenGL context & enable V-Sync */
    _ImForge_Print_SDL_Err(SDL_GL_MakeCurrent, window, gl_context);
    _ImForge_Print_SDL_Err(SDL_GL_SetSwapInterval, 1); 

    // ImGui context setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    // setup backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(GLSL_version);

    ImVec4 grey = ImVec4(.5f, .5f, .5f, 1.f);
    ImVec4 skyblue = ImVec4(.4f, .8f, .4f, 1.f);
    SDL_Rect fullscreen_rect = SDL_Rect{0,  0, (int)io.DisplaySize.x, (int)io.DisplaySize.y};
    SDL_FRect square = SDL_FRect{100.f, 100.f, 100.f, 100.f};

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer){
        std::cout << "[ERR] << SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    _ImForge_Print_SDL_Err(SDL_SetRenderDrawColor, renderer, skyblue.x, skyblue.y, skyblue.z, 1.f);
    _ImForge_Print_SDL_Err(SDL_SetRenderDrawBlendMode, renderer, SDL_BLENDMODE_BLEND);
    _ImForge_Print_SDL_Err(SDL_SetRenderViewport, renderer, &fullscreen_rect);
    
    _ImForge_Print_SDL_Err(SDL_SetRenderDrawColor, renderer, skyblue.x, skyblue.y, skyblue.z, 1.f);

    bool finished = false;
    while (!finished) {

        /* poll events & check for quit events */ 
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            finished = event.type == SDL_EVENT_QUIT 
                || (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST 
                && event.window.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED 
                && event.window.windowID == SDL_GetWindowID(window));
        }
 
        /* ImGui frame creation */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("[INITIAL]");
        ImGui::Text("If you are reading this, you are viewing the [INITIAL] commit!"); 
        ImGui::Text("(%.2f FPS)", io.Framerate);

        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* present render to window and clear screen */
        _ImForge_Print_SDL_Err(SDL_RenderPresent, renderer);
        _ImForge_Print_SDL_Err(SDL_RenderClear, renderer);

        SDL_GL_SwapWindow(window);
    }

    // clean shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

