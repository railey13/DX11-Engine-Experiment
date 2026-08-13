#include <DX3D/SceneHandler/SceneHandler.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/GameObject/RigidBodyComponent.h>
#include <DX3D/GameObject/MeshComponent.h>
#include <DX3D/GameObject/LightComponent.h>
#include <DX3D/GameObject/GameObject.h>
#include <DX3D/Game/World.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Debug/Debug.h>
#include <DX3D/Resource/PrimitiveFactory.h>
#include <DX3D/Game/Game.h>

#include <fstream>
#include <iostream>
#include <filesystem>

SceneHandler::SceneHandler() {

}

SceneHandler::~SceneHandler() {
	
}

void SceneHandler::saveScene(const std::string& filePath) {
	json root;

	root["Scene"] = std::filesystem::path(filePath).stem().c_str();

	std::vector<GameObject*> list = m_world->getGameObjects();

	for (auto obj : list) {
		if (obj->getParent() == nullptr) {
			json objects;
			saveGameObjects(objects, obj);
			root["GameObjects"].push_back(objects);
		}
	}

	std::ofstream scene(filePath);
	scene << root.dump(4);
}

void SceneHandler::loadScene(const std::string& filePath) {
	std::ifstream file(filePath);

	if (!file.is_open()) {
		Debug::Error("Couldn't open scene: " + filePath);
		return;
	}

	json scene;

	try {
		file >> scene;
	}
	catch (const json::parse_error& e){
		Debug::Error("Parsing error: " + std::string(e.what()));
		file.close();
		return;
	}

	if (scene.begin().key() != "Scene") {
		Debug::Error("Missing Scene Header / Invalid Scene File");
		file.close();
		return;
	}

	file.close();

	if (!scene.contains("GameObjects"))
		return;

	json gameObjects = scene["GameObjects"];

	if (gameObjects == nullptr) return;
	
	m_world->removeAllGameObjects();

	for (json object : gameObjects) {
		GameObject* obj = m_world->createGameObject<GameObject>();
		loadGameObjects(object, obj);

	}
}

void SceneHandler::saveGameObjects(json& node, GameObject* obj) {
	node["Name"] = obj->getName();
	node["IsActive"] = obj->isActive();

	saveComponents(node, obj);
	
	std::vector<GameObject*> children = obj->getChildren();

	for (auto child : children) {
		json objects;
		saveGameObjects(objects, child);
		node["Children"].push_back(objects);
	}
}

void SceneHandler::saveComponents(json& node, GameObject* obj) {

	auto meshComponent = obj->getComponent<MeshComponent>();
	auto rigidBodyComponent = obj->getComponent<RigidBodyComponent>();
	auto transformComponent = obj->getComponent<TransformComponent>();
	auto lightComponent = obj->getComponent<LightComponent>();

	json component;

	if (transformComponent != nullptr) {
		json transform;

		Vector3D pos = transformComponent->getPosition();
		Quaternion rot = transformComponent->getRotation();
		Vector3D rotEuler = transformComponent->getRotationEuler();
		Vector3D scale = transformComponent->getScale();

		transform["Position"] = { pos.m_x, pos.m_y, pos.m_z };
		transform["Rotation"] = { rot.m_x, rot.m_y, rot.m_z, rot.m_w };
		transform["Scale"] = { scale.m_x, scale.m_y, scale.m_z };
		transform["RotationEuler"] = { rotEuler.m_x, rotEuler.m_y, rotEuler.m_z };

		component["Transform"] = transform;
	}

	if (meshComponent != nullptr) {
		json mesh;

		mesh["IsActive"] = meshComponent->isActive();

		if (meshComponent->getMeshSource() == MeshSource::Primitive) {
			mesh["PrimitiveType"] = obj->getComponent<MeshComponent>()->getPrimitiveTypeString();
		}
		else if (meshComponent->getMeshSource() == MeshSource::File) {
			mesh["MeshSource"] = obj->getComponent<MeshComponent>()->getFilePath();
		}

		component["Mesh"] = mesh;
	}

	if (rigidBodyComponent) {
		json rigidbody;

		rigidbody["IsActive"] = rigidBodyComponent->isActive();
		rigidbody["Gravity"] = rigidBodyComponent->isGravityEnabled();
		rigidbody["Mass"] = rigidBodyComponent->getMass();
		rigidbody["BodyType"] = rigidBodyComponent->getBodyTypeString();
		rigidbody["FreezeY"] = rigidBodyComponent->isYFreeze();
		
		component["RigidBody"] = rigidbody;
	}

	if (lightComponent != nullptr) {
		json light;

		light["IsActive"] = lightComponent->isActive();

		light["LightType"] = lightComponent->getLightTypeString();

		Vector4D color = lightComponent->getColor();

		light["Color"] = {color.m_x, color.m_y, color.m_z, color.m_w};
		light["Radius"] = lightComponent->getRadius();
		light["Intensity"] = lightComponent->getIntensity();

		component["Light"] = light;
	}

	node["Components"] = component;
}

