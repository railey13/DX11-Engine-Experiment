#include <DX3D/User Interface/InspectorUI.h>
#include <DX3D/User Interface/UIHandler.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/GameObject/LightComponent.h>
#include <DX3D/GameObject/MeshComponent.h>
#include <DX3D/GameObject/RigidBodyComponent.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Resource/Texture.h>
#include <DX3D/Commands/CommandInvoker.h>

InspectorUI::InspectorUI(UIHandler* handler) : UI(handler) {
	m_isActive = true;
}

InspectorUI::~InspectorUI() {

}

void InspectorUI::draw() {
	if (m_handler->getGame()->m_state == EngineState::Play) m_disabled = true;
	else if (m_handler->getGame()->m_state == EngineState::Edit) m_disabled = false;

	GameObject* obj = m_world->getSelectedGameObject();
	if (m_isActive) {
		if (ImGui::Begin("Inspector", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			if (obj) {
				// GameObject Name
				RigidBodyComponent* rb = obj->getComponent<RigidBodyComponent>();

				{
					strncpy_s(m_nameBuffer, obj->getName().c_str(), sizeof(m_nameBuffer) - 1);
					m_nameBuffer[sizeof(m_nameBuffer) - 1] = '\0';

					if (ImGui::InputText("Name", m_nameBuffer, sizeof(m_nameBuffer))) {
						if (m_nameBuffer[0] == '\0') {
							obj->setName("GameObject");
						}
						else {
							obj->setName(m_nameBuffer);
						}
					}
				}
				// GameObject Transform
				Transform(obj, rb);
				// GameObject Light
				if (obj->getComponent<LightComponent>() != nullptr) {
					LightComponent* light = obj->getComponent<LightComponent>();
					if (ImGui::CollapsingHeader(obj->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
						f32 radius = light->getRadius();
						f32 intensity = light->getIntensity();
						Vector4D color = light->getColor();
						if (ImGui::SliderFloat("Intensity", &intensity, 0, 10)) {
							light->setIntensity(intensity);
						}
						if (ImGui::SliderFloat("Radius", &radius, 0, 10)) {
							light->setRadius(radius);
						}
						if (ImGui::DragFloat3("Color", &color.m_x, m_transform_speed)) {
							light->setColor(color);
						}
					}
				}
				// GameObject Texture
				if (obj->getComponent<MeshComponent>() != nullptr) {
					if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen)) {
						float buttonWidth = ImGui::CalcTextSize("Cartethyia").x + ImGui::GetStyle().FramePadding.x * 2.0f;

						setButton("Cartethyia", L"Game/Assets/Textures/CartethyiaPuppet.gif", obj, buttonWidth);
						setButton("Aemeath", L"Game/Assets/Textures/AemeathGame.gif", obj, buttonWidth);
						setButton("Mornye", L"Game/Assets/Textures/MornyeThinking.gif", obj, buttonWidth);
						setButton("Default", L"Assets/Textures/white.png", obj, buttonWidth);
					}
				}
				// RigidBody	
				if (rb != nullptr) {
					if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_DefaultOpen)) {
						i32 currentItem = static_cast<i32>(rb->getBodyType());

						const char* bodyTypes[] = { "Static", "Kinematic", "Dynamic" };
						f32 mass = rb->getMass();

						ImGui::BeginDisabled(m_disabled);
						if (ImGui::Combo("Body Type", &currentItem, bodyTypes, 3)) {
							rb->setBodyType(static_cast<RBType>(currentItem));
						}
						if (ImGui::InputFloat("Mass", &mass)) {
							rb->setMass(mass);
						}
						ImGui::EndDisabled();
					}

				}
			}		
		}

		ImGui::End();
	}
}

void InspectorUI::Transform(GameObject* obj, RigidBodyComponent* rb) {
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		TransformComponent* transform = obj->getTransform();
		Vector3D pos = transform->getPosition();
		Vector3D rot = transform->getRotationEuler();
		Vector3D scale = transform->getScale();

		bool changedThisFrame = false;

		if (ImGui::DragFloat3("Position", &pos.m_x, m_transform_speed)) {
			saveTransform(transform);
			obj->getTransform()->setPosition(pos);
			changedThisFrame = true;
		}
		if (ImGui::DragFloat3("Rotation", &rot.m_x, m_transform_speed)) {
			saveTransform(transform);
			obj->getTransform()->setRotationEuler(rot);
		}
		if (ImGui::DragFloat3("Scale", &scale.m_x, m_transform_speed)) {
			saveTransform(transform);
			obj->getTransform()->setScale(scale);
		}

		if (rb) rb->updateTransform(pos, transform->getRotation());

		if (m_isDraggingTransform && ImGui::IsMouseReleased(ImGuiMouseButton_Left) && !changedThisFrame) {
			m_isDraggingTransform = false;

			Vector3D endPos = transform->getPosition();
			Vector3D endRot = transform->getRotationEuler();
			Vector3D endScale = transform->getScale();

			if (endPos != m_dragStartPos || endRot != m_dragStartRot || endScale != m_dragStartScale) {
				auto& p = m_world->m_pendingTransform;
				p.object = obj;

				p.oldPos = m_dragStartPos;   
				p.oldRot = m_dragStartRot;   
				p.oldScale = m_dragStartScale;

				p.newPos = endPos;           
				p.newRot = endRot;           
				p.newScale = endScale;

				m_handler->getGame()->getCommandInvoker()->executeBoundCommand(Action::TransformObject);
			}
		}
	}
}

void InspectorUI::setButton(const char* label, const wchar_t* path, GameObject* obj, float width) {
	auto tex = m_handler->getGame()->getResourceManager()->createResourceFromFile<Texture>(path);

	if (ImGui::Button(label, ImVec2(width, 0))) {
		obj->getComponent<MeshComponent>()->getMaterials()[0]->setMainTexture(tex);	
	}
	ImGui::SameLine();	
	ImGui::Image(tex->getSRV(), ImVec2(32,32));
}

void InspectorUI::saveTransform(TransformComponent* transform) {
	if (!m_isDraggingTransform) {
		m_dragStartPos = transform->getPosition();
		m_dragStartRot = transform->getRotationEuler();
		m_dragStartScale = transform->getScale();
		m_isDraggingTransform = true;
	}
}
