#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/Game/EditorCamera.h>
#include <DX3D/Math/Matrix4x4.h>

World::World(Game* game) : m_game(game) {

}

World::~World() {

}

void World::update(f32 deltaTime) {
	deleteGameObjects();

	for (auto&& [typeID, gameObjects] : m_game_objects) {
		for (auto&& [ptr, gameObject] : gameObjects) {
			if(ptr->isActive())
				ptr->update(deltaTime);
		}
	}
}

void World::setSeletectedGameObject(GameObject* obj) {
	m_selected_gameObject = obj;
}

GameObject* World::getGameObjectByName(std::string name) {
	for (auto&& [typeID, gameObjects] : m_game_objects) {
		for (auto&& [ptr, gameObject] : gameObjects) {
			if (ptr->getName() == name) {
				return ptr;
			}
		}
	}
	return nullptr;
}

GameObject* World::getGameObjectByID(size_t ID) {
	for (auto&& [typeID, gameObjects] : m_game_objects) {
		for (auto&& [ptr, gameObject] : gameObjects) {
			if (ptr->getInstanceID() == ID) {
				return ptr;
			}
		}
	}
	return nullptr;
}

GameObjectPtr World::extractGameObject(GameObject* object) {
	if (!object) return nullptr;

	auto it_1 = m_game_objects.find(object->m_id);
	if (it_1 == m_game_objects.end()) return nullptr;

	auto it_2 = it_1->second.find(object);
	if (it_2 == it_1->second.end()) return nullptr;

	GameObjectPtr obj = std::move(it_2->second);
	it_1->second.erase(it_2);

	m_game_objects_order.erase(std::remove(m_game_objects_order.begin(), m_game_objects_order.end(), object),
		m_game_objects_order.end());

	m_game_objects_to_destroy.erase(object);

	if (m_selected_gameObject == object) {
		m_selected_gameObject = nullptr;
	}

	return obj;
}

GameObject* World::insertGameObject(GameObjectPtr object, size_t index) {
	if (!object) return nullptr;

	GameObject* raw = object.get();
	size_t id = raw->m_id;

	m_game_objects[id].emplace(raw, std::move(object));

	if (index >= m_game_objects_order.size())
		m_game_objects_order.push_back(raw);
	else
		m_game_objects_order.insert(m_game_objects_order.begin() + index, raw);

	return raw;
}

size_t World::getGameObjectIndex(GameObject* object) const {
	auto it = std::find(m_game_objects_order.begin(), m_game_objects_order.end(), object);
	if (it == m_game_objects_order.end()) return m_game_objects_order.size();

	return static_cast<size_t>(std::distance(m_game_objects_order.begin(), it));
}

void World::deleteGameObjects() {
	for (auto obj : m_game_objects_to_destroy) {
		m_game_objects[obj->m_id].erase(obj);
		m_game_objects_order.erase(
			std::remove(m_game_objects_order.begin(),
				m_game_objects_order.end(), obj),
			m_game_objects_order.end()
		);
	}
	m_game_objects_to_destroy.clear();
}

void World::createGameObjectInternal(GameObject* object, size_t id) {
	auto gameObjectPtr = std::unique_ptr<GameObject>(object);
	m_game_objects[id].emplace(object, std::move(gameObjectPtr));
	m_game_objects_order.push_back(object);
	object->m_instance_id = m_next_instance_id++;
	object->m_id = id;
	object->m_world = this;
	object->onCreate();

	f32 spawnDistance = 1.0f;
	EditorCamera* camera = m_game->m_editorCamera.get();
	Vector3D spawnPos = camera->getTransform()->getPosition() + camera->getTransform()->getForwardDirection() * spawnDistance;
	object->getTransform()->setPosition(spawnPos);
}

void World::removeGameObject(GameObject* object) {
	m_game_objects_to_destroy.emplace(object);
}

std::string World::generateUniqueName(const std::string& baseName) {
	std::set<std::string> obj_names;

	for (auto obj : m_game_objects_order) {
		obj_names.insert(obj->getName());
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