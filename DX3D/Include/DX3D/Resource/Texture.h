#pragma once
#include <DX3D/Resource/Resource.h>
#include <d3d11.h>

class Texture : public Resource {
public:
	Texture(const wchar_t* full_path, ResourceManager* manager);
	ID3D11ShaderResourceView* getSRV();
private:
	Texture2DPtr m_texture;
private:
	friend class Material;
};

