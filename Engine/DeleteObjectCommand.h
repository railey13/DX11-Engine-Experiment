#pragma once
#include "Command.h"
#include "AppWindow.h"
#include "GameObjectManager.h"

class DeleteObjectCommand : public Command {
public:
	DeleteObjectCommand(GameObjectID target) : selected_object(target){

	}
	// Inherited via Command
	void execute() override {
		if (selected_object != 0) {
			auto ids = GameObjectManager::get()->getAllObjects();

			for (ui32 i = 0; i < ids.size(); i++) {
				if (ids[i] == selected_object) {
					m_index = i; 
					break;
				}
			}

			removed_object = GameObjectManager::get()->removeObject(selected_object);
		}
	}

	void undo() override {
		if (selected_object == 0) return; 

		GameObjectManager::get()->insertGameObject(std::move(removed_object), m_index);
	}
private:
	GameObjectID selected_object;
	GameObjectPtr removed_object;
	size_t m_index = 0;
};