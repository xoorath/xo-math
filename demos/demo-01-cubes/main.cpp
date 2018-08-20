// warning C4530: C++ exception handler used, but unwind semantics are not enabled.
// warning C4577: 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed.
#pragma warning(disable : 4530 4577)
#include "../demo-00-common/demo-application.h"
#include "../demo-00-common/demo-scene.h"
#include <limits>
#include <functional>

using namespace xo;

#define LimitedLog(fmt, ...) { static int limiter = 16; if(limiter > 0) { Log(fmt __VA_ARGS__); limiter--;} }
// both positive and negative numbers of cubes on each axis (-x to x)
constexpr int CUBES_XYZ[3] = {5, 5, 5};

class CubesApplication : public Application {
    AMatrix4x4 m_Proj = AMatrix4x4::Identity;
    AMatrix4x4 m_View = AMatrix4x4::Identity;
    AMatrix4x4 m_Model = AMatrix4x4::Identity;
    AMatrix4x4 m_Model2 = AMatrix4x4::Identity;
    AMatrix4x4 m_MatrixMVP = AMatrix4x4::Identity;
    bool m_MVPDirty = false;

    AVector3 m_CamPosition = { 0.f, 0.f, 0.f };
    AVector3 m_CamTarget = AVector3::Zero;
    AVector3 m_CamUp = AVector3::Up;

    Scene m_Scene;
    SceneObject m_Root;
    std::vector<SceneObject> m_Cubes;

    float m_CameraFOV = 45.f;

    float m_CubeRotationSpeed = 1.f;
    bool m_SpinCube = false;

    float m_AppTime = 0.f;
public:
    CubesApplication()
    : Application()
    , m_CameraFOV(45.f) 
    , m_Root(nullptr) {
        m_Root = m_Scene.NewSceneObject();
        for (int x = -CUBES_XYZ[0]; x < CUBES_XYZ[0]; ++x) {
            for (int y = -CUBES_XYZ[1]; y < CUBES_XYZ[1]; ++y) {
                for (int z = -CUBES_XYZ[2]; z < CUBES_XYZ[2]; ++z) {
                    auto so = m_Scene.NewSceneObject();
                    so.SetParent(m_Root);
                    m_Cubes.push_back(so);
                }
            }
        }
    }

protected:
    void ResetScene() {
        m_CamPosition = { 0.f, 0.f, -20.f };
        m_CamTarget = AVector3::Zero;
        m_CamUp = AVector3::Up;

        m_Root.SetLocalScale({ 1.f, 1.f, 1.f });
        m_Root.SetLocalTranslation({ 0.f, 0.f, 0.f });
        m_Root.SetLocalRotation({ 0.f, 0.f, 0.f });
        size_t i = 0;
        for (int x = -CUBES_XYZ[0]; x < CUBES_XYZ[0]; ++x) {
            for (int y = -CUBES_XYZ[1]; y < CUBES_XYZ[1]; ++y) {
                for (int z = -CUBES_XYZ[2]; z < CUBES_XYZ[2]; ++z) {
                    m_Cubes[i].SetLocalScale({ 0.5f, 0.5f, 0.5 });
                    m_Cubes[i].SetLocalTranslation({ (float)x, (float)y, (float)z });
                    m_Cubes[i].SetLocalRotation({ 0.f, 0.f, 0.f });
                    ++i;
                }
            }
        }

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

        m_AppTime += deltaTime;

        constexpr float tau = 6.28318530718;
        AVector3 r = m_Root.GetLocalRotation();
        r.x = WrapMinMax(r.x, 0.f, tau);
        r.y = WrapMinMax(r.y, 0.f, tau);
        r.z = WrapMinMax(r.z, 0.f, tau);
        m_Root.SetLocalRotation(r);

        r = m_Root.GetLocalRotation();
        if (m_SpinCube) {
            m_Root.SetLocalRotation(r += AVector3(deltaTime * m_CubeRotationSpeed));
        }

        size_t i = 0;
        for (int x = -CUBES_XYZ[0]; x < CUBES_XYZ[0]; ++x) {
            for (int y = -CUBES_XYZ[1]; y < CUBES_XYZ[1]; ++y) {
                for (int z = -CUBES_XYZ[2]; z < CUBES_XYZ[2]; ++z) {
                    m_Cubes[i].SetLocalRotation((AVector3((float)x, (float)y, (float)z ) * AVector3(m_AppTime*m_CubeRotationSpeed)));
                    ++i;
                }
            }
        }

        m_CamTarget = m_Root.GetLocalTranslation();

        m_Proj = AMatrix4x4::PerspectiveFOV(m_CameraFOV * Deg2Rad, (float)m_Width / (float)m_Height);
        m_View = AMatrix4x4::LookAt(m_CamPosition, m_CamTarget, m_CamUp);
        AMatrix4x4::Invert(m_View);

        m_Scene.Update();

        for (auto& so : m_Cubes) {
            m_MatrixMVP = so.GetModelNoUpdate() * (m_View *  m_Proj);
            SetMVP(m_MatrixMVP.v);
            RenderCube();
        }
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
        AVector3 pos = m_Root.GetLocalTranslation();
        ImGui::DragFloat3("Cube Pos",   (float*)&pos,    0.05f);
        m_Root.SetLocalTranslation(pos);

        AVector3 rot = m_Root.GetLocalRotation() * AVector3(Rad2Deg);
        ImGui::DragFloat3("Cube Rot",   (float*)&rot,               0.05f);
        m_Root.SetLocalRotation(rot * AVector3(Deg2Rad));

        AVector3 sca = m_Root.GetLocalScale();
        ImGui::DragFloat3("Cube Scale", (float*)&sca,       0.05f);
        m_Root.SetLocalScale(sca);

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