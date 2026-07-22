#include "MainBarUI.h"
#include "UIManager.h"
#include "AppWindow.h"
#include "HierarchyUI.h"
#include "InspectorUI.h"
#include "AboutUI.h"

MainBarUI::MainBarUI() {

}

MainBarUI::~MainBarUI() {

}

void MainBarUI::draw() {
	UIManager* ui = UIManager::get();
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("GameObjects")) {
			if (ImGui::MenuItem("Cube")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnCube);
			}
			if (ImGui::MenuItem("Sphere")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnSphere);
			}
			if (ImGui::MenuItem("Plane")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnPlane);
			}			
			if (ImGui::MenuItem("Pot")) {
				AppWindow::get()->getInvoker().executeCommand((int)Action::SpawnPot);
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Panels")) {
			if (ImGui::MenuItem("Scene Hierarchy")) {
				if (HierarchyUI* hierarchy = ui->getUI<HierarchyUI>()) {
					hierarchy->toggleActive();
				}
			}
			if (ImGui::MenuItem("Inspector")) {
				if (InspectorUI* inspector = ui->getUI<InspectorUI>()) {
					inspector->toggleActive();
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) {
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("About")) {
			if (AboutUI* inspector = ui->getUI<AboutUI>()) {
				inspector->toggleActive();
			}
		}
		ImGui::EndMainMenuBar();
	}
}
