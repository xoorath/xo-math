#pragma once
#include <xo-math.h>
#include <vector>
#include <algorithm>

namespace xo {

struct Transformation {
    void SetParent(Transformation* transformation) {
        if (m_Parent != nullptr) {
            m_Parent->m_Children.erase(std::find(m_Parent->m_Children.begin(), m_Parent->m_Children.end(), this));
        }
        m_Parent = transformation;
        transformation->m_Children.push_back(this);
        SetParentDirty();
    }

#define ENCAPSULATE_TRANSFORM(namedType) \
    AVector3 const& GetLocal##namedType () { return m_Values.m_ ## namedType; } \
    void SetLocal##namedType (AVector3 const& v) { m_Values.m_ ## namedType = v; Set##namedType##Dirty(); } \
    AVector3 const& GetWorld##namedType () { if (namedType ## Dirty()) Update(); return m_Matricies.m_Model.Transform(m_Values.m_ ## namedType); } \
    void SetWorld##namedType (AVector3 const& v) { if (namedType ## Dirty()) Update(); m_Values.m_ ## namedType = m_Matricies.m_Model.InverseTransform(v); Set##namedType##Dirty(); }

    ENCAPSULATE_TRANSFORM(Scale);
    ENCAPSULATE_TRANSFORM(Translation);
    ENCAPSULATE_TRANSFORM(Rotation);

#undef ENCAPSULATE_TRANSFORM

    inline size_t CountDepth() {
        size_t count = 0;
        Transformation* p = m_Parent;
        while (p) {
            p = p->m_Parent;
            count++;
        }
        return count;
    }

private:
    enum {
        SCALE_DIRTY = 1,
        TRANSLATION_DIRTY = 2,
        ROTATION_DIRTY = 4,
        PARENT_DIRTY = 8
    };

    Transformation* m_Parent = nullptr;
    std::vector<Transformation*> m_Children;
    uintptr_t m_Flags = 0xffffFFFF;
    struct {
        AMatrix4x4 m_Model = AMatrix4x4::Identity;
        AMatrix4x4 m_Scale = AMatrix4x4::Identity;
        AMatrix4x4 m_Translation = AMatrix4x4::Identity;
        AMatrix4x4 m_Rotation = AMatrix4x4::Identity;
    } m_Matricies;

    struct {
        AVector3 m_Scale = AVector3::One;
        AVector3 m_Translation = AVector3::Zero;
        AVector3 m_Rotation = AVector3::Zero;
    } m_Values;

    bool ScaleDirty(bool parentKnownDirty = false) const         { return (m_Flags & SCALE_DIRTY)        || parentKnownDirty ? true : ParentDirty(); }
    bool TranslationDirty(bool parentKnownDirty = false) const   { return (m_Flags & TRANSLATION_DIRTY)  || parentKnownDirty ? true : ParentDirty(); }
    bool RotationDirty(bool parentKnownDirty = false) const      { return (m_Flags & ROTATION_DIRTY)     || parentKnownDirty ? true : ParentDirty(); }

    bool ParentDirty() const        { return (m_Flags & PARENT_DIRTY)       || (m_Parent && m_Parent->AnyDirty()); }
    bool AnyDirty() const           { return (m_Flags != 0)                 || (m_Parent && m_Parent->AnyDirty()); }

    void SetAllChildrenDirty()      { for (auto* child : m_Children) child->SetParentDirty(); }
    void SetParentDirty()           { m_Flags |= PARENT_DIRTY;      SetAllChildrenDirty(); }
    void SetScaleDirty()            { m_Flags |= SCALE_DIRTY;       SetAllChildrenDirty(); }
    void SetTranslationDirty()      { m_Flags |= TRANSLATION_DIRTY; SetAllChildrenDirty(); }
    void SetRotationDirty()         { m_Flags |= ROTATION_DIRTY;    SetAllChildrenDirty(); }

public:
    AMatrix4x4 const& GetModelNoUpdate() const
    {
        return m_Matricies.m_Model;
    }

    AMatrix4x4 const& GetModel()
    {
        Update();
        return m_Matricies.m_Model;
    }

    void Update() {
        if (AnyDirty()) {
            bool parentDirty = ParentDirty();
            if (ScaleDirty(parentDirty)) {
                m_Matricies.m_Scale = AMatrix4x4::Scale(m_Values.m_Scale);
            }
            if (TranslationDirty(parentDirty)) {
                m_Matricies.m_Translation = AMatrix4x4::Translation(m_Values.m_Translation);
            }
            if (RotationDirty(parentDirty)) {
                m_Matricies.m_Rotation = AQuaternion::RotationEuler(m_Values.m_Rotation).ToMatrix();
            }
            m_Matricies.m_Model = m_Matricies.m_Scale * m_Matricies.m_Rotation * m_Matricies.m_Translation;
            if (m_Parent) {
                if (parentDirty) {
                    m_Parent->Update();
                }
                m_Matricies.m_Model *= m_Parent->m_Matricies.m_Model;
            }
            m_Flags = 0;
        }
    }
};

// Wrap Transformation for now. When we move to a DOD approach this will wrap access to the the SOA
struct SceneObject {
    SceneObject(Transformation* t) :m_Transformation(t){}

    inline void SetParent(SceneObject const& so) {
        m_Transformation->SetParent(so.m_Transformation);
    }

#define ENCAPSULATE_SCENE_OBJECT(namedType) \
    inline AVector3 const& GetLocal##namedType () { return m_Transformation->GetLocal##namedType(); } \
    inline void SetLocal##namedType (AVector3 const& v) { m_Transformation->SetLocal##namedType(v); } \
    inline AVector3 const& GetWorld##namedType () { m_Transformation->GetWorld##namedType(); } \
    inline void SetWorld##namedType (AVector3 const& v) { m_Transformation->SetWorld##namedType(v); }

    ENCAPSULATE_SCENE_OBJECT(Scale);
    ENCAPSULATE_SCENE_OBJECT(Translation);
    ENCAPSULATE_SCENE_OBJECT(Rotation);
#undef ENCAPSULATE_SCENE_OBJECT

    inline size_t CountDepth() const { return m_Transformation->CountDepth(); }

    AMatrix4x4 const& GetModelNoUpdate() const { return m_Transformation->GetModelNoUpdate(); }
    AMatrix4x4 const& GetModel() { return m_Transformation->GetModel(); }
    void Update() { m_Transformation->Update(); }
    void Destroy() { delete m_Transformation; m_Transformation = nullptr; }

private:
    Transformation * m_Transformation;
};

struct Scene {
    std::vector<SceneObject> m_SceneObjects;

    SceneObject NewSceneObject() {
        Transformation* t = new Transformation();
        m_SceneObjects.emplace_back(t);
        return { t };
    }

    void Update() {
        for (auto& so : m_SceneObjects) {
            so.Update();
        }
    }

    Scene() = default;
    ~Scene() {
        for (auto& so : m_SceneObjects) {
            so.Destroy();
        }
    }
};

} // xo