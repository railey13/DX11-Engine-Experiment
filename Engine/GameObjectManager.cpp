#include "GameObjectManager.h"
#include "EngineTime.h"
#include "AppWindow.h"

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"

#include <set>

GameObjectManager* GameObjectManager::sharedInstance = NULL;

GameObjectManager* GameObjectManager::get() {
	return sharedInstance;
}

void GameObjectManager::initialize() {
	sharedInstance = new GameObjectManager();
}

void GameObjectManager::destroy() {
	if (sharedInstance == NULL) return;

	sharedInstance->m_gameobject_list.clear();
	sharedInstance->m_gameobject_table.clear();

	delete sharedInstance;
}

GameObjectManager::GameObjectManager() {

}

GameObjectManager::~GameObjectManager() {

}

void GameObjectManager::update(f32 deltaTime) {
	for (auto& [type, obj] : m_gameobject_table) {
		obj->update(deltaTime);
	}
}

void GameObjectManager::draw(Matrix4x4 view, Matrix4x4 proj) {
	for (auto& [type, obj] : m_gameobject_table) {
		obj->draw(view, proj);
	}
}

GameObjectID GameObjectManager::insertGameObject(GameObjectPtr object, size_t index) {
	if (!object) return 0;

	GameObjectID id;
	if (object->m_id != 0) {
		id = object->m_id;        
	}
	else {
		id = m_nextId;
		m_nextId++;                
	}

	object->m_id = id;

	m_gameobject_table.emplace(id, std::move(object));
	m_gameobject_list.insert(m_gameobject_list.begin() + index, id);

	return id;
}

//GameObjectPtr GameObjectManager::createGameObject(PrimitiveType type) {
//	GameObjectPtr object;
//	switch (type) {
//		case PrimitiveType::CUBE:
//			object = std::make_unique<Cube>();
//			break;
//		case PrimitiveType::SPHERE:
//			object = std::make_unique<Sphere>();
//			break;
//		case PrimitiveType::PLANE:
//			object = std::make_unique<Plane>();
//			break;
//		default: return nullptr;
//	}
//		
//	f32 spawnDistance = 1.0f;
//	Camera* camera = AppWindow::get()->getCamera();
//	Vector3D spawnPos = camera->getPosition() + camera->getForwardDirection() * spawnDistance;
//	object->getTransform()->setPosition(spawnPos);
//
//	addGameObject(object);
//
//	return object;
//}

GameObjectPtr GameObjectManager::removeObject(GameObjectID gameobject_id) {
	if (gameobject_id == 0) return nullptr;

	if (m_selected_gameObject == gameobject_id) m_selected_gameObject = 0;

	auto it = std::find(m_gameobject_list.begin(), m_gameobject_list.end(), gameobject_id);

	if (it != m_gameobject_list.end()) {
		m_gameobject_list.erase(it);
	}

	auto table_it = m_gameobject_table.find(gameobject_id);

	if (table_it == m_gameobject_table.end()) {
		return nullptr;
	}

	GameObjectPtr obj = std::move(table_it->second);
	m_gameobject_table.erase(table_it);

	return obj;
}

void GameObjectManager::setSelectedGameObject(GameObjectID gameobject_id) {
	m_selected_gameObject = gameobject_id;
}

GameObject* GameObjectManager::getSelectedGameObject() const {
	return getGameObjectInternal(m_selected_gameObject);
}

GameObjectID GameObjectManager::getSelectedGameObjectID() const {
	return m_selected_gameObject;
}

std::vector<GameObjectID> GameObjectManager::getAllObjects() const {
	return m_gameobject_list;
}

GameObjectID GameObjectManager::createGameObjectInternal(GameObjectPtr object) {
	if (!object) return 0;
	
	GameObjectID id = m_nextId;

	object->m_id = id;

	f32 spawnDistance = 1.0f;
	Camera* camera = AppWindow::get()->getCamera();
	Vector3D spawnPos = camera->getPosition() + camera->getForwardDirection() * spawnDistance;
	object->getTransform()->setPosition(spawnPos);

	object->m_name = generateUniqueName(object->m_name);
	
	m_gameobject_table.emplace(object->m_id, std::move(object));
	m_gameobject_list.push_back(id);

	m_nextId++;

	return id;
}

std::string GameObjectManager::generateUniqueName(const std::string& baseName) {
	std::set<std::string> obj_names;

	for (auto& [type, obj] : m_gameobject_table) {
		obj_names.insert(obj->m_name);
	}

	if (obj_names.find(baseName) == obj_names.end()) {
		return baseName;
	}

	int i = 1;
	std::string newName;
	do {
		newName = baseName + " " + std::to_string(i);
		std::cout << newName << std::endl;
		i++;
	} while (obj_names.find(newName) != obj_names.end());

	return newName;
}

GameObject* GameObjectManager::getGameObjectInternal(GameObjectID id) const {
	auto it = m_gameobject_table.find(id);

	if (it == m_gameobject_table.end()) return nullptr;

	return it->second.get();
}
