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

	m_frameCount++;
	m_fpsUpdateTimer += ImGui::GetIO().DeltaTime;

	if (m_fpsUpdateTimer >= 1.0f) {
		m_displayedFps = (f32)m_frameCount / m_fpsUpdateTimer;
		m_frameCount = 0;
		m_fpsUpdateTimer = 0.0f;
	}

	if (ImGui::Begin("Debug Window",&m_isActive)) {
		if (ImGui::Button("Clear Logs")) {
			Debug::clearLogs();
		}
		ImGui::SameLine();
		ImGui::Text("FPS: %.1f", m_displayedFps);

		if (ImGui::BeginChild("Logs", ImVec2(0,0), true, ImGuiWindowFlags_HorizontalScrollbar)) {
			for (const auto& log : Debug::getLogs()) {
				ImVec4 color;
				if (log.level == LogLevel::Warning) color = ImVec4(1,0.8,0.2,1); 
				if (log.level == LogLevel::Error) color = ImVec4(1,0.3,0.3,1); 
				if (log.level == LogLevel::Info) color = ImVec4(1,1,1,1); 

				ImGui::PushStyleColor(ImGuiCol_Text, color);
				ImGui::TextUnformatted(log.msg.c_str());
				ImGui::PopStyleColor();
			}
		}
		
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
			ImGui::SetScrollHereY(1);
		}

		ImGui::EndChild();
	}

	ImGui::End();
}
