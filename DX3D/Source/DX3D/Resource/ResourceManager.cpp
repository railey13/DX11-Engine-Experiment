#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Texture.h>
#include <DX3D/Resource/Material.h>
#include <filesystem>

ResourceManager::ResourceManager(Game* game) : m_game(game){

}

ResourceManager::~ResourceManager() {

}

ResourcePtr ResourceManager::createResourceFromFileConcrete(const wchar_t* file_path) {
	std::filesystem::path resourcePath = file_path;
	auto ext = resourcePath.extension();

	auto it = m_map_resources.find(file_path);

	if (it != m_map_resources.end()) {
		auto mat = std::dynamic_pointer_cast<Material>(it->second);
		if (mat) {
			return std::make_shared<Material>(mat, this);
		}
		return it->second;
	}

	if (!std::filesystem::exists(resourcePath)) {
		return ResourcePtr();
	}

	ResourcePtr resPtr;

	if (!ext.compare(L".obj")) {
		resPtr = std::make_shared<Mesh>(resourcePath.c_str(), this);  
	}
	else if (!ext.compare(L".png") || !ext.compare(L".jpg") || !ext.compare(L".bmp") || !ext.compare(L".gif")) {
		resPtr = std::make_shared<Texture>(resourcePath.c_str(), this);
	}
	else if (!ext.compare(L".hlsl") || !ext.compare(L".fx")) {
		resPtr = std::make_shared<Material>(resourcePath.c_str(), this);
	}

	if (resPtr) {
		m_map_resources.emplace(file_path, resPtr);

		auto mat = std::dynamic_pointer_cast<Material>(resPtr);
		if (mat) {
			return std::make_shared<Material>(mat, this);  // new copy so it's not shared
		}

		return resPtr;
	}

	return nullptr;
}

MaterialPtr ResourceManager::getDefaultMaterial() {
	if (!m_default_material) {
		auto tex = createResourceFromFile<Texture>(L"Game/Assets/Textures/white.png");
		m_default_material = createResourceFromFile<Material>(L"Assets/Shaders/Material.hlsl");
		m_default_material->addTexture(tex);
	}
	return std::make_shared<Material>(m_default_material, this);
}