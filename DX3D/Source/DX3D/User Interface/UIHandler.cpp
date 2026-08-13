#include <DX3D/User Interface/UIHandler.h>
#include <DX3D/User Interface/UI.h>
#include <DX3D/User Interface/MainBarUI.h>
#include <DX3D/User Interface/HierarchyUI.h>
#include <DX3D/User Interface/InspectorUI.h>
#include <DX3D/User Interface/DebugUI.h>

#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/Display.h>

#include <DX3D/Vendor/IMGUI/ImGuiFileDialog.h>

UIHandler::UIHandler(Game* game, HWND hwnd) : m_game(game){
	graphEngine = m_game->getGraphicsEngine();
	IMGUISetUp(hwnd);
	registerUI<MainBarUI>();
	registerUI<HierarchyUI>();
	registerUI<InspectorUI>();
	registerUI<DebugUI>();
}

UIHandler::~UIHandler() {
	m_ui_table.clear();
}

void UIHandler::IMGUISetUp(HWND hwnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(graphEngine->getRenderSystem()->getD11Device(),
		graphEngine->getRenderSystem()->getImmediateDeviceContext()->getContext());
}

void UIHandler::registerUIInternal(UI* ui, size_t id) {
	auto compPtr = std::unique_ptr<UI>(ui);
	m_ui_table.emplace(id, std::move(compPtr));
	ui->m_typeId = id;
	ui->m_world = m_game->getWorld();
}

UI* UIHandler::getUIInternal(size_t id) const {
	auto it = m_ui_table.find(id);

	if (it == m_ui_table.end()) return nullptr;

	return it->second.get();
}

void UIHandler::draw() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& [type, ui] : m_ui_table) {
		ui->draw();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void UIHandler::setAllActive(bool flag) {
	for (auto& [type, ui] : m_ui_table) {
		ui->setActive(flag);
	}
}

void UIHandler::toggleAllActive() {
	for (auto& [type, ui] : m_ui_table) {
		ui->setActive(!ui->isActive());
	}
}

void UIHandler::openFileDialog(std::string path, const char* title, const char* filters, std::function<void(const std::string&)> onFileSelected) {
	m_onFileSelected = onFileSelected;

	std::string filePathStart = "Game/" + path;

	IGFD::FileDialogConfig config;
	config.path = filePathStart;

	ImGuiFileDialog::Instance()->OpenDialog("GlobalFileDlgKey", title, filters, config);
}

void UIHandler::drawFileDialog() {
	ImVec2 dialogSize(800.0f, 500.0f);
	if (ImGuiFileDialog::Instance()->Display("GlobalFileDlgKey")) // => will show a dialog
	{
		if (ImGuiFileDialog::Instance()->IsOk()) {
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

			if(m_onFileSelected && !filePathName.empty()) {
				m_onFileSelected(filePathName);
			}
		}

		ImGuiFileDialog::Instance()->Close();
		m_onFileSelected = nullptr;
	}
}
