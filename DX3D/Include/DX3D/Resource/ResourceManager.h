#pragma once
#include <unordered_map>
#include <string>

#include <DX3D/Prerequisites.h>
#include <DX3D/Resource/Resource.h>

class ResourceManager {
public:
	ResourceManager(Game* game);
	~ResourceManager();
	
	template<typename T>
	std::shared_ptr<T> createResourceFromFile(const wchar_t* file_path) {
		return std::dynamic_pointer_cast<T>(createResourceFromFileConcrete(file_path));
	}
	
	Game* getGame() { return m_game; }
private:
	ResourcePtr createResourceFromFileConcrete(const wchar_t* file_path);
	MaterialPtr getDefaultMaterial();
private:
	std::unordered_map<std::wstring, ResourcePtr> m_map_resources;
	Game* m_game =  nullptr;

	MaterialPtr m_default_material = nullptr;

	friend class GameObject;
};

