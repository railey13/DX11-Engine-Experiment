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
		std::map<InputListener*, InputListener*>::iterator it = m_map_listeners.begin();

		while (it != m_map_listeners.end()) {

			it->second->onMouseMove(Point(current_moues_pos.x - m_old_mouse_pos.m_x, current_moues_pos.y - m_old_mouse_pos.m_y));	

			it++;
		}

	}

	m_old_mouse_pos = Point(current_moues_pos.x, current_moues_pos.y);




	if (::GetKeyboardState(m_keys_state)) {
		for (unsigned int i = 0; i < 256; i++) {
			// IF KEY IS DOWN
			if (m_keys_state[i] & 0x80) {
				std::map<InputListener*, InputListener*>::iterator it = m_map_listeners.begin();

				while (it != m_map_listeners.end()) {
					
					// MOUSE BUTTON 

					if (i == VK_LBUTTON) {
						if (m_keys_state[i] != m_old_keys_state[i]) {
							it->second->onLeftMouseDown(Point(current_moues_pos.x, current_moues_pos.y));
						}
					}
					else if (i == VK_RBUTTON) {
						if (m_keys_state[i] != m_old_keys_state[i]) {
							it->second->onRightMouseDown(Point(current_moues_pos.x, current_moues_pos.y));
						}
					}
					else {
						it->second->onKeyDown(i);
					}

					it++;
				}

			}
			// IF KEY IS UP
			else {
				if (m_keys_state[i] != m_old_keys_state[i]) {
					std::map<InputListener*, InputListener*>::iterator it = m_map_listeners.begin();

					while (it != m_map_listeners.end()) {
						if (i == VK_LBUTTON) {
							it->second->onLeftMouseUp(Point(current_moues_pos.x, current_moues_pos.y));
						}
						else if (i == VK_RBUTTON) {
							it->second->onRightMouseUp(Point(current_moues_pos.x, current_moues_pos.y));
						}
						else {
							it->second->onKeyUp(i);
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
	m_map_listeners.insert(std::make_pair<InputListener*, InputListener*>
		(std::forward<InputListener*>(listener), std::forward<InputListener*>(listener)));
}

void InputSystem::removeListener(InputListener* listener) {
	std::map<InputListener*, InputListener*>::iterator it = m_map_listeners.find(listener);

	if (it != m_map_listeners.end()) {
		m_map_listeners.erase(it);
	}

}
