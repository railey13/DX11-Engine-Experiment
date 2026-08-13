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
#include <DX3D/Vendor/IMGUI/ImGuiFileDialog.h>
#include <DX3D/Graphics/Texture2D.h>
#include <filesystem>

InspectorUI::InspectorUI(UIHandler* handler) : UI(handler) {
	m_isActive = true;
}

InspectorUI::~InspectorUI() {

}

void InspectorUI::draw() {
	if (!m_isActive) return;

	m_disabled = m_handler->getGame()->isPlay();
	GameObject* obj = m_world->getSelectedGameObject();

	if (ImGui::Begin("Inspector", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
		ImGui::BeginDisabled(m_disabled);
		if (obj) {
			// GameObject Name
			activeButton(obj, "name");
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
			RigidBodyComponent* rb = obj->getComponent<RigidBodyComponent>();
			Transform(obj, rb);
			// GameObject Light
			LightComponent* light = obj->getComponent<LightComponent>();
			if (light != nullptr) {
				activeButton(light, "light");
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
			MeshComponent* mesh = obj->getComponent<MeshComponent>();
			if (mesh != nullptr) {
				activeButton(mesh, "mesh");
				if (ImGui::CollapsingHeader("Mesh Texture", ImGuiTreeNodeFlags_DefaultOpen)) {
					ui32 meshSize = mesh->getMaterials().size();
					auto materials = mesh->getMaterials();

					for (ui32 i = 0; i < meshSize; i++) {
						auto material = mesh->getMaterials()[i];
						ui32 texSize = material->getTextures2D().size();

						for (ui32 j = 0; j < texSize; j++) {
							ImGui::PushID(i);
							if (ImGui::Button("Change Texture")) {
								m_handler->openFileDialog(
									"Assets/Textures",
									"Choose Texture",
									".png,.jpg,.jpeg,.gif",
									[mesh, this, material, j](const std::string& filePath) {
										std::wstring path = std::filesystem::path(filePath).wstring();
										auto tex = m_handler->getGame()->getResourceManager()->createResourceFromFile<Texture>(path.c_str());
										material->setTexture(j, tex);
									}
								);
							}
							ImGui::SameLine();
							ImGui::Image(material->getTexture2D(j)->getSRV(), ImVec2(32, 32));
							ImGui::PopID();
						}
					}
				}
			}
			// RigidBody	
			if (rb != nullptr) {
				activeButton(rb, "rigidbody");
				if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_DefaultOpen)) {
					i32 currentItem = static_cast<i32>(rb->getBodyType());

					const char* bodyTypes[] = { "Static", "Kinematic", "Dynamic" };
					f32 mass = rb->getMass();

					bool freeze = rb->isYFreeze();

					if (ImGui::Combo("Body Type", &currentItem, bodyTypes, 3)) {
						rb->setBodyType(static_cast<RBType>(currentItem));
					}
					if (ImGui::InputFloat("Mass", &mass)) {
						rb->setMass(mass);
					}
					if (ImGui::Checkbox("Freeze Y", &freeze)) {
						rb->setFreezeY(freeze);
					}
				}

			}
		}
		ImGui::EndDisabled();
	}
	ImGui::End();


	m_handler->drawFileDialog();
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

void InspectorUI::setButton(const char* label, const wchar_t* path, MeshComponent* mesh, float width) {
	auto tex = m_handler->getGame()->getResourceManager()->createResourceFromFile<Texture>(path);

	if (ImGui::Button(label, ImVec2(width, 0))) {
		mesh->getMaterials()[0]->setMainTexture(tex);
	}
	ImGui::SameLine();	
	ImGui::Image(tex->getSRV(), ImVec2(32,32));
}

void InspectorUI::activeButton(GameObject* obj, const char* name) {
	bool active = obj->isActive();
	std::string label = std::string("Active##") + name;
	if (ImGui::Checkbox(label.c_str(), &active)) {
		obj->setActive(active);
	}
	ImGui::SameLine();
}

void InspectorUI::activeButton(Component* c, const char* name) {
	bool active = c->isActive();
	std::string label = std::string("Active##") + name;
	if (ImGui::Checkbox(label.c_str(), &active)) {
		c->setActive(active);
	}
	ImGui::SameLine();
}

void InspectorUI::saveTransform(TransformComponent* transform) {
	if (!m_isDraggingTransform) {
		m_dragStartPos = transform->getPosition();
		m_dragStartRot = transform->getRotationEuler();
		m_dragStartScale = transform->getScale();
		m_isDraggingTransform = true;
	}
}


