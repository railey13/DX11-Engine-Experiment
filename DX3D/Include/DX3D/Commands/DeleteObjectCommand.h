#pragma once
#include <DX3D/Commands/Command.h>
#include <DX3D/Game/World.h>
#include <DX3D/GameObject/GameObject.h>

class DeleteObjectCommand : public Command {
public:
	DeleteObjectCommand(World* world, size_t id) : m_selected_object(id), m_world(world){
		
	}
	// Inherited via Command
	void execute() override {
		if (m_selected_object != 0) {
			auto obj = m_world->getGameObjectByID(m_selected_object);
			m_index = m_world->getGameObjectIndex(obj);

			m_removed_object = m_world->extractGameObject(obj);
			m_removed_object->setActive(false);
		}
	}

	void undo() override {
		if (m_selected_object == 0) return;

		m_removed_object->setActive(true);
		m_world->insertGameObject(std::move(m_removed_object), m_index);
	}
private:
	size_t m_selected_object;
	GameObjectPtr m_removed_object;
	size_t m_index = 0;
	World* m_world;
};