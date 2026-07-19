#pragma once
#include "Command.h"
#include "AppWindow.h"
#include "GameObjectManager.h"z

class SpawnObjectCommand : public Command {
public:
	SpawnObjectCommand(AppWindow* receiver, GameObjectManager::PrimitiveType type) : receiver(receiver), type(type) {

	}
	// Inherited via Command
	void execute() override {
		if (!receiver) return;

		if (object == nullptr){
			object = GameObjectManager::get()->createObject(type);
		}
		else {
			GameObjectManager::get()->addObject(object);
		}
	}

	void undo() override {
		if (object) {
			GameObjectManager::get()->removeObject(object);
		}
	}

private:
	AppWindow* receiver;
	GameObject* object = nullptr;
	GameObjectManager::PrimitiveType type;
};