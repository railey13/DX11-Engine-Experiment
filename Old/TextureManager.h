#pragma once
#include <DX3D/Resource/ResourceManager.h>


class TextureManager : public ResourceManager{
public:
	TextureManager();
	~TextureManager();
	TexturePtr createTextureFromFile(const wchar_t* file_path);
protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* file_path);
};

