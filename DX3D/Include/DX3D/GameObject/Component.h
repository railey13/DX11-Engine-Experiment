#pragma once
#include <DX3D/Prerequisites.h>

class Component {
public:
	Component();
	virtual ~Component();

	void release();

	GameObject* getGameObject() { return m_gameobject; }
protected:
	virtual void onCreateInternal();
protected:
	size_t m_typeId = 0;
	GameObject* m_gameobject = nullptr;

	friend class GameObject;
};

