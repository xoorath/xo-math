#pragma once

#include <xo-math.h>
#include <vector>
#include <stdint.h>

using EntityID_t = uint32_t;
using EntityIdx_t = uint32_t;

struct S_EntityIdentity {
	EntityID_t id;
	EntityIdx_t idx;

	bool operator == (S_EntityIdentity const& other) { return id == other.id && idx == other.idx; }

	static const S_EntityIdentity Invalid;
};

template<typename T>
void RemoveLastSwap(std::vector<T> &vec, EntityIdx_t i)
{
	std::swap(vec[vec.size() - 1], vec[i]);
	vec.pop_back();
}

class C_Scene {
public:
	std::vector<xo::AVector3>    m_Pos; // our position
	std::vector<xo::AQuaternion> m_Rot; // our rotation
	std::vector<xo::AVector3>    m_Sca; // our scale
	std::vector<xo::AMatrix4x4>  m_Loc; // local matrix
	std::vector<xo::AMatrix4x4>  m_Mod; // model matrix
	std::vector<std::string>     m_Nam; // object name
	std::vector<EntityID_t>      m_Id;  // our id
	std::vector<EntityID_t>      m_Par; // parent id

	void DestroyIndex(EntityIdx_t idx)
	{
		RemoveLastSwap(m_Id, idx);
		RemoveLastSwap(m_Pos, idx);
		RemoveLastSwap(m_Rot, idx);
		RemoveLastSwap(m_Sca, idx);
		RemoveLastSwap(m_Loc, idx);
		RemoveLastSwap(m_Mod, idx);
		RemoveLastSwap(m_Nam, idx);
		RemoveLastSwap(m_Par, idx);
		// todo: remove children of that entity
	}

	void DestroyId(EntityID_t id)
	{
		EntityID_t count = m_Id.size();
		for (EntityID_t i = 0; i < count; ++i)
		{
			if (m_Id[i] == id)
			{
				DestroyIndex(i);
				break;
			}
		}
	}

	xo::AVector3 m_CameraPosition;
	xo::AVector3 m_CameraTarget;
	xo::AMatrix4x4 m_CameraMatrix;
};

class C_EntityHandle {
public:
	C_Scene *m_Scene = nullptr;
	S_EntityIdentity m_Identity = S_EntityIdentity::Invalid;

	C_EntityHandle() = default;
	C_EntityHandle(C_Scene* scene, S_EntityIdentity identity)
		: m_Scene(scene)
		, m_Identity(identity)
	{
	}

	inline xo::AVector3& Position() { return m_Scene->m_Pos[m_Identity.idx]; }
	inline xo::AQuaternion& Rotation() { return m_Scene->m_Rot[m_Identity.idx]; }
	inline xo::AVector3& Scale() { return m_Scene->m_Sca[m_Identity.idx]; }

	C_EntityHandle GetParent() const {
		auto parentId = m_Scene->m_Par[m_Identity.idx];
		if (parentId == -1) {
			return C_EntityHandle();
		}
		auto const& ids = m_Scene->m_Id;
		EntityIdx_t count = static_cast<EntityIdx_t>(ids.size());
		for (EntityIdx_t i = 0; i < count; ++i)
		{
			if (ids[i] == parentId)
			{
				return C_EntityHandle{ m_Scene, S_EntityIdentity{ parentId, i } };
			}
		}
	}

	void GetChildren(std::vector<C_EntityHandle>& outChildren)
	{
		auto const& ids = m_Scene->m_Par;
		EntityIdx_t count = static_cast<EntityIdx_t>(ids.size());
		for (EntityIdx_t i = 0; i < count; ++i)
		{
			if (ids[i] == m_Identity.id)
			{
				outChildren.emplace_back(m_Scene, S_EntityIdentity{ids[i], i});
			}
		}
	}

	bool IsValid() {
		if (m_Identity == S_EntityIdentity::Invalid)
		{
			// we already know this scene object is invalid
			return false;
		}
		auto const& ids = m_Scene->m_Id;
		EntityIdx_t count = static_cast<EntityIdx_t>(ids.size());

		if (m_Identity.idx < count && ids[m_Identity.idx] == m_Identity.id) {
			// nothing's changed, this scene object is valid.
			return true;
		}

		for (EntityIdx_t i = 0; i < count; ++i)
		{
			if (ids[i] == m_Identity.id) {
				// something caused our handle to move, update it and remember where it is for the future.
				m_Identity.idx = i;
				return true;
			}
		}
		m_Identity = S_EntityIdentity::Invalid;
	}

};