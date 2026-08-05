#pragma once
#include <DX3D/User Interface/UI.h>
#include <DX3D/Math/Vector3D.h>

class InspectorUI : public UI{
public:
	InspectorUI(UIHandler* handler);
	~InspectorUI();

	// Inherited via UI
	void draw() override;
private:
	void Transform(GameObject* obj, RigidBodyComponent* rb);
	void setButton(const char* label, const wchar_t* path, GameObject* obj, float width);

	void saveTransform(TransformComponent* transform);
private:
	bool m_disabled = false;
	bool m_isDraggingTransform = false;
	char m_nameBuffer[128] = "";
	f32 m_transform_speed = 0.05f;
	Vector3D m_dragStartPos, m_dragStartRot, m_dragStartScale;
};

