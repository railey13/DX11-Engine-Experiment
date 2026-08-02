#include <DX3D/User Interface/HierarchyUI.h>
#include <DX3D/User Interface/UIHandler.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Commands/CommandInvoker.h>

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

				if (ImGui::BeginPopupContextItem("ItemContexMenu")) {
					m_world->setSeletectedGameObject(obj);

					if (ImGui::MenuItem("Delete")) {
						m_world->getGame()->getCommandInvoker()->executeBoundCommand(Action::DeleteObject);
					}
					ImGui::EndPopup();
				}

				ImGui::PopID();
			}

			ImGui::InvisibleButton("##HierarchyEmptySpace", ImGui::GetContentRegionAvail());

			RightClickWindowPopup();

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)
				&& !ImGui::IsAnyItemHovered()) {
				m_world->setSeletectedGameObject(nullptr);
			}
		}
		ImGui::End();
	}
}

void HierarchyUI::RightClickWindowPopup() {
	if (ImGui::BeginPopupContextItem("WindowsContexMenu", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
		if (ImGui::BeginMenu("Create 3D Objects")) {
			if (ImGui::MenuItem("Cube")) {
				m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::SpawnCube);
			}
			if (ImGui::MenuItem("Sphere")) {
				m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::SpawnSphere);
			}
			if (ImGui::MenuItem("Plane")) {
				m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::SpawnPlane);
			}
			if (ImGui::MenuItem("Capsule")) {
				m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::SpawnCapsule);
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}
}
