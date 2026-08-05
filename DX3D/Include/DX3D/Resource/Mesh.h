#pragma once
#include <DX3D/Resource/Resource.h>
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Prerequisites.h>
#include <DX3D/Math/VertexMesh.h>


struct MaterialSlot {
	size_t start_index = 0;
	size_t num_indices;
	size_t maetrial_id;
};

class Mesh : public Resource {
public:
	Mesh(const wchar_t* full_path, ResourceManager* manager);
	Mesh(VertexMesh* vertex_list_data, ui32 vertex_list_size,
		ui32* index_list_data, unsigned int index_list_size,
		MaterialSlot* material_slot_list, unsigned int material_slot_list_size, 
		ResourceManager* manager);
	~Mesh();

	MaterialSlot getMaterialSlot(ui32 slot);
	size_t getNumMaterialSlots() { return m_material_slots.size(); }
	const Vector3D& getHalfExtents() const { return m_halfExtents; }
private:
	static void computeTangents(
		const Vector3D& v0, const Vector3D& v1, const Vector3D& v2,
		const Vector2D& t0, const Vector2D& t1, const Vector2D& t2,
		Vector3D& tangent, Vector3D& binormal);
private:
	VertexBufferPtr m_vertex_buffer;
	IndexBufferPtr m_index_buffer;
	std::vector<MaterialSlot> m_material_slots;

	Vector3D m_minBounds;
	Vector3D m_maxBounds;
	Vector3D m_halfExtents;
private:
	friend class DeviceContext;
	friend class GraphicsEngine;
	friend class PrimitiveFactory;
};
