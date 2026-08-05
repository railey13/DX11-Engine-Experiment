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

			for (auto obj : objs) {
				if (!obj->getParent()) {
					DrawGameObjectList(obj);
				}
			}

			ImGui::InvisibleButton("##HierarchyEmptySpace", ImGui::GetContentRegionAvail());

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("GAMEOBJECT")) {
					GameObject* drag = *(GameObject**)payLoad->Data;

					if (drag && drag->getParent() != nullptr) {
						m_world->m_pendingParent = { drag, nullptr };
						m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::Parent);
					}
				}
				ImGui::EndDragDropTarget();
			}

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
		if (ImGui::BeginMenu("3D Objects")) {
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
		if (ImGui::BeginMenu("Lights")) {
			if (ImGui::MenuItem("Directional Light")) {
				m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::SpawnDirLight);
			}
			if (ImGui::MenuItem("Point Light")) {
				m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::SpawnPointLight);
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}
}

void HierarchyUI::DrawGameObjectList(GameObject* obj) {
	ImGui::PushID(obj);

	const auto& children = obj->getChildren();
	bool isSelected = (m_world->getSelectedGameObject() == obj);
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (isSelected) flags = flags | ImGuiTreeNodeFlags_Selected;
	if (children.empty()) flags = flags | ImGuiTreeNodeFlags_Leaf;

	bool openChildList = ImGui::TreeNodeEx((void*)(intptr_t)obj, flags, "%s", obj->getName().c_str());
	
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
		m_world->setSeletectedGameObject(obj);
	}

	if (ImGui::BeginPopupContextItem("ItemContexMenu")) {
		m_world->setSeletectedGameObject(obj);

		if (ImGui::MenuItem("Delete")) {
			m_world->getGame()->getCommandInvoker()->executeBoundCommand(Action::DeleteObject);
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
		ImGui::SetDragDropPayload("GAMEOBJECT", &obj, sizeof(GameObject*));
		ImGui::Text("Move %s", obj->getName().c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("GAMEOBJECT")) {
			GameObject* drag = *(GameObject**)payLoad->Data;

			if (drag && drag != obj && !isDescendant(obj, drag)) {
				m_world->m_pendingParent = { drag, obj };
				m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::Parent);
			}
		}
		ImGui::EndDragDropTarget();
	}

	if (openChildList) {
		for (auto c : children) {
			if (c) {
				DrawGameObjectList(c);
			}
		}
		ImGui::TreePop();
	}

	ImGui::PopID();
}

bool HierarchyUI::isDescendant(GameObject* drag, GameObject* obj) {
	GameObject* current = drag;

	while (current) {
		if (current == obj) return true;
		current = current->getParent();
	}

	return false;
}
