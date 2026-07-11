#include "HierarchyScreen.h"
#include "UIManager.h"
#include "AppWindow.h"

HierarchyScreen::HierarchyScreen() : AUIScreen(UINames::HIERARCHY_SCREEN) {

}

HierarchyScreen::~HierarchyScreen() {

}

void HierarchyScreen::draw() {
	if (m_isActive) {
		if (ImGui::Begin("Hierarchy Tree", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			const std::vector<AGameObject*> objects = AppWindow::get()->getGameObjects();
			for (int i = 0; i < (int)objects.size(); i++) {
				AGameObject* obj = objects[i];
				ImGui::PushID(i); 

				bool isSelected = (AppWindow::get()->m_selectedGameObject == obj);
				if (ImGui::Selectable(obj->m_name.c_str(), isSelected)) {
					AppWindow::get()->m_selectedGameObject = obj;
				}

				ImGui::PopID();
			}
		}

		ImGui::End();
	}
}
