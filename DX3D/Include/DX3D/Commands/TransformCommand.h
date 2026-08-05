#pragma once
#include <DX3D/Commands/Command.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/Math/Vector3D.h>

class TransformCommand : public Command {
public:
	TransformCommand(GameObject* obj, Vector3D oldPos, Vector3D oldRot, Vector3D oldScale,
		Vector3D newPos, Vector3D newRot, Vector3D newScale) {
		m_gameObject = obj;

		m_old_pos = oldPos;
		m_old_rot = oldRot;
		m_old_scale = oldScale;

		m_new_pos = newPos;
		m_new_rot = newRot;
		m_new_scale = newScale;
	}

	// Inherited via Command
	void execute() override {
		auto t = m_gameObject->getTransform();
		t->setPosition(m_new_pos);
		t->setRotationEuler(m_new_rot);
		t->setScale(m_new_scale);
	}

	void undo() override {
		auto t = m_gameObject->getTransform();
		t->setPosition(m_old_pos);
		t->setRotationEuler(m_old_rot);
		t->setScale(m_old_scale);
	}
	
private:
	GameObject* m_gameObject;
	Vector3D m_old_pos, m_old_rot, m_old_scale;
	Vector3D m_new_pos, m_new_rot, m_new_scale;
};