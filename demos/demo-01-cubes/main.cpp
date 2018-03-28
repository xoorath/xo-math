#include "../demo-00-common/demo-application.h"
#include <limits>
using namespace xo;

#define LimitedLog(fmt, ...) { static int limiter = 16; if(limiter > 0) { Log(fmt __VA_ARGS__); limiter--;} }

constexpr size_t VertCount = 24;
constexpr size_t IndexCount = 36;

class CubesApplication : public Application {
    AMatrix4x4 m_Proj = AMatrix4x4::Identity;
    AMatrix4x4 m_View = AMatrix4x4::Identity;

    AVector3 m_CamPosition = { 0.f, 0.f, 3.f};
    AVector3 m_CamTarget = AVector3::Zero;
    AVector3 m_CamUp = AVector3::Up;

    AVector3 m_CubePosition = {0.f, 0.f, 0.f};
    AVector3 m_CubeScale = AVector3::One * AVector3(0.5f);
    AVector3 m_CubeRotation = AVector3::Zero;

    float m_CameraFOV = 45.f;

public:
    CubesApplication()
    : Application()
    {
    }

    ~CubesApplication() {
        if (m_VAO != GL_INVALID_INDEX) {
            glDeleteBuffers(1, &m_VAO);
        }
        if (m_IBO != GL_INVALID_INDEX) {
            glDeleteBuffers(1, &m_IBO);
        }
        if (m_ShaderProgram != GL_INVALID_INDEX) {
            glDeleteProgram(m_ShaderProgram);
        }
    }

protected:
    void ResetScene() {
        m_CamPosition = { 0.f, 0.f, 3.f };
        m_CamTarget = AVector3::Zero;
        m_CamUp = AVector3::Up;

        m_CubePosition = { 0.f, 0.f, 0.f };
        m_CubeScale = AVector3::One * AVector3(0.5f);
        m_CubeRotation = AVector3::Zero;

        m_CameraFOV = 45.f;
    }

    void BuildCube() {
        /*    TOP / BOTTOM           LEFT / RIGHT        FRONT / BACK
               _________           9       13          21________22 
             1/        /3         /|       /|          |        |   
             /        /          / |      / |          |        |   
           0/________/2        8/  |   12/  |15   17___|___18___|23 
               5/-------/7      |  /11   |  /      |   20  |        
               /       /        | /      | /       |       |        
             4/_______/6        |/10     |/14    16|_______|19      
        */

        constexpr float uvMin = 0.f;
        constexpr float uvMax = 1.f;
        Vertex Verts[VertCount] = {
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


        // abc = clockwise
        // cba = counter clockwise
#define TRIANGLE_WINDING(a, b, c) a, b, c

        GLuint Indices[IndexCount] = {
            // top
            TRIANGLE_WINDING(0, 1, 2),
            TRIANGLE_WINDING(1, 3, 2),
            // bottom
            TRIANGLE_WINDING(4, 6, 5),
            TRIANGLE_WINDING(5, 6, 7),

            //front
            TRIANGLE_WINDING(16, 17, 18),
            TRIANGLE_WINDING(18, 19, 16),
            // back
            TRIANGLE_WINDING(20, 23, 21),
            TRIANGLE_WINDING(22, 21, 23),

            // left
            TRIANGLE_WINDING(8, 10, 11),
            TRIANGLE_WINDING(11, 9, 8),
            // right
            TRIANGLE_WINDING(12, 13, 15),
            TRIANGLE_WINDING(15, 14, 12)
        };
#undef TRIANGLE_WINDING
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

    bool BuildShaderInternal(GLuint kind, char const* source) {
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

    void BuildShader() {
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
            GLSL("uniform vec3 LightDirection;")
            GLSL("uniform vec4 Tint;")
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
    }


    virtual void Init() override {
        Application::Init();
        ResetScene();
        BuildCube();
        m_ShaderProgram = glCreateProgram();
        BuildShader();
        glLinkProgram(m_ShaderProgram);
        m_UniformMVP = glGetUniformLocation(m_ShaderProgram, "MVP");

    }

    virtual void Tick() override {
        Application::Tick();

        // perform transformations
        AMatrix4x4 model = AMatrix4x4::Identity;
        model *= AMatrix4x4::Scale(m_CubeScale);
        model *= AQuaternion::RotationEuler(m_CubeRotation).ToMatrix();
        model *= AMatrix4x4::Translation(m_CubePosition);

        m_View = AMatrix4x4::LookAt(m_CamPosition, m_CamTarget, m_CamUp);
        AMatrix4x4::Invert(m_View);

        m_Proj = AMatrix4x4::PerspectiveFOV(m_CameraFOV * Deg2Rad, (float)m_Width / (float)m_Height);

        m_MatrixMVP = model * (m_View *  m_Proj);

        if (m_ShaderProgram != GL_INVALID_INDEX && m_VAO != GL_INVALID_INDEX) {
            glUseProgram(m_ShaderProgram);
            // setup shader
            if (m_UniformMVP != GL_INVALID_INDEX) {
                glUniformMatrix4fv(m_UniformMVP, 1, GL_FALSE, m_MatrixMVP.v);
            }
            else {
                LimitedLog("No MP uniform.");
            }

            // draw mesh
            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
            glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);
        }
        else {
            if (m_ShaderProgram == GL_INVALID_INDEX) {
                LimitedLog("Shader program is invalid.");
            }
            if (m_VAO != GL_INVALID_INDEX) {
                LimitedLog("Vertex array object is invalid.");
            }
        }
    }

    virtual void GUI() override {
        Application::GUI();

    }

    virtual void GUIPanel() override {
        Application::GUIPanel();
        ImGui::Separator();
        ImGui::DragFloat("Cam FOV", &m_CameraFOV);
        
        ImGui::Separator();
        ImGui::DragFloat3("Cam Pos", (float*)&m_CamPosition, 0.05f);
        ImGui::DragFloat3("Cam Target", (float*)&m_CamTarget, 0.05f);
        ImGui::DragFloat3("Cam Up", (float*)&m_CamUp, 0.05f, -1.f, 1.f);
        ImGui::Separator();
        ImGui::DragFloat3("Cube Pos", (float*)&m_CubePosition, 0.05f);
        AVector3 rotDeg = m_CubeRotation * AVector3(Rad2Deg);
        ImGui::DragFloat3("Cube Rot", (float*)&rotDeg, 0.05f);
        m_CubeRotation = rotDeg * AVector3(Deg2Rad);
        ImGui::DragFloat3("Cube Scale", (float*)&m_CubeScale, 0.05f);
        if (ImGui::Button("Reset")) {
            ResetScene();
        }
    }

private:
    struct Vertex {
        Vector3 Position;
        Vector3 Normal;
        float UV[2];
    };

    AMatrix4x4 m_MatrixMVP;

    GLuint m_VAO = GL_INVALID_INDEX; // vertex array object
    GLuint m_IBO = GL_INVALID_INDEX;
    GLuint m_ShaderProgram = GL_INVALID_INDEX;
    GLuint m_UniformMVP = GL_INVALID_INDEX;
};

extern C_LINKAGE SDLMAIN_DECLSPEC int SDL_main(int argc, char *argv[]) {
    CubesApplication app;
    return app.run();
}