#include <DX3D/User Interface/InspectorUI.h>
#include <DX3D/User Interface/UIHandler.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/GameObject/LightComponent.h>
#include <DX3D/GameObject/MeshComponent.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Resource/Texture.h>

InspectorUI::InspectorUI(UIHandler* handler) : UI(handler) {
	m_isActive = true;
}

InspectorUI::~InspectorUI() {

}

void InspectorUI::draw() {
	
	GameObject* obj = m_world->getSelectedGameObject();

	if (m_isActive) {
		if (ImGui::Begin("Inspector", &m_isActive, ImGuiWindowFlags_NoCollapse)) {
			if (obj) {
				// GameObject Name
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
				if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
					TransformComponent* transform = obj->getTransform();
					Vector3D pos = transform->getPosition();
					Vector3D rot = transform->getRotation();
					Vector3D scale = transform->getScale();

					if (ImGui::DragFloat3("Position", &pos.m_x, m_transform_speed)) {
						obj->getTransform()->setPosition(pos);
					}
					if (ImGui::DragFloat3("Rotation", &rot.m_x, m_transform_speed)) {
						obj->getTransform()->setRotation(rot);
					}
					if (ImGui::DragFloat3("Scale", &scale.m_x, m_transform_speed)) {
						obj->getTransform()->setScale(scale);
					}
				}	
				// GameObject Light
				if (obj->getComponent<LightComponent>() != nullptr) {
					LightComponent* light = obj->getComponent<LightComponent>();
					if (ImGui::CollapsingHeader(obj->getName().c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
						f32 radius = light->getRadius();
						f32 strength = light->getColor().m_w;
						if (ImGui::SliderFloat("Strength", &strength, 0, 10)) {
							light->setColor(Vector4D(strength, strength, strength, light->getColor().m_z));
						}
						if (ImGui::SliderFloat("Radius", &radius, 0, 10)) {
							light->setRadius(radius);
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
			}		
		}

		ImGui::End();
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
