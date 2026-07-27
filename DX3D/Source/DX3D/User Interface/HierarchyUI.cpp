#include <DX3D/User Interface/HierarchyUI.h>
#include <DX3D/User Interface/UIHandler.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>

#include <DX3D/GameObject/GameObject.h>
HierarchyUI::HierarchyUI(UIHandler* handler) : UI(handler){
	m_isActive = true;
}

HierarchyUI::~HierarchyUI() {

}

void HierarchyUI::draw() {
	if (m_isActive) {
		if (ImGui::Begin("Hierarchy Tree", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			auto objs = m_world->getGameObjects();
			for (int i = 0; i < objs.size(); i++) {
				GameObject* obj = objs[i];
				if (!obj) continue;

				ImGui::PushID(i); 

				bool isSelected = (m_world->getSelectedGameObject() == obj);
				if (ImGui::Selectable(obj->getName().c_str(), isSelected)) {
					m_world->setSeletectedGameObject(obj);
				}

				ImGui::PopID();
			}

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)
				&& !ImGui::IsAnyItemHovered()) {
				m_world->setSeletectedGameObject(nullptr);
			}
		}

		ImGui::End();
	}
}
