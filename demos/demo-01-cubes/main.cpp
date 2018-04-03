// warning C4530: C++ exception handler used, but unwind semantics are not enabled.
// warning C4577: 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed.
#pragma warning(disable : 4530 4577)
#include "../demo-00-common/demo-application.h"
#include <limits>
#include <functional>
using namespace xo;

#define LimitedLog(fmt, ...) { static int limiter = 16; if(limiter > 0) { Log(fmt __VA_ARGS__); limiter--;} }

class CubesApplication : public Application {
    AMatrix4x4 m_Proj = AMatrix4x4::Identity;
    AMatrix4x4 m_View = AMatrix4x4::Identity;
    AMatrix4x4 m_Model = AMatrix4x4::Identity;
    AMatrix4x4 m_MatrixMVP = AMatrix4x4::Identity;
    bool m_MVPDirty = false;

    AVector3 m_CamPosition = { 0.f, 0.f, 0.f };
    AVector3 m_CamTarget = AVector3::Zero;
    AVector3 m_CamUp = AVector3::Up;

    AVector3 m_CubePosition = { 3.f, 0.f, 0.f };
    AVector3 m_CubeScale = AVector3(0.5f);
    AVector3 m_CubeRotation = AVector3::Zero;

    float m_CameraFOV = 45.f;

    float m_CubeRotationSpeed = 1.f;
    bool m_SpinCube = false;
public:
    CubesApplication()
    : Application()
    , m_CameraFOV(45.f) {

    }

protected:
    void ResetScene() {
        m_CamPosition = { 0.f, 0.f, 0.f };
        m_CamTarget = AVector3::Zero;
        m_CamUp = AVector3::Up;

        m_CubePosition = { 3.f, 0.f, 0.f };
        m_CubeScale = AVector3::One * AVector3(0.5f);
        m_CubeRotation = AVector3::Zero;

        m_CameraFOV = 45.f;

        m_SpinCube = false;
        m_CubeRotationSpeed = 1.f;
    }

    virtual void Init() override {
        Application::Init();
        ResetScene();
    }

    virtual void Tick(float deltaTime) override {
        Application::Tick(deltaTime);

        constexpr float tau = 6.28318530718;
        AVector3 r = m_CubeRotation;
        r.x = WrapMinMax(r.x, 0.f, tau);
        r.y = WrapMinMax(r.y, 0.f, tau);
        r.z = WrapMinMax(r.z, 0.f, tau);
        m_CubeRotation = r;

        if (m_SpinCube) {
            m_CubeRotation += AVector3(deltaTime * m_CubeRotationSpeed);
        }

        m_CamTarget = m_CubePosition;

        m_Proj = AMatrix4x4::PerspectiveFOV(m_CameraFOV * Deg2Rad, (float)m_Width / (float)m_Height);
        m_View = AMatrix4x4::LookAt(m_CamPosition, m_CamTarget, m_CamUp);
        AMatrix4x4::Invert(m_View);

        m_Model = AMatrix4x4::Scale(m_CubeScale);
        m_Model *= AQuaternion::RotationEuler(m_CubeRotation).ToMatrix();
        m_Model *= AMatrix4x4::Translation(m_CubePosition);

        m_MatrixMVP = m_Model * (m_View *  m_Proj);
        SetMVP(m_MatrixMVP.v);
        RenderCube();
    }

    virtual void GUI() override {
        Application::GUI();
    }

    virtual void GUIPanel() override {
        Application::GUIPanel();
        
        // proj
        ImGui::Separator();
        ImGui::DragFloat("Cam FOV", &m_CameraFOV, 0.05f);

        // model
        ImGui::Separator();
        ImGui::Separator();
        ImGui::DragFloat3("Cube Pos",   (float*)&m_CubePosition,    0.05f);

        AVector3 rot = m_CubeRotation * AVector3(Rad2Deg);
        ImGui::DragFloat3("Cube Rot",   (float*)&rot,               0.05f);
        m_CubeRotation = rot * AVector3(Deg2Rad);

        ImGui::DragFloat3("Cube Scale", (float*)&m_CubeScale,       0.05f);

        ImGui::Checkbox("Spin Cube",    &m_SpinCube);
        ImGui::DragFloat("Spin Speed",  &m_CubeRotationSpeed,       0.001f);

        // application
        ImGui::Separator();
        if (ImGui::Button("Reset")) {
            ResetScene();
        }
    }
};

extern C_LINKAGE SDLMAIN_DECLSPEC int SDL_main(int argc, char *argv[]) {
    CubesApplication app;
    return app.run();
}