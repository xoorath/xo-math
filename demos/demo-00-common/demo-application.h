#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <chrono>
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
    virtual void Tick(float deltaTime);
    virtual void GUI();
    virtual void GUIPanel() { };
    void SetMVP(float*) const;
    void RenderCube();
    int const m_Width = 1224;
    int const m_Height = 720;
private:
    typedef std::chrono::high_resolution_clock app_clock;
    SDL_GLContext m_Context = SDL_GLContext();
    SDL_Window* m_Window = nullptr;
    uint8_t const* m_Keys = nullptr;

    xo::Vector4 m_ClearColor = { 0.392f, 0.584f, 0.929f, 1.f };
    ImGuiTextBuffer m_LogBuffer;
    GLuint m_ShaderProgram = GL_INVALID_INDEX;
    GLuint m_UniformMVP = GL_INVALID_INDEX;
    GLuint m_VAO = GL_INVALID_INDEX;
    GLuint m_IBO = GL_INVALID_INDEX;
    bool m_Quitting = false;
    bool m_ShowDebugPanel = true;
    bool m_ShowLogPanel = false;
    bool m_ShowPerfPanel = false;
    bool m_ScrollLogPanel = false;
    app_clock::time_point m_LastFrameTime;
    size_t const m_MaxTrackedFPS = 200;
    size_t m_FrameIdx = 0;
    float m_TrackedFPS[200] = { 0 };
    float m_AverageFPS[200] = { 0 };
    void SetAttribs();
    bool BuildShaderInternal(GLuint kind, char const* source);
    void SetupDemoShader();
    void BuildDemoMeshes();
};