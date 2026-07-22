#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <locale>
#include <codecvt>

#include "GraphicsEngine.h"
#include "ShaderManager.h"
#include "VertexMesh.h"

#include "Vector3D.h"
#include "Vector2D.h"

Mesh::Mesh(const wchar_t* full_path) : Resource(full_path){
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string err;

	//std::string inputFile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	int size = WideCharToMultiByte(CP_UTF8, 0, full_path, -1, nullptr, 0, nullptr, nullptr);
	std::string inputFile(size - 1, 0); // size includes null terminator, so trim it
	WideCharToMultiByte(CP_UTF8, 0, full_path, -1, &inputFile[0], size, nullptr, nullptr);

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warning, &err, inputFile.c_str());

	if (!err.empty()) throw std::exception("Mesh was not creaetd successfully.");

	if (!res) throw std::exception("Mesh was not creaetd successfully.");

	if(shapes.size() > 1) throw std::exception("Mesh was not creaetd successfully.");


	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		list_vertices.reserve(shapes[s].mesh.indices.size());
		list_indices.reserve(shapes[s].mesh.indices.size());

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size();f++) {
			i32 num_face_verts = shapes[s].mesh.num_face_vertices[f];

			for (unsigned char v = 0; v < num_face_verts; v++) {
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

				VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));

				list_vertices.push_back(vertex);

				list_indices.push_back(index_offset + v);
			}

			index_offset += num_face_verts;
		}
	}

	RenderSystem* render = GraphicsEngine::get()->getRenderSystem();
	ShaderManager* shader = GraphicsEngine::get()->getShaderManager();

	m_vertex_buffer = render->createVertexBuffer(
		&list_vertices[0], 
		sizeof(VertexMesh), 
		list_vertices.size(),
		shader->getMLByteCode(),
		shader->getMLSize()
	);

	m_index_buffer = render->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::~Mesh() {

}

const VertexBufferPtr& Mesh::getVertexBuffer() {
	return m_vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer() {
	return m_index_buffer;
}

std::string WStringToString(const std::wstring& wstr) {
	if (wstr.empty()) return std::string();
	int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
	std::string result(size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &result[0], size, nullptr, nullptr);
	return result;
}
