#define XO_MATH_IMPL
#include <xo-math.h>
#include <iostream>

#include "demo-application.h"

int Application::run() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "could not initialize sdl2:" << SDL_GetError() << std::endl;
        return 1;
    }
    m_Window = SDL_CreateWindow("demo-01-cubes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, SDL_WINDOW_OPENGL);
    if (m_Window == NULL) {
        std::cerr << "could not create window:" << SDL_GetError() << std::endl;
        return 1;
    }

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
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
    SDL_GL_SwapWindow(m_Window);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    Init();
    while (!m_Quitting) {

        //glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplSdlGL3_NewFrame(m_Window);

        Tick();
        GUI();

        ImGui::Render();
        ImGui_ImplSdlGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(m_Window);
        // process all pending events
        SDL_Event frameEvent;
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
    ImGui_ImplSdlGL3_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
    return 0;
}

void Application::Log(char const* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    m_LogBuffer.appendfv(fmt, args);
    m_LogBuffer.appendf("\n");
    va_end(args);
    m_ScrollLogPanel = true;
}

/*virtual*/
void Application::Init() {
    SDL_PumpEvents();
    m_Keys = static_cast<uint8_t const*>(SDL_GetKeyboardState(nullptr));
}

/*virtual*/
void Application::Tick() {
    if (m_Keys[SDL_SCANCODE_ESCAPE]) {
        m_Quitting = true;
        return;
    }
}

/*virtual*/
void Application::GUI() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Toggle Debug Panel")) {
                m_ShowDebugPanel = !m_ShowDebugPanel;
            }
            if (ImGui::MenuItem("Toggle Log Panel")) {
                m_ShowLogPanel = !m_ShowLogPanel;
                m_ScrollLogPanel = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit")) {
                m_Quitting = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    if (m_ShowDebugPanel) {
        ImGui::Begin("Debug", &m_ShowDebugPanel);
        ImGui::ColorEdit3("Clear Color", (float*)&m_ClearColor);
        GUIPanel();
        ImGui::End();
    }

    if (m_ShowLogPanel) {
        ImGui::Begin("Log", &m_ShowLogPanel);
        if (ImGui::Button("Clear")) {
            m_LogBuffer.clear();
        }
        ImGui::Separator();
        ImGui::BeginChild("scroll area");
        ImGui::TextUnformatted(m_LogBuffer.begin());
        if (m_ScrollLogPanel) {
            ImGui::SetScrollHere(1.f);
            m_ScrollLogPanel = false;
        }
        ImGui::EndChild();
        ImGui::End();
    }
}

void Application::SetAttribs() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}