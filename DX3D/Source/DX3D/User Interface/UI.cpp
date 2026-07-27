#include <DX3D/User Interface/UI.h>
#include <DX3D/User Interface/UIHandler.h>

UI::UI(UIHandler* handler) : m_handler(handler) {

}

UI::~UI() {

}

void UI::toggleActive() {
	m_isActive = !m_isActive;
}

void UI::setActive(bool flag) {
	m_isActive = flag;
}
