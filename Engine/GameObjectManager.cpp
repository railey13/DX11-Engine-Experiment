#include "GameObjectManager.h"
#include "EngineTime.h"
#include "AppWindow.h"

#include "Cube.h"
#include "Plane.h"
#include "Sphere.h"

GameObjectManager* GameObjectManager::sharedInstance = NULL;

GameObjectManager* GameObjectManager::get() {
	return sharedInstance;
}

void GameObjectManager::initialize() {
	sharedInstance = new GameObjectManager();
}

void GameObjectManager::destroy() {
	if (sharedInstance == NULL) return;

	for (AGameObject* obj : sharedInstance->m_gameobject_list) {
		delete obj;
	}

	sharedInstance->m_gameobject_list.clear();
	sharedInstance->m_gameobject_table.clear();

	delete sharedInstance;
}

GameObjectManager::GameObjectManager() {

}

GameObjectManager::~GameObjectManager() {

}

void GameObjectManager::updateObjects() {
	for (AGameObject* obj : m_gameobject_list) {
		obj->update(EngineTime::getDeltaTime());
	}
}

void GameObjectManager::renderObjects(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) {
	for (AGameObject* obj : m_gameobject_list) {
		obj->draw(vs, ps, view, proj);
	}
}

void GameObjectManager::insertObject(AGameObject* object, size_t index) {
	m_gameobject_list.insert(m_gameobject_list.begin() + index, object);
	m_gameobject_table[object->m_name] = object;
}

void GameObjectManager::addObject(AGameObject* object) {
	if (m_gameobject_table.find(object->m_name) != m_gameobject_table.end()) {
		int count = 0;
		std::string newName;
		do {
			count++;
			newName = object->m_name + " " + "(" + std::to_string(count) + ")";
		} while (m_gameobject_table.find(newName) != m_gameobject_table.end());

		object->m_name = newName;
		m_gameobject_table[object->m_name] = object;
	}
	else {
		m_gameobject_table[object->m_name] = object;
	}

	m_gameobject_list.push_back(object);
}

AGameObject* GameObjectManager::createObject(PrimitiveType type, void* shader_byte_code, size_t size_shader) {
	AGameObject* obj = nullptr;
	switch (type) {
		case PrimitiveType::CUBE:
			obj = new Cube(shader_byte_code, size_shader);
			break;
		case PrimitiveType::SPHERE:
			obj = new Sphere(shader_byte_code, size_shader);
			break;
		case PrimitiveType::PLANE:
			obj = new Plane(shader_byte_code, size_shader);
			break;
		default: break;
	}
		
	f32 spawnDistance = 1.0f;
	Camera* camera = AppWindow::get()->getCamera();
	Vector3D spawnPos = camera->getPosition() + camera->getForwardDirection() * spawnDistance;
	obj->getTransform()->setPosition(spawnPos);

	addObject(obj);

	return obj;
}

void GameObjectManager::removeObject(AGameObject* object) {
	auto it = std::find(m_gameobject_list.begin(), m_gameobject_list.end(), object);

	if (it != m_gameobject_list.end()) {
		m_gameobject_list.erase(it);
	}

	m_gameobject_table.erase(object->m_name);

	if (m_selectedGameObject == object) m_selectedGameObject = nullptr;
}

void GameObjectManager::deleteObject(AGameObject* object) {
	removeObject(object);
	delete object;
}

void GameObjectManager::deleteObjectByName(std::string name) {
	AGameObject* obj = findObjectByName(name);

	if(obj) deleteObject(obj);
}

void GameObjectManager::setSelectedObject(std::string name) {
	auto it = m_gameobject_table.find(name);
	if (it != m_gameobject_table.end()) {
		m_selectedGameObject = it->second;
	}
}

void GameObjectManager::setSelectedObject(AGameObject* object) {
	m_selectedGameObject = object;
}

AGameObject* GameObjectManager::getSelectedObject() {
	return m_selectedGameObject;
}

AGameObject* GameObjectManager::findObjectByName(std::string name) {
	auto it = m_gameobject_table.find(name);
	if (it != m_gameobject_table.end()) {
		return m_gameobject_table[name];
	}
	return nullptr;
}

std::vector<AGameObject*> GameObjectManager::getAllObjects() {
	return m_gameobject_list;
}
