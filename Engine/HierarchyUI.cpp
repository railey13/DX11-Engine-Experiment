#include "HierarchyUI.h"
#include "UIManager.h"
#include "AppWindow.h"
#include "GameObjectManager.h"
#include "GameObject.h"

HierarchyUI::HierarchyUI() {
	m_isActive = true;
}

HierarchyUI::~HierarchyUI() {

}

void HierarchyUI::draw() {
	if (m_isActive) {
		if (ImGui::Begin("Hierarchy Tree", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			auto ids = GameObjectManager::get()->getAllObjects();
			for (int i = 0; i < ids.size(); i++) {
				GameObject* obj = GameObjectManager::get()->getGameObjectInternal(ids[i]);
				if (!obj) continue;

				ImGui::PushID(i); 

				bool isSelected = (GameObjectManager::get()->getSelectedGameObject() == obj);
				if (ImGui::Selectable(obj->m_name.c_str(), isSelected)) {
					GameObjectManager::get()->setSelectedGameObject(ids[i]);
				}

				ImGui::PopID();
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)
				&& !ImGui::IsAnyItemHovered()) {
				GameObjectManager::get()->setSelectedGameObject(0);
			}
		}

		ImGui::End();
	}
}
