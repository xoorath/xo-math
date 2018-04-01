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
    SDL_GL_SetSwapInterval(0);

#if !defined(__APPLE__)
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    SDL_GL_SwapWindow(m_Window);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    SetupDemoShader();
    BuildDemoMeshes();
    Init();
    
    glUseProgram(m_ShaderProgram);
    m_LastFrameTime = app_clock::now();
    while (!m_Quitting) {
        glClearColor(m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplSdlGL3_NewFrame(m_Window);
        app_clock::time_point frameTime = app_clock::now();
        std::chrono::duration<float> span = std::chrono::duration_cast<std::chrono::duration<float>>(frameTime - m_LastFrameTime);
        m_LastFrameTime = frameTime;
        Tick(span.count());
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

    if (m_ShaderProgram != GL_INVALID_INDEX) {
        glDeleteProgram(m_ShaderProgram);
    }
    if (m_VAO != GL_INVALID_INDEX) {
        glDeleteBuffers(1, &m_VAO);
    }
    if (m_IBO != GL_INVALID_INDEX) {
        glDeleteBuffers(1, &m_IBO);
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
void Application::Tick(float deltaTime) {
    if (m_Keys[SDL_SCANCODE_ESCAPE]) {
        m_Quitting = true;
        return;
    }
    ++m_FrameIdx;
    size_t trackIdx = m_FrameIdx % m_MaxTrackedFPS;
    m_TrackedFPS[trackIdx] = deltaTime;

    size_t avgCount = xo::Min(m_FrameIdx, m_MaxTrackedFPS);
    float accum = 0.f;
    for (size_t i = 0; i < avgCount; ++i) {
        accum += m_TrackedFPS[i];
    }
    m_AverageFPS[trackIdx] = accum / (float)avgCount;
}

/*virtual*/
void Application::GUI() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Debug Panel")) {
                m_ShowDebugPanel = !m_ShowDebugPanel;
            }
            if (ImGui::MenuItem("Log Panel")) {
                m_ShowLogPanel = !m_ShowLogPanel;
                m_ScrollLogPanel = true;
            }
            if (ImGui::MenuItem("Show Performance")) {
                m_ShowPerfPanel = !m_ShowPerfPanel;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Quit")) {
                m_Quitting = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    
    if (m_ShowPerfPanel) {
        ImGui::Begin("Performance", &m_ShowPerfPanel);

        float relativeMin = FLT_MAX, relativeMax = FLT_MIN;
        size_t thisFrameIdx = m_FrameIdx % m_MaxTrackedFPS;
        size_t framesToCount = xo::Min(m_FrameIdx, m_MaxTrackedFPS);
        for (size_t i = 0; i < framesToCount; ++i) {
            if (relativeMin > m_AverageFPS[i]) relativeMin = m_AverageFPS[i];
            if (relativeMax < m_AverageFPS[i]) relativeMax = m_AverageFPS[i];
            if (relativeMin > m_TrackedFPS[i]) relativeMin = m_TrackedFPS[i];
            if (relativeMax < m_TrackedFPS[i]) relativeMax = m_TrackedFPS[i];
        }
        
        float thisFrame = m_AverageFPS[thisFrameIdx];
        {
            char text[64];
            sprintf_s(text, 64, "Avg FPS: %f", 1.f / (thisFrame));
            ImGui::Text(text);
        }
        {
            char text[64];
            sprintf_s(text, 64, "avg frame time (mspf) %f", thisFrame);
            ImGui::PlotHistogram(text, m_AverageFPS, m_MaxTrackedFPS, thisFrameIdx + 1, NULL, relativeMin, relativeMax, ImVec2(300, 100));
        }
        thisFrame = m_TrackedFPS[thisFrameIdx];
        {
            char text[64];
            sprintf_s(text, 64, "FPS: %f", 1.f / (thisFrame));
            ImGui::Text(text);
        }
        {
            char text[64];
            sprintf_s(text, 64, "frame time (mspf) %f", thisFrame);
            ImGui::PlotHistogram(text, m_TrackedFPS, m_MaxTrackedFPS, thisFrameIdx + 1, NULL, relativeMin, relativeMax, ImVec2(300, 100));
        }
        ImGui::End();
        
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

void Application::SetMVP(float* v) const {
    glUniformMatrix4fv(m_UniformMVP, 1, GL_FALSE, v);
}

void Application::RenderCube() {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Application::SetAttribs() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

bool Application::BuildShaderInternal(GLuint kind, char const* source) {
    GLuint shader = glCreateShader(kind);
    GLint status;
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint logLen;
        Log("Couldn't build shader");
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        GLchar* logBuff = new GLchar[logLen];
        glGetShaderInfoLog(shader, logLen, nullptr, logBuff);
        Log(logBuff);
        delete[] logBuff;
        return false;
    }
    glAttachShader(m_ShaderProgram, shader);
    glDeleteShader(shader);
    return true;
}

void Application::SetupDemoShader() {
    m_ShaderProgram = glCreateProgram();
    
#define GLSL(code) code "\n"
    const char* vertSource =
        GLSL("#version 440 core")
        GLSL("layout(location = 0) in vec3 vertPosition;")
        GLSL("layout(location = 1) in vec3 vertNormal;")
        GLSL("layout(location = 2) in vec2 vertUV;")
        GLSL("")
        GLSL("out vec3 Normal;")
        GLSL("out vec2 UV;")
        GLSL("")
        GLSL("uniform mat4 MVP;")
        GLSL("")
        GLSL("void main(){")
        GLSL("	gl_Position =  MVP * vec4(vertPosition, 1.0);")
        GLSL("	Normal = vertNormal;")
        GLSL("	UV = vertUV;")
        GLSL("}");

    const char* fragSource =
        GLSL("#version 440 core")
        GLSL("")
        GLSL("in vec3 Normal;")
        GLSL("in vec2 UV;")
        GLSL("")
        GLSL("out vec4 color;")
        GLSL("")
        GLSL("void main() {")
        GLSL("	color = vec4(((Normal.xyz + vec3(0.5, 0.5, 0.5)) * vec3(0.5, 0.5, 0.5)) * vec3(1.5, 1.5, 1.5), 1.0);")
        GLSL("}");
#undef GLSL

    if (!BuildShaderInternal(GL_VERTEX_SHADER, vertSource)) {
        return;
    }
    if (!BuildShaderInternal(GL_FRAGMENT_SHADER, fragSource)) {
        return;
    }

    glLinkProgram(m_ShaderProgram);
    m_UniformMVP = glGetUniformLocation(m_ShaderProgram, "MVP");
}

void Application::BuildDemoMeshes() {
    using namespace xo;
    struct Vertex {
        Vector3 Position;
        Vector3 Normal;
        float UV[2];
    };
    /*    TOP / BOTTOM           LEFT / RIGHT        FRONT / BACK
           _________           9       13          21________22 
         1/        /3         /|       /|          |        |   
         /        /          / |      / |          |        |   
       0/________/2        8/  |   12/  |15   17___|___18___|23 
           5/-------/7      |  /11   |  /      |   20  |        
           /       /        | /      | /       |       |        
         4/_______/6        |/10     |/14    16|_______|19      
    */

    // warning: no thought was put into the UV's yet.
    constexpr float uvMin = 0.f;
    constexpr float uvMax = 1.f;
    Vertex Verts[24] = {
        // top
        { { -0.5f,  0.5f, -0.5f }, Vector3::Up, { uvMin, uvMin } },
        { { -0.5f,  0.5f,  0.5f }, Vector3::Up, { uvMin, uvMax } },
        { {  0.5f,  0.5f, -0.5f }, Vector3::Up, { uvMax, uvMin } },
        { {  0.5f,  0.5f,  0.5f }, Vector3::Up, { uvMax, uvMax } },
        // bottom
        { { -0.5f, -0.5f, -0.5f }, Vector3::Down, { uvMin, uvMin } },
        { { -0.5f, -0.5f,  0.5f }, Vector3::Down, { uvMin, uvMax } },
        { {  0.5f, -0.5f, -0.5f }, Vector3::Down, { uvMax, uvMin } },
        { {  0.5f, -0.5f,  0.5f }, Vector3::Down, { uvMax, uvMax } },
        // left
        { { -0.5f,  0.5f, -0.5f }, Vector3::Left, { uvMin, uvMin } },
        { { -0.5f,  0.5f,  0.5f }, Vector3::Left, { uvMin, uvMax } },
        { { -0.5f, -0.5f, -0.5f }, Vector3::Left, { uvMax, uvMin } },
        { { -0.5f, -0.5f,  0.5f }, Vector3::Left, { uvMax, uvMax } },
        // right
        { { 0.5f,  0.5f, -0.5f }, Vector3::Right, { uvMin, uvMin } },
        { { 0.5f,  0.5f,  0.5f }, Vector3::Right, { uvMin, uvMax } },
        { { 0.5f, -0.5f, -0.5f }, Vector3::Right, { uvMax, uvMin } },
        { { 0.5f, -0.5f,  0.5f }, Vector3::Right, { uvMax, uvMax } },
        // front
        { { -0.5f, -0.5f, -0.5f }, Vector3::Forward, { uvMin, uvMin } },
        { { -0.5f,  0.5f, -0.5f }, Vector3::Forward, { uvMin, uvMax } },
        { {  0.5f,  0.5f, -0.5f }, Vector3::Forward, { uvMax, uvMin } },
        { {  0.5f, -0.5f, -0.5f }, Vector3::Forward, { uvMax, uvMax } },
        // back
        { { -0.5f, -0.5f,  0.5f }, Vector3::Backward,{ uvMin, uvMin } },
        { { -0.5f,  0.5f,  0.5f }, Vector3::Backward,{ uvMin, uvMax } },
        { {  0.5f,  0.5f,  0.5f }, Vector3::Backward,{ uvMax, uvMin } },
        { {  0.5f, -0.5f,  0.5f }, Vector3::Backward,{ uvMax, uvMax } },
    };

    GLuint Indices[36] = {
        // top
        0, 1, 2,
        1, 3, 2,
        // bottom
        4, 6, 5,
        5, 6, 7,

        //front
        16, 17, 18,
        18, 19, 16,
        // back
        20, 23, 21,
        22, 21, 23,

        // left
        8, 10, 11,
        11, 9, 8,
        // right
        12, 13, 15,
        15, 14, 12
    };
    // referencing: https://github.com/Polytonic/Glitter/blob/master/Samples/mesh.cpp
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    GLuint vbo;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &m_IBO);
        
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, UV));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);
}