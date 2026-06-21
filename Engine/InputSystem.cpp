#include "InputSystem.h"
#include <d3dcompiler.h>
 
#include <Windows.h>

InputSystem* InputSystem::sharedInstance = NULL;

InputSystem* InputSystem::get() {

	return sharedInstance;
}

void InputSystem::initialize() {	
	sharedInstance = new InputSystem();
}

InputSystem::InputSystem() {

}

InputSystem::~InputSystem() {

}

void InputSystem::update() {
	POINT current_moues_pos = {};

	::GetCursorPos(&current_moues_pos);
	::GetKeyboardState(m_keys_state);

	if (m_first_time) {
		m_old_mouse_pos = Point(current_moues_pos.x, current_moues_pos.y);
		// prevent onkeyup events on launch (trust ! ! !)
		::memcpy(m_old_keys_state, m_keys_state, sizeof(m_old_keys_state)); 
		m_first_time = false;
	}

	//  IF TRUE, THERE IS A MOUSE EVENT
	if (current_moues_pos.x != m_old_mouse_pos.m_x || current_moues_pos.y != m_old_mouse_pos.m_y) {
		for (auto it = m_set_listeners.begin(); it != m_set_listeners.end(); it++) {
			(*it)->onMouseMove(Point(current_moues_pos.x - m_old_mouse_pos.m_x, current_moues_pos.y - m_old_mouse_pos.m_y));
		}
	}

	m_old_mouse_pos = Point(current_moues_pos.x, current_moues_pos.y);

	if (::GetKeyboardState(m_keys_state)) {
		for (unsigned int i = 0; i < 256; i++) {
			// IF KEY IS DOWN
			if (m_keys_state[i] & 0x80) {
				for (auto it = m_set_listeners.begin(); it != m_set_listeners.end(); it++) {

					// FOR MOUSE BUTTONS
					if (i == VK_LBUTTON) {
						if (m_keys_state[i] != m_old_keys_state[i]) {
							(*it)->onLeftMouseDown(Point(current_moues_pos.x, current_moues_pos.y));
						}
					}
					else if (i == VK_RBUTTON) {
						if (m_keys_state[i] != m_old_keys_state[i]) {
							(*it)->onRightMouseDown(Point(current_moues_pos.x, current_moues_pos.y));
						}
					}
					// FOR KEYS
					else {
						(*it)->onKeyDown(i);
					}
					it++;
				}
			}
			// IF KEY IS UP
			else {
				if (m_keys_state[i] != m_old_keys_state[i]) {
					for (auto it = m_set_listeners.begin(); it != m_set_listeners.end(); it++) {
						if (i == VK_LBUTTON) {
							(*it)->onLeftMouseUp(Point(current_moues_pos.x, current_moues_pos.y));
						}
						else if (i == VK_RBUTTON) {
							(*it)->onRightMouseUp(Point(current_moues_pos.x, current_moues_pos.y));
						}
						else {
							(*it)->onKeyUp(i);
						}
						it++;
					}
				}
			}
		}
		// store current keys to old keys state buffer
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char)*256);
	}
}

void InputSystem::addListener(InputListener* listener) {
	m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener) {
	m_set_listeners.erase(listener);

}