void SceneHandler::loadGameObjects(json& node, GameObject* obj) {
	obj->setActive(node.at("IsActive").get<bool>());

	loadComponents(node, obj);
	
	// hard set to prevent being overwritten from loading components (name gets overwitten during creation)
	obj->m_name = node.at("Name").get<std::string>();

	if (node.contains("Children")) {
		for (json child : node.at("Children")) {
			GameObject* obj = m_world->createGameObject<GameObject>();
			loadGameObjects(node, obj);

			obj->setParent(obj);
		}
	}
}

void SceneHandler::loadComponents(json& node, GameObject* obj) {
	if (node.contains("Components")) {
		json components = node["Components"];

		if (components.contains("Transform")) {
			json transform = components.at("Transform");
			TransformComponent* objTransform = obj->getTransform();

			objTransform->setPosition(
				FromJsonVector3D(transform.at("Position"))
			);
			objTransform->setRotation(
				FromJsonQuaternion(transform.at("Rotation"))
			);
			objTransform->setScale(
				FromJsonVector3D(transform.at("Scale"))
			);
			objTransform->setRotationEuler(
				FromJsonVector3D(transform.at("RotationEuler"))
			);
		}

		if (components.contains("Mesh")) {
			json mesh = components.at("Mesh");
			std::string type = mesh.at("PrimitiveType").get<std::string>();
			
			if (type == "Cube") PrimitiveFactory::createCube(m_world->getGame()->getResourceManager(), obj);
			else if (type == "Capsule") PrimitiveFactory::createCapsule(m_world->getGame()->getResourceManager(), obj);
			else if (type == "Plane") PrimitiveFactory::createPlane(m_world->getGame()->getResourceManager(), obj);
			else if (type == "Sphere") PrimitiveFactory::createSphere(m_world->getGame()->getResourceManager(), obj);
			
			MeshComponent* meshObj = obj->getComponent<MeshComponent>();

			meshObj->setActive(
				mesh.at("IsActive").get<bool>()
			);

			meshObj->setPrimitiveType(type);
		}

		if (components.contains("RigidBody")) {
			json rigidBody = components.at("RigidBody");
			
			RigidBodyComponent* rb = obj->getComponent<RigidBodyComponent>();
			
			rb->setActive(
				rigidBody.at("IsActive").get<bool>()
			);

			rb->enableGravity(
				rigidBody.at("Gravity").get<bool>()
			);
			
			rb->setMass(
				rigidBody.at("Mass").get<f32>()
			);

			std::string type = rigidBody.at("BodyType").get<std::string>();

			if (type == "Static") rb->setBodyType(type);
			else if (type == "Kinematic") rb->setBodyType(type);
			else if (type == "Dynamic") rb->setBodyType(type);

			rb->setFreezeY(rigidBody.at("FreezeY").get<bool>());
		}

		if (components.contains("Light")) {
			json light = components.at("Light");

			LightComponent* lightComponent = obj->createComponent<LightComponent>();

			lightComponent->setActive(
				light.at("IsActive").get<bool>()
			);

			std::string type = light.at("LightType").get<std::string>();

			if (type == "DirectionLight") lightComponent->setLightType(type);
			else if (type == "PointLight") lightComponent->setLightType(type);

			lightComponent->setColor(FromJsonVector4D(
				light.at("Color"))
			);

			lightComponent->setRadius(
				light.at("Radius").get<f32>()
			);
			
			lightComponent->setIntensity(
				light.at("Intensity").get<f32>()
			);
		}
	}
}

Vector3D SceneHandler::FromJsonVector3D(const json& node) {
	return Vector3D(
		node.at(0).get<f32>(),
		node.at(1).get<f32>(),
		node.at(2).get<f32>()
	);
}

Vector4D SceneHandler::FromJsonVector4D(const json& node) {
	return Vector4D(
		node.at(0).get<f32>(),
		node.at(1).get<f32>(),
		node.at(2).get<f32>(),
		node.at(3).get<f32>()
	);
}

Quaternion SceneHandler::FromJsonQuaternion(const json& node) {
	return Quaternion(FromJsonVector4D(node));
}
