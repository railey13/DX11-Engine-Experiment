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

#include <DX3D/User Interface/HierarchyUI.h>
#include <DX3D/User Interface/InspectorUI.h>

MainBarUI::MainBarUI(UIHandler* handler) : UI(handler) {
	resource = m_handler->getGame()->getResourceManager();
	m_isActive = true;
}

MainBarUI::~MainBarUI() {

}

void MainBarUI::draw() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("3D Objects")) {
			if (ImGui::MenuItem("Cube")) {
				auto obj = m_world->createGameObject<GameObject>();
				PrimitiveFactory::createCube(resource, obj);
			}
			if (ImGui::MenuItem("Sphere")) {
				auto obj = m_world->createGameObject<GameObject>();
				PrimitiveFactory::createSphere(resource, obj);
			}
			if (ImGui::MenuItem("Plane")) {
				auto obj = m_world->createGameObject<GameObject>();
				PrimitiveFactory::createPlane(resource, obj);
			}
			if (ImGui::MenuItem("Capsule")) {
				auto obj = m_world->createGameObject<GameObject>();
				PrimitiveFactory::createCapsule(resource, obj);
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
