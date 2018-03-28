#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <xo-math.h>
#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>

class Application {
public:
    Application() = default;
    ~Application() = default;
    Application(Application const&) = delete;
    Application(Application &&) = delete;
    Application& operator = (Application const&) = delete;
    Application& operator = (Application &&) = delete;

    int run();

protected:
    void Log(char const* fmt, ...);
    virtual void Init();
    virtual void Tick();
    virtual void GUI();
    virtual void GUIPanel() { };
    int const m_Width = 1224;
    int const m_Height = 720;
private:
    SDL_GLContext m_Context = SDL_GLContext();
    SDL_Window* m_Window = nullptr;
    uint8_t const* m_Keys = nullptr;
    xo::Vector4 m_ClearColor = { 0.45f, 0.55f, 0.60f, 1.f };
    ImGuiTextBuffer m_LogBuffer;
    bool m_Quitting = false;
    bool m_ShowDebugPanel = true;
    bool m_ShowLogPanel = true;
    bool m_ScrollLogPanel = false;

    void SetAttribs();
};