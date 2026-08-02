#include <DX3D/Input/InputSystem.h>
#include <Windows.h>

InputSystem::InputSystem() {

}

InputSystem::~InputSystem() {

}

bool InputSystem::isKeyUp(const Key& key) {
	return (m_final_keys_state[getInternalKeyCode(key)] == 1);
}

bool InputSystem::isKeyDown(const Key& key) {
	return (m_final_keys_state[getInternalKeyCode(key)] == 0);
}

void InputSystem::update() {
	POINT current_mouse_pos = {};

	::GetCursorPos(&current_mouse_pos);

	if (current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y) {
		m_delta_mouse_pos = Vector2D(
			(float)current_mouse_pos.x - (float)m_old_mouse_pos.m_x,
			(float)current_mouse_pos.y - (float)m_old_mouse_pos.m_y
		);
	}
	else {
		m_delta_mouse_pos = Vector2D(0, 0);

	}

	if (!m_cursor_locked) {
		m_old_mouse_pos = Vector2D(current_mouse_pos.x, current_mouse_pos.y);
	}
	else {
		SetCursorPos((int)m_lock_area_center.m_x, (int)m_lock_area_center.m_y);
		m_old_mouse_pos = m_lock_area_center;
	}

	for (ui32 i = 0; i < 256; i++) {
		m_keys_state[i] = ::GetAsyncKeyState(i);
		// IF KEY IS DOWN
		if (m_keys_state[i] & 0x8001) {
			m_final_keys_state[i] = 0;
		}
		// IF KEY IS UP
		else {
			if (m_keys_state[i] != m_old_keys_state[i]) {
				m_final_keys_state[i] = 1;
			}
			else {
				m_final_keys_state[i] = 2;
			}
		}
	}
	// store current keys to old keys state buffer
	::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
}

void InputSystem::lockCursor(bool lock) {
	m_cursor_locked = lock;
	if (lock) while(ShowCursor(false) >= 0);
	else while(ShowCursor(true) <= 1);
}

void InputSystem::toggleLockCursor() {
	m_cursor_locked = !m_cursor_locked;
}

void InputSystem::setLockArea(const Rect& area) {
	m_lock_area = area;
	m_lock_area_center = Vector2D(area.left + (float)area.width / 2.0f, area.top + (float)area.height / 2.0f);
}

short InputSystem::getInternalKeyCode(const Key& key) {
	short keyWin = 0;

	if (key >= Key::A && key <= Key::Z) {
		keyWin = 'A' + ((short)key - (short)Key::A);
	}
	else if (key >= Key::_0 && key <= Key::_9) {
		keyWin = '0' + ((short)key - (short)Key::_0);
	}
	else if (key == Key::Shift) {
		keyWin = VK_SHIFT;
	}
	else if (key == Key::Escape) {
		keyWin = VK_ESCAPE;
	}
	else if (key == Key::Space) {
		keyWin = VK_SPACE;
	}
	else if (key == Key::Enter) {
		keyWin = VK_RETURN;
	}
	else if (key == Key::Delete) {
		keyWin = VK_DELETE;
	}
	else if (key == Key::R_Ctrl) {
		keyWin = VK_RCONTROL;
	}
	else if (key == Key::L_Ctrl) {
		keyWin = VK_LCONTROL;
	}
	else if (key == Key::LeftMouseButton) {
		keyWin = VK_LBUTTON;
	}
	else if (key == Key::RightMouseButton) {
		keyWin = VK_RBUTTON;
	}
	else if (key == Key::MiddleMouseButton) {
		keyWin = VK_MBUTTON;
	}

	return keyWin;
}
