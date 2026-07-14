#pragma once
#include "Prerequisites.h"

class AComponent {
public:
	AComponent();
	virtual ~AComponent();

	void release();
private:
	size_t m_typeId = 0;
	AGameObject* m_gameobject = nullptr;

	friend class AGameObject;
};

