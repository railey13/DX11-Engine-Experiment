#pragma once
#include "Command.h"
#include "AppWindow.h"
#include "GameObjectManager.h"

class DeleteObjectCommand : public Command {
public:
	DeleteObjectCommand(AppWindow* receiver, AGameObject* target) : receiver(receiver), object(target){

	}
	// Inherited via Command
	void execute() override {
		if (receiver && object) {
			std::vector<AGameObject*> objects = GameObjectManager::get()->getAllObjects();

			for (ui32 i = 0; i < objects.size(); i++) {
				if (objects[i] == object) {
					m_index = i;
					break;
				}
			}

			GameObjectManager::get()->removeObject(object);
			m_inScene = false;
		}
	}

	void undo() override {
		if (receiver && object) {
			if (m_index >= GameObjectManager::get()->getAllObjects().size()) {
				GameObjectManager::get()->addObject(object);
			}
			else {
				GameObjectManager::get()->insertObject(object, m_index);
			}
			m_inScene = true;
		}
	}

	~DeleteObjectCommand() override {
		if (object && !m_inScene) {
			delete object;
		}
	}
private:
	AppWindow* receiver;
	AGameObject* object = nullptr;
	bool m_inScene = false;
	size_t m_index = 0;
};