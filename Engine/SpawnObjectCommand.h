#pragma once
#include "Command.h"
#include "AppWindow.h"
#include "GameObjectManager.h"



template<typename T>
class SpawnObjectCommand : public Command {
public:
	SpawnObjectCommand() {

	}
	// Inherited via Command
	void execute() override {
		if (createdId == 0 && !removed_object) {
			createdId = GameObjectManager::get()->createGameObjectID<T>();
		}
		else if (removed_object) {
			createdId = GameObjectManager::get()->insertGameObject(std::move(removed_object),
				GameObjectManager::get()->getAllObjects().size());
		}
	}

	void undo() override {
		if (createdId != 0) {
			removed_object = GameObjectManager::get()->removeObject(createdId);
			createdId = 0;
		}
	}

private:
	GameObjectID createdId = 0;
	GameObjectPtr removed_object;
};