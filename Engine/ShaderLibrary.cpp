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

void ShaderLibrary::requestVertexShaderData(std::wstring vertexShaderName, void** shaderByteCode, size_t* sizeShader){
    GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vertexShaderName.c_str(), "vsmain", shaderByteCode, sizeShader);
}

VertexShaderPtr ShaderLibrary::getVertexShader(std::wstring vertexShaderName) {
    auto it = m_active_vertexShaders.find(vertexShaderName);
    if (it != m_active_vertexShaders.end()) {
        return m_active_vertexShaders[vertexShaderName];
    }
    return nullptr;
}

PixelShaderPtr ShaderLibrary::getPixelShader(std::wstring pixelShaderName) {
    auto it = m_active_pixelShaders.find(pixelShaderName);
    if (it != m_active_pixelShaders.end()) {
        return m_active_pixelShaders[pixelShaderName];
    }
    return nullptr;
}

ShaderLibrary::ShaderLibrary() {
    RenderSystem* renderSystem = GraphicsEngine::get()->getRenderSystem();

    ShaderData shaderData;

    renderSystem->compileVertexShader(ShaderNames::BASE_VERTEX_SHADER_NAME, "vsmain", &shaderData.s_byte_code, &shaderData.s_size);
    m_active_vertexShaders[ShaderNames::BASE_VERTEX_SHADER_NAME] = renderSystem->createVertexShader(shaderData.s_byte_code, shaderData.s_size);

    renderSystem->compilePixelShader(ShaderNames::BASE_PIXEL_SHADER_NAME, "psmain", &shaderData.s_byte_code, &shaderData.s_size);
    m_active_pixelShaders[ShaderNames::BASE_PIXEL_SHADER_NAME] = renderSystem->createPixelShader(shaderData.s_byte_code, shaderData.s_size);

    //renderSystem->compileVertexShader(ShaderNames::TEXTURED_VERTEX_SHADER_NAME, "vsmain", &shaderData.s_byte_code, &shaderData.s_size);
    //m_active_vertexShaders[ShaderNames::TEXTURED_VERTEX_SHADER_NAME] = renderSystem->createVertexShader(shaderData.s_byte_code, shaderData.s_size);

    //renderSystem->compileVertexShader(ShaderNames::TEXTURED_PIXEL_SHADER_NAME, "psmain", &shaderData.s_byte_code, &shaderData.s_size);
    //m_active_pixelShaders[ShaderNames::TEXTURED_PIXEL_SHADER_NAME] = renderSystem->createPixelShader(shaderData.s_byte_code, shaderData.s_size);

}

ShaderLibrary::~ShaderLibrary() {
    if (sharedInstance == nullptr) return;

    m_active_vertexShaders.clear();
    m_active_pixelShaders.clear();
}
