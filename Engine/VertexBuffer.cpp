#include "VertexBuffer.h"
#include "GraphicsEngine.h"

VertexBuffer::VertexBuffer():m_layout(0), m_buffer(0) {

}

bool VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader) {
	if (m_buffer) {
		m_buffer->Release();
	}

	if (m_layout) {
		m_layout->Release();
	}

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer))) {
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{
			"POSITION",						// SEMANTIC NAME
			0,								// SEMANTIC INDEX
			DXGI_FORMAT_R32G32B32_FLOAT,	// FORMAT
			0,								// INPUT SLOT
			0,								// ALIGNED BYTE OFFSET
			D3D11_INPUT_PER_VERTEX_DATA,	// INPUT SLOT CLASS	
			0								// INSTANCE DATA STEP RATE
		},

		{
			"COLOR",						// SEMANTIC NAME
			0,								// SEMANTIC INDEX
			DXGI_FORMAT_R32G32B32_FLOAT,	// FORMAT
			0,								// INPUT SLOT
			12,								// ALIGNED BYTE OFFSET
			D3D11_INPUT_PER_VERTEX_DATA,	// INPUT SLOT CLASS	
			0								// INSTANCE DATA STEP RATE
		},

		{
			"COLOR",						// SEMANTIC NAME
			1,								// SEMANTIC INDEX
			DXGI_FORMAT_R32G32B32_FLOAT,	// FORMAT
			0,								// INPUT SLOT
			24,								// ALIGNED BYTE OFFSET
			D3D11_INPUT_PER_VERTEX_DATA,	// INPUT SLOT CLASS	
			0								// INSTANCE DATA STEP RATE
		}
	};


	UINT size_layout = ARRAYSIZE(layout);

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout))) {
		return false;
	}

	
	return true;
}

UINT VertexBuffer::getSizeVertexList() {
	return this->m_size_list;
}

bool VertexBuffer::release() {
	m_layout->Release();
	m_buffer->Release();

	delete this;
	return true;
}

VertexBuffer::~VertexBuffer() {

}
