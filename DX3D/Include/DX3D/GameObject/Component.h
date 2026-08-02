#pragma once
#include <DX3D/Prerequisites.h>

class Component {
public:
	Component();
	virtual ~Component();

	void release();

	GameObject* getGameObject() { return m_gameobject; }
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

