#include "InspectorScreen.h"
#include "UIManager.h"
#include "AppWindow.h"
#include "AGameObject.h"

InspectorScreen::InspectorScreen() : AUIScreen(UINames::INSPECTOR_SCREEN){
	m_isActive = true;
}

InspectorScreen::~InspectorScreen() {

}

void InspectorScreen::draw() {
	AGameObject* obj = AppWindow::get()->m_selectedGameObject;

	if (m_isActive) {
		if (ImGui::Begin("Inspector", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			if (obj) {
				strncpy_s(m_nameBuffer, obj->m_name.c_str(), sizeof(m_nameBuffer) - 1);
				m_nameBuffer[sizeof(m_nameBuffer) - 1] = '\0';

				if (ImGui::InputText("Name", m_nameBuffer, sizeof(m_nameBuffer))) {
					if (m_nameBuffer[0] == '\0') {
						obj->m_name = "GameObject";
					}
					else {
						obj->m_name = m_nameBuffer;
					}
				}
			}		
		}

		ImGui::End();
	}
}
