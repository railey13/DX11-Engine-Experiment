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
