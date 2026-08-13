#include <DX3D/Resource/Mesh.h>
#include "DX3D/Resource/ResourceManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>

#include "DX3D/Math/VertexMesh.h"
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector2D.h>

#include <DX3D/Game/Game.h>

#include <filesystem>

Mesh::Mesh(const wchar_t* full_path, ResourceManager* manager) : Resource(full_path, manager) {
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string err;

	auto inputFile = std::filesystem::path(full_path).string();

	std::string mtldir = inputFile.substr(0, inputFile.find_last_of("\\/"));

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warning, &err, inputFile.c_str(), mtldir.c_str());

	if (!err.empty()) DX3DError("Mesh was not creaetd successfully.");

	if (!res) DX3DError("Mesh was not creaetd successfully.");

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	size_t size_vertex_index_lists = 0;

	for (size_t s = 0; s < shapes.size(); s++) {
		size_vertex_index_lists += shapes[s].mesh.indices.size();
	}

	list_vertices.reserve(size_vertex_index_lists);
	list_indices.reserve(size_vertex_index_lists);

	size_t materialCount = materials.empty() ? 1 : materials.size();
	m_material_slots.resize(materialCount);

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materialCount; m++) {
		m_material_slots[m].start_index = index_global_offset;
		m_material_slots[m].maetrial_id = m;

		for (size_t s = 0; s < shapes.size(); s++) {
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size();f++) {
				int faceMat = shapes[s].mesh.material_ids[f];
				int effectiveMat = (faceMat < 0) ? 0 : faceMat;
				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				if (effectiveMat != (int)m) {
					index_offset += num_face_verts;   
					continue;
				}

				Vector3D vertices_face[3];
				Vector2D texcoords_face[3];

				for (unsigned char v = 0; v < num_face_verts; v++) {
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0.0f;
					tinyobj::real_t ty = 0.0f;

					if (index.texcoord_index >= 0) {
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					vertices_face[v] = Vector3D(vx, vy, vz);
					texcoords_face[v] = Vector2D(tx, ty);
				}

				Vector3D tangent;
				Vector3D binormal;

				computeTangents(
					vertices_face[0], vertices_face[1], vertices_face[2],
					texcoords_face[0], texcoords_face[1], texcoords_face[2],
					tangent, binormal
					);

				for (unsigned char v = 0; v < num_face_verts; v++) {
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
					tinyobj::real_t vz = -attribs.vertices[index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0.0f;
					tinyobj::real_t ty = 0.0f;

					if (index.texcoord_index >= 0) {
						tx = attribs.texcoords[index.texcoord_index * 2 + 0];
						ty = 1.0f - attribs.texcoords[index.texcoord_index * 2 + 1];
					}

					tinyobj::real_t nx = 0.0f;
					tinyobj::real_t ny = 0.0f;
					tinyobj::real_t nz = 0.0f;

					if (index.normal_index >= 0) {
						nx = attribs.normals[index.normal_index * 3 + 0];
						ny = attribs.normals[index.normal_index * 3 + 1];
						nz = -attribs.normals[index.normal_index * 3 + 2];
					}

					Vector3D v_tangent;
					Vector3D v_binormal;

					v_binormal = Vector3D(nx, ny, nz).vectorProduct(tangent);
					v_tangent = v_binormal.vectorProduct(Vector3D(nx, ny, nz));
					
					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz), v_tangent, v_binormal);
					list_vertices.push_back(vertex);

					list_indices.push_back((ui32)index_global_offset + v);
					//list_indices.push_back((ui32)index_global_offset + (num_face_verts - 1 - v));
				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}

		m_material_slots[m].num_indices = index_global_offset - m_material_slots[m].start_index;
	}

	if (list_vertices.empty() || list_indices.empty()) DX3DError("Mesh has no vertex/index data after parsing.");

	//
	m_minBounds = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	m_maxBounds = Vector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const auto& vertex : list_vertices) {
		const Vector3D& pos = vertex.m_position; 
		m_minBounds.m_x = std::min(m_minBounds.m_x, pos.m_x);
		m_minBounds.m_y = std::min(m_minBounds.m_y, pos.m_y);
		m_minBounds.m_z = std::min(m_minBounds.m_z, pos.m_z);

		m_maxBounds.m_x = std::max(m_maxBounds.m_x, pos.m_x);
		m_maxBounds.m_y = std::max(m_maxBounds.m_y, pos.m_y);
		m_maxBounds.m_z = std::max(m_maxBounds.m_z, pos.m_z);
	}
	m_halfExtents = (m_maxBounds - m_minBounds) * 0.5f;
	//

	auto render = m_manager->getGame()->getGraphicsEngine()->getRenderSystem();

	m_vertex_buffer = render->createVertexBuffer(
		&list_vertices[0], 
		sizeof(VertexMesh), 
		list_vertices.size()
	);

	m_index_buffer = render->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::Mesh(VertexMesh* vertex_list_data, ui32 vertex_list_size, ui32* index_list_data, 
	unsigned int index_list_size, MaterialSlot* material_slot_list, 
	unsigned int material_slot_list_size, 
	ResourceManager* manager) : Resource(L"", manager) {
	//
	m_minBounds = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
	m_maxBounds = Vector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (ui32 i = 0; i < vertex_list_size; ++i) {
		const Vector3D& pos = vertex_list_data[i].m_position;
		m_minBounds.m_x = std::min(m_minBounds.m_x, pos.m_x);
		m_minBounds.m_y = std::min(m_minBounds.m_y, pos.m_y);
		m_minBounds.m_z = std::min(m_minBounds.m_z, pos.m_z);

		m_maxBounds.m_x = std::max(m_maxBounds.m_x, pos.m_x);
		m_maxBounds.m_y = std::max(m_maxBounds.m_y, pos.m_y);
		m_maxBounds.m_z = std::max(m_maxBounds.m_z, pos.m_z);
	}
	m_halfExtents = (m_maxBounds - m_minBounds) * 0.5f;
	//
	auto render = m_manager->getGame()->getGraphicsEngine()->getRenderSystem();

	m_vertex_buffer = render->createVertexBuffer(vertex_list_data,
		sizeof(VertexMesh),
		(UINT)vertex_list_size
	);

	m_index_buffer = render->createIndexBuffer(index_list_data,
		(UINT)index_list_size);

	this->m_material_slots.resize(material_slot_list_size);

	for (unsigned int i = 0; i < material_slot_list_size; i++) {
		this->m_material_slots[i] = material_slot_list[i];
	}
}

Mesh::~Mesh() {

}

MaterialSlot Mesh::getMaterialSlot(ui32 slot) {
	if (slot >= m_material_slots.size()) return MaterialSlot();
	return m_material_slots[slot];
}

void Mesh::computeTangents(const Vector3D& v0, const Vector3D& v1, const Vector3D& v2, const Vector2D& t0, const Vector2D& t1, const Vector2D& t2, Vector3D& tangent, Vector3D& binormal) {
	Vector3D deltaPos1 = v1 - v0;
	Vector3D deltaPos2 = v2 - v0;

	Vector2D deltaUV1 = t1 - t0;
	Vector2D deltaUV2 = t2 - t0;

	float r = 1.0f / (deltaUV1.m_x * deltaUV2.m_y - deltaUV1.m_y * deltaUV2.m_x);
	tangent = (deltaPos1 * deltaUV2.m_y - deltaPos2 * deltaUV1.m_y);
	tangent = tangent.normalize();
	binormal = (deltaPos2 * deltaUV1.m_x - deltaPos1 * deltaUV2.m_x);
	binormal = binormal.normalize();
}
