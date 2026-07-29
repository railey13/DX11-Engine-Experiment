#include <DX3D/Resource/Texture.h>
#include "DX3D/Resource/ResourceManager.h"

#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/Texture2D.h>

#include <DX3D/Game/Game.h>

Texture::Texture(const wchar_t* full_path, ResourceManager* manager) : Resource(full_path, manager) {
	m_texture = manager->getGame()->getGraphicsEngine()->getRenderSystem()->createTexture(full_path);
}

ID3D11ShaderResourceView* Texture::getSRV(){
	return m_texture->getSRV(); 
}
