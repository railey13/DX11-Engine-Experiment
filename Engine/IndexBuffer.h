#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class IndexBuffer {
public:
	IndexBuffer(void* list_indices, ui32 size_list, RenderSystem* system);

	ui32 getSizeIndexList();

	~IndexBuffer();
private:
	ui32  m_size_list;
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};


