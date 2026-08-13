#pragma once
//#include <DX3D/Graphics/VertexShader.h>
//#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Resource/Resource.h>
#include <DX3D/Prerequisites.h>
#include <vector>

class Material : public Resource { 
public:
	Material(const wchar_t* path, ResourceManager* manager);
	Material(const MaterialPtr& material, ResourceManager* manager);

	void setTexture(ui32 index, const TexturePtr& texture);
	void setMainTexture(const TexturePtr& texture);
	void addTexture(const TexturePtr& texture);
	void removeTexture(ui32 index);

	void setData(void* data, ui32 size);

	void setCullMode(CullMode mode);

	std::vector<Texture2DPtr> getTextures2D() const { return m_vec_textures; }
	Texture2DPtr getTexture2D(ui32 index) { return m_vec_textures[index]; }
	CullMode getCullMode() { return m_cull_mode; }
private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	std::vector<Texture2DPtr> m_vec_textures;
	
	CullMode m_cull_mode = CullMode::Back;
private:
	friend class GraphicsEngine;
};

