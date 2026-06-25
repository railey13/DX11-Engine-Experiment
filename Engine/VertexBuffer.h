#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class VertexBuffer {
public:
	VertexBuffer(void* list_vertices, ui32 size_vertex, ui32  size_list, void* shader_byte_code, ui32 size_byte_shader, RenderSystem* system);

	ui32 getSizeVertexList();

	~VertexBuffer();
private:
	ui32 m_size_vertex;
	ui32 m_size_list;	
private:
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

