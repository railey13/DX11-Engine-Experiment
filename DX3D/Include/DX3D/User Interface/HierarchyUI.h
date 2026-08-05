#pragma once
#include <DX3D/User Interface/UI.h>

class HierarchyUI : public UI{
public:
	HierarchyUI(UIHandler* handler);
	~HierarchyUI();

	// Inherited via UI
	void draw() override;
private:
	void RightClickWindowPopup();
	void DrawGameObjectList(GameObject* obj);
	bool isDescendant(GameObject* drag, GameObject* obj);
private:
	bool m_isSelected = false;
};

