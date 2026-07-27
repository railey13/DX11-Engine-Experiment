#pragma once
//#include <DX3D/Graphics/VertexShader.h>
//#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Resource/Resource.h>
#include <DX3D/Prerequisites.h>
#include <vector>

enum CULL_MODE {
	CULL_MODE_FRONT = 0,
	CULL_MODE_BACK
};

class Material : public Resource { 
public:
	Material(const wchar_t* path, ResourceManager* manager);
	Material(const MaterialPtr& material, ResourceManager* manager);

	void addTexture(const TexturePtr& texture);
	void removeTexture(ui32 index);

	void setData(void* data, ui32 size);

	void setCullMode(CULL_MODE mode);
	CULL_MODE getCullMode() { return m_cull_mode; }
private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<Texture2DPtr> m_vec_textures;
	
	CULL_MODE m_cull_mode = CULL_MODE_BACK;
private:
	friend class GraphicsEngine;
};

