#include "../demo-00-common/demo-application.h"

using namespace xo;

#define LimitedLog(fmt, ...) { static int limiter = 100; if(limiter > 0) { Log(fmt __VA_ARGS__); limiter--;} }

class CubesApplication : public Application {
    AMatrix4x4 m_Proj = AMatrix4x4::Identity;
    AMatrix4x4 m_View = AMatrix4x4::Identity;
public:
    CubesApplication()
    : Application()
    , m_Proj(xo::AMatrix4x4::Perspective(static_cast<float>(m_Width),
                                         static_cast<float>(m_Height),
                                         60.0_deg2rad))
    , m_View(AMatrix4x4::LookAt({ 0.f, 0.f, -10.f }, AVector3::Zero))
    {
    }

    ~CubesApplication() {
        if (m_VAO != GL_INVALID_INDEX) {
            glDeleteBuffers(1, &m_VAO);
        }
    }

protected:
    void BuildCube() {
        /*
               _________
              /1      3/|
             /        / |
            /0______2/  |
            |   5----|-7/
            |  /     | /
            |4______6|/
        */
        Vertex Verts[8] = {
            { { -0.5f,  0.5f, -0.5f },{ 0.f, 1.f, 0.f },{ 0.f, 0.f } },
            { { -0.5f,  0.5f,  0.5f },{ 0.f, 1.f, 0.f },{ 0.f, 1.f } },
            { { 0.5f,  0.5f, -0.5f },{ 0.f, 1.f, 0.f },{ 1.f, 0.f } },
            { { 0.5f,  0.5f,  0.5f },{ 0.f, 1.f, 0.f },{ 1.f, 1.f } },
            { { -0.5f, -0.5f, -0.5f },{ 0.f, 1.f, 0.f },{ 0.f, 0.f } },
            { { -0.5f, -0.5f,  0.5f },{ 0.f, 1.f, 0.f },{ 0.f, 1.f } },
            { { 0.5f, -0.5f, -0.5f },{ 0.f, 1.f, 0.f },{ 1.f, 0.f } },
            { { 0.5f, -0.5f,  0.5f },{ 0.f, 1.f, 0.f },{ 1.f, 1.f } },
        };
        GLuint Indices[36] = {
            // top
            0, 1, 2,
            2, 1, 3,
            //front
            4, 0, 6,
            6, 0, 2,
            // bottom
            6, 4, 5,
            5, 7, 6,
            // right
            6, 2, 7,
            7, 2, 3,
            // left
            4, 5, 0,
            5, 1, 0
        };
        // referencing: https://github.com/Polytonic/Glitter/blob/master/Samples/mesh.cpp
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        GLuint tempBuffers[2];
        glGenBuffers(2, tempBuffers);
        glBindBuffer(GL_ARRAY_BUFFER, tempBuffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Verts), Verts, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempBuffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, UV));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        glDeleteBuffers(2, tempBuffers);
    }

    void BuildShader() {
    }

    virtual void Init() override {
        Application::Init();
        BuildCube();
    }

    virtual void Tick() override {
        Application::Tick();
        if (m_ShaderProgram != GL_INVALID_INDEX && m_VAO != GL_INVALID_INDEX) {
            glUseProgram(m_ShaderProgram);
            glBindVertexArray(m_VAO);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            LimitedLog("Draw");
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

    }

private:
    struct Vertex {
        Vector3 Position;
        Vector3 Normal;
        float UV[2];
    };

    GLuint m_VAO = GL_INVALID_INDEX; // vertex array object
    GLuint m_ShaderProgram = GL_INVALID_INDEX;
};

extern C_LINKAGE SDLMAIN_DECLSPEC int SDL_main(int argc, char *argv[]) {
    CubesApplication app;
    return app.run();
}