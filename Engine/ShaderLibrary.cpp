#include "ShaderLibrary.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"

#include "VertexShader.h"
#include "PixelShader.h"


ShaderLibrary* ShaderLibrary::sharedInstance = NULL;

ShaderLibrary* ShaderLibrary::get() {
    return sharedInstance;
}

void ShaderLibrary::initialize() {
    sharedInstance = new ShaderLibrary();
}

void ShaderLibrary::destroy() {
    delete sharedInstance;
}

ShaderLibrary::ShaderLibrary() {
	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"Engine/VertexShader.hlsl", "vsmain", &vs_byte_code, &vs_size);
	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(vs_byte_code, vs_size);

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"Engine/PixelShader.hlsl", "psmain", &ps_byte_code, &ps_size);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(ps_byte_code, ps_size);
}

ShaderLibrary::~ShaderLibrary() {

}


