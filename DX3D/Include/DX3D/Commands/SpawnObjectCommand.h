#pragma once
#include <DX3D/Commands/Command.h>
#include <DX3D/Game/World.h>
#include <DX3D/GameObject/GameObject.h>
#include <functional>

template<typename T>
class SpawnObjectCommand : public Command {
public:
	SpawnObjectCommand(World* world, std::function<void(T*)> on_spawn = nullptr) {
		m_world = world;
		m_on_spawn = std::move(on_spawn);
	}

	// Inherited via Command
	void execute() override {
		// creating a new object
		if (m_object_id == 0) {
			T* obj = m_world->createGameObject<T>();
			m_object_id = obj->getInstanceID();
			if (m_on_spawn) m_on_spawn(obj);
			std::cout << "Created ID: " << m_object_id << std::endl;
			std::cout << "Created Name: " << obj->getName() << std::endl;
		}
		// redo
		else if (m_removed_object) {
			T* obj = static_cast<T*>(m_world->insertGameObject(std::move(m_removed_object), m_insert_index));
			m_object_id = obj->getInstanceID();
			obj->setActive(true);
			std::cout << "ReInserted ID: " << m_object_id << std::endl;
			std::cout << "ReInserted Name: " << obj->getName() << std::endl;
		}
	}

	void undo() override {
		GameObject* obj = m_world->getGameObjectByID(m_object_id);
		if (obj) {
			obj->setActive(false);
			m_insert_index = m_world->getGameObjectIndex(obj);
			m_removed_object = m_world->extractGameObject(obj);
			std::cout << "Removed ID: " << m_object_id << std::endl;
			std::cout << "Removed Name: " << obj->getName() << std::endl;
		}
	}

private:
	World* m_world;
	std::function<void(T*)> m_on_spawn;
	size_t m_object_id = 0;
	GameObjectPtr m_removed_object;
	size_t m_insert_index = 0;
};