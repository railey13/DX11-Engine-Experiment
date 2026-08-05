#pragma once
#include <DX3D/Prerequisites.h>
#include <unordered_set>
#include <DX3D/Math/Vector2D.h>
#include <DX3D/Math/Rect.h>

class InputSystem {
public:
	InputSystem();
	~InputSystem();
public:
	bool isKeyUp(const Key& key);
	bool isKeyDown(const Key& key);
	Vector2D getDeltaMousePosition() { return m_delta_mouse_pos; }

	void update();

	void lockCursor(bool lock);
	void toggleLockCursor();
	void setLockArea(const Rect& area);
	void toggleCursorVisible();
	bool isCursorLocked() const { return m_cursor_locked; }
private:
	short getInternalKeyCode(const Key& key);
private:
	short m_keys_state[256] = {};
	short m_old_keys_state[256] = {};
	short m_final_keys_state[256] = {};

	bool m_is_cursor_visible = true;
	bool m_cursor_locked = false;
	Rect m_lock_area;
	
	Vector2D m_lock_area_center;
	Vector2D m_old_mouse_pos;
	Vector2D m_delta_mouse_pos;



};

