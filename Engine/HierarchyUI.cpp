#include "HierarchyUI.h"
#include "UIManager.h"
#include "AppWindow.h"
#include "GameObjectManager.h"

HierarchyUI::HierarchyUI() {
	m_isActive = true;
}

HierarchyUI::~HierarchyUI() {

}

void HierarchyUI::draw() {
	if (m_isActive) {
		if (ImGui::Begin("Hierarchy Tree", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			const std::vector<AGameObject*> objects = GameObjectManager::get()->getAllObjects();
			for (int i = 0; i < (int)objects.size(); i++) {
				AGameObject* obj = objects[i];
				ImGui::PushID(i); 

				bool isSelected = (GameObjectManager::get()->getSelectedObject() == obj);
				if (ImGui::Selectable(obj->m_name.c_str(), isSelected)) {
					GameObjectManager::get()->setSelectedObject(obj);
				}

				ImGui::PopID();
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)
				&& !ImGui::IsAnyItemHovered()) {
				GameObjectManager::get()->setSelectedObject(nullptr);
			}
		}

		ImGui::End();
	}
}
