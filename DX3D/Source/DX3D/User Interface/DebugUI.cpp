#include <DX3D/User Interface/DebugUI.h>
#include <DX3D/User Interface/UIHandler.h>
#include <DX3D/Debug/Debug.h>

DebugUI::DebugUI(UIHandler* handler) : UI(handler){
	m_isActive = true;
}

DebugUI::~DebugUI() {

}

void DebugUI::draw() {
	if (!m_isActive) return;

	if (ImGui::Begin("Debug Window",&m_isActive)) {
		for (const auto& log : Debug::getLogs()) {
			ImGui::Text(log.msg.c_str());
		}


	}

	ImGui::End();
}
