#pragma once
#include <unordered_map>
#include <string>

#include "Prerequisites.h"

namespace ShaderNames {
	constexpr const wchar_t* BASE_VERTEX_SHADER_NAME = L"Engine/VertexShader.hlsl";
	constexpr const wchar_t* TEXTURED_VERTEX_SHADER_NAME = L"Engine/TexturedVertexShader.hlsl";

	constexpr const wchar_t* BASE_PIXEL_SHADER_NAME = L"Engine/PixelShader.hlsl";
	constexpr const wchar_t* TEXTURED_PIXEL_SHADER_NAME = L"Engine/TexturedPixelShader.hlsl";
}

class ShaderLibrary {
public:
	class ShaderData {
	public:
		void* s_byte_code = nullptr;
		size_t s_size = 0;
	};
public:
	static ShaderLibrary* get();

	static void initialize();
	static void destroy();

	void requestVertexShaderData(std::wstring vertexShaderName, void** shaderByteCode, size_t* sizeShader);
	VertexShaderPtr getVertexShader(std::wstring vertexShaderName);
	PixelShaderPtr getPixelShader(std::wstring pixelShaderName);
private:
	ShaderLibrary();
	~ShaderLibrary();

	ShaderLibrary(ShaderLibrary const&);
	ShaderLibrary& operator=(ShaderLibrary const&) {};
	static ShaderLibrary* sharedInstance;
private:
	std::unordered_map<std::wstring, VertexShaderPtr> m_active_vertexShaders;
	std::unordered_map<std::wstring, PixelShaderPtr> m_active_pixelShaders;
};

