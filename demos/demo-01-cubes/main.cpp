#include <windows.h>
#define XO_MATH_IMPL
#include <xo-math.h>
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>
#include <iostream>

void TickDemo() {
    static struct DemoData {
        DemoData() {
            m_Projection = xo::Matrix4x4::Identity;
        }
        xo::Matrix4x4 m_Projection;
    } demo;
}

struct Application {
    /*
    IMGUI_API bool        ImGui_ImplSdlGL3_Init(SDL_Window* window, const char* glsl_version = NULL);
IMGUI_API void        ImGui_ImplSdlGL3_Shutdown();
IMGUI_API void        ImGui_ImplSdlGL3_NewFrame(SDL_Window* window);
IMGUI_API void        ImGui_ImplSdlGL3_RenderDrawData(ImDrawData* draw_data);
IMGUI_API bool        ImGui_ImplSdlGL3_ProcessEvent(SDL_Event* event);
    */
    int run() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "could not initialize sdl2:" << SDL_GetError() << std::endl;
            return 1;
        }
        m_Window = SDL_CreateWindow("demo-01-cubes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, SDL_WINDOW_OPENGL);
        if (m_Window == NULL) {
            std::cerr << "could not create window:" << SDL_GetError() << std::endl;
            return 1;
        }

        if (!ImGui_ImplSdlGL3_Init(m_Window)) {
            std::cerr << "could not init imgui:" << SDL_GetError() << std::endl;
            return 1;
        }

        m_Context = SDL_GL_CreateContext(m_Window);
        SetAttribs();
        SDL_GL_SetSwapInterval(1);

#if !defined(__APPLE__)
        glewExperimental = GL_TRUE;
        glewInit();
#endif

        glClearColor(0.0, 0.0, 0.0, 1.0);
        SDL_GL_SwapWindow(m_Window);

        SDL_Event frameEvent;
        
        SDL_PumpEvents();
        m_Keys = static_cast<uint8_t const*>(SDL_GetKeyboardState(nullptr));

        while (!m_Quitting) {
            glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
            glClear(GL_COLOR_BUFFER_BIT);
            Tick();
            ImGui::Render();
            ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(m_Window);
            // process all pending events
            while (SDL_PollEvent(&frameEvent)) {
                switch (frameEvent.type)
                {
                case SDL_QUIT:
                    m_Quitting = true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    m_Keys = static_cast<uint8_t const*>(SDL_GetKeyboardState(nullptr));
                    break;
                }
                ImGui_ImplSdlGL3_ProcessEvent(&frameEvent);
            }
        }
        return 0;
    }
    
    Application() = default;
    ~Application() {
        if(m_Window) SDL_DestroyWindow(m_Window);
        SDL_Quit();
        ImGui_ImplSdlGL3_Shutdown();
    }
    Application(Application const&) = delete;
    Application(Application &&) = delete;
    Application& operator = (Application const&) = delete;
    Application& operator = (Application &&) = delete;

private:
    SDL_GLContext m_Context = SDL_GLContext();
    SDL_Window* m_Window    = nullptr;
    int m_Width             = 1224;
    int m_Height            = 720;
    bool m_Quitting         = false;
    uint8_t const*m_Keys    = nullptr;

    void Tick() {
        if (m_Keys[SDL_SCANCODE_ESCAPE]) {
            m_Quitting = true;
            return;
        }
        TickDemo();
    }

    void SetAttribs() {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    }

};

extern C_LINKAGE SDLMAIN_DECLSPEC int SDL_main(int argc, char *argv[]) {
    Application app;
    return app.run();
}