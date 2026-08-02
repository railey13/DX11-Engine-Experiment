#include <DX3D/User Interface/MainBarUI.h>
#include <DX3D/User Interface/UIHandler.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>

#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/MeshComponent.h>
#include <DX3D/GameObject/TransformComponent.h>

#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Texture.h>

#include <DX3D/Resource/PrimitiveFactory.h>
#include <DX3D/Commands/CommandInvoker.h>
#include <DX3D/Commands/SpawnObjectCommand.h>

#include <DX3D/User Interface/HierarchyUI.h>
#include <DX3D/User Interface/InspectorUI.h>

MainBarUI::MainBarUI(UIHandler* handler) : UI(handler) {
	m_resource = m_handler->getGame()->getResourceManager();
	m_isActive = true;
}

MainBarUI::~MainBarUI() {

}

void MainBarUI::draw() {
	if (ImGui::BeginMainMenuBar()) {
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
		if (ImGui::BeginMenu("Panels")) {
			if (ImGui::MenuItem("Hierarchy Panel")) {
				if (HierarchyUI* ui = m_handler->getUI<HierarchyUI>()) {
					ui->toggleActive();
				}
			}
			if (ImGui::MenuItem("Inspector Panel")) {
				if (InspectorUI* ui = m_handler->getUI<InspectorUI>()) {
					ui->toggleActive();
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}
