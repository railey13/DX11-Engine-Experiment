#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Vendor/nlohmann/json.hpp>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector4D.h>
#include <DX3D/Math/Quaternion.h>

using json = nlohmann::ordered_json;

class SceneHandler {
public:
	SceneHandler();
	~SceneHandler();
public:
	void saveScene(const std::string& filePath);
	void loadScene(const std::string& filePath);
private:
	void saveGameObjects(json& node, GameObject* obj);
	void saveComponents(json& node, GameObject* obj);

	void loadGameObjects(json& node, GameObject* obj);
	void loadComponents(json& node, GameObject* obj);

	Vector3D FromJsonVector3D(const json& node);
	Vector4D FromJsonVector4D(const json& node);
	Quaternion FromJsonQuaternion(const json& node);
private:
	World* m_world = nullptr;
private:
	friend class Game;
};

