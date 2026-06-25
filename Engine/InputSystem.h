#pragma once
#include "InputListener.h"
#include "Prerequisites.h"
#include <unordered_set>
#include "Point.h"

class InputSystem {
public:
	static InputSystem* get();

	static void initialize();
	static void destroy();
private:
	InputSystem();

	InputSystem(InputSystem const&);
	InputSystem& operator=(InputSystem const&);
	static InputSystem* sharedInstance;

	~InputSystem();
public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point &pos);
	void showCursor(bool show);
	
	bool isCursorVisible();
private:
	std::unordered_set<InputListener*> m_set_listeners;
	unsigned char m_keys_state[256] = {};
	unsigned char m_old_keys_state[256] = {};
	Point m_old_mouse_pos;
	bool m_first_time = true;

	bool isCursorVis = true;
};

