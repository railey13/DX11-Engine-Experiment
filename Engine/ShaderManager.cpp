#include "ShaderManager.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"

#include "VertexShader.h"
#include "PixelShader.h"

ShaderManager::ShaderManager(RenderSystem* render) {
	render->compileVertexShader(L"Engine/VertexShader.hlsl", "vsmain", &vs_byte_code, &vs_size);
	m_vs = render->createVertexShader(vs_byte_code, vs_size);
	//render->releaseCompiledShader();

	render->compilePixelShader(L"Engine/PixelShader.hlsl", "psmain", &ps_byte_code, &ps_size);
	m_ps = render->createPixelShader(ps_byte_code, ps_size);
	//render->releaseCompiledShader();

	render->compileVertexShader(L"Engine/VertexMeshLayoutShader.hlsl", "vsmain", &ml_byte_code, &ml_size);
	::memcpy(m_mesh_layout_byte_code, ml_byte_code, ml_size);
	m_mesh_layout_size = ml_size;
	//render->releaseCompiledShader();
}

ShaderManager::~ShaderManager() {
	
}

