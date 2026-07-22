#pragma once
#include "Prerequisites.h"

class ShaderManager {
public:
	ShaderManager(RenderSystem* render);
	~ShaderManager();
public:
	VertexShaderPtr getVS() { return m_vs; }
	PixelShaderPtr getPS() { return m_ps; }

	void* getVSByteCode() { return vs_byte_code; }
	size_t getVSSize() { return vs_size; }

	void* getMLByteCode() { return ml_byte_code; }
	size_t getMLSize() { return ml_size; }

private:
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;

	void* vs_byte_code = nullptr;
	size_t vs_size = 0;
	void* ps_byte_code = nullptr;
	size_t ps_size = 0;
	void* ml_byte_code = nullptr;
	size_t ml_size = 0;
};

