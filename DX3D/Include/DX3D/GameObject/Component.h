#pragma once
#include <DX3D/Prerequisites.h>

class Component {
public:
	Component();
	virtual ~Component();

	void release();

	GameObject* getGameObject() { return m_gameobject; }
	size_t getID() const { return m_typeId; }
	bool isActive() const { return m_active; }
protected:
	virtual void onCreateInternal();
	virtual void onActivate();
	virtual void onDeactivate();
protected:
	size_t m_typeId = 0;
	GameObject* m_gameobject = nullptr;
	bool m_active = true;
	friend class GameObject;
};

