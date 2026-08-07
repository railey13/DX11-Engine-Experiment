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

#include <DX3D/Input/InputSystem.h>

MainBarUI::MainBarUI(UIHandler* handler) : UI(handler) {
	m_resource = m_handler->getGame()->getResourceManager();
	m_isActive = true;
}

MainBarUI::~MainBarUI() {

}

void MainBarUI::draw() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("GameObjects")) {
			auto invoker = m_handler->getGame()->getCommandInvoker();
			if (ImGui::BeginMenu("3D Objects")) {
				if (ImGui::MenuItem("Cube")) invoker->executeBoundCommand(Action::SpawnCube);
				if (ImGui::MenuItem("Sphere")) invoker->executeBoundCommand(Action::SpawnSphere);
				if (ImGui::MenuItem("Plane")) invoker->executeBoundCommand(Action::SpawnPlane);
				if (ImGui::MenuItem("Capsule")) invoker->executeBoundCommand(Action::SpawnCapsule);
				
				if (ImGui::MenuItem("Spawn 10 PCubes")) spawnNumPrimitives(Action::SpawnCube, invoker);
				if (ImGui::MenuItem("Spawn 10 PSpheres")) spawnNumPrimitives(Action::SpawnSphere, invoker);
				if (ImGui::MenuItem("Spawn 10 PPlane")) spawnNumPrimitives(Action::SpawnPlane, invoker);
				if (ImGui::MenuItem("Spawn 10 PCapsule")) spawnNumPrimitives(Action::SpawnCapsule, invoker);

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Lights")) {
				if (ImGui::MenuItem("Directional Light")) invoker->executeBoundCommand(Action::SpawnDirLight);
				if (ImGui::MenuItem("Point Light")) invoker->executeBoundCommand(Action::SpawnPointLight);

				ImGui::EndMenu();
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

		auto game = m_handler->getGame();

		if (game->isEdit()) {
			if (ImGui::MenuItem("Play")) game->play();
		}
		else {
			if (ImGui::MenuItem("Edit")) game->edit();

			if (!game->isPause()) {
				if (ImGui::MenuItem("Pause")) game->pause();
			}
			else {
				if (ImGui::MenuItem("Resume")) game->resume();
				if (ImGui::MenuItem("Next Frame")) game->frameStep();
			}

			if (!game->m_useEditorCamera){
				if (ImGui::MenuItem("Use Editor Camera")) game->setEditorCameraUse(true);
			}
			else {
				if (ImGui::MenuItem("Use Game Camera")) game->setEditorCameraUse(false);
			}
		}

		ImGui::EndMainMenuBar();
	}
}

void MainBarUI::spawnNumPrimitives(Action action, CommandInvoker* invoker) {
	for (i32 i = 0; i < 10; i++) 
		invoker->executeBoundCommand(action);
}
