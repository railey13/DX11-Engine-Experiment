#pragma once
#include "Command.h"
#include "AppWindow.h"
#include "GameObjectManager.h"

class SpawnObjectCommand : public Command {
public:
	SpawnObjectCommand(AppWindow* receiver, GameObjectManager::PrimitiveType type) : receiver(receiver), type(type) {

	}
	// Inherited via Command
	void execute() override {
		if (!receiver) return;

		if (object == nullptr){
			object = GameObjectManager::get()->createObject(type, receiver->vs_byte_code, receiver->vs_size);
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
	AGameObject* object = nullptr;
	GameObjectManager::PrimitiveType type;
};