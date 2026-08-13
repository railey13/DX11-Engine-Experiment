#pragma once
#include <string>
#include <DX3D/Prerequisites.h>
#include <DX3D/Debug/Debug.h>

class UI {
public:
	UI(UIHandler* handler);
	virtual ~UI();
public:
	virtual void draw() = 0;
public:
	void toggleActive();
	void setActive(bool flag);
	bool isActive() { return m_isActive; }
protected:
	bool m_isActive = false;
	UIHandler* m_handler = nullptr;
	World* m_world;
private:
	size_t m_typeId = 0;
private:
	friend class UIHandler;
};

