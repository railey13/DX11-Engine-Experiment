#pragma once
#include <vector>
#include <unordered_map>
#include <string>

#include "AGameObject.h"

class GameObjectManager {
public:
	enum PrimitiveType {
		CUBE,
		SPHERE,
		PLANE	
	};

	static GameObjectManager* get();

	static void initialize();
	static void destroy();
public:
	GameObjectManager();
	~GameObjectManager();
	
	GameObjectManager(GameObjectManager const&);
	GameObjectManager& operator=(GameObjectManager const&) {};
	static GameObjectManager* sharedInstance;
public:
	void updateObjects();
	void renderObjects(Matrix4x4 view, Matrix4x4 proj);

	void insertObject(AGameObject* object, size_t index);
	void addObject(AGameObject* object);
	AGameObject* createObject(PrimitiveType type);

	void removeObject(AGameObject* object);
	void deleteObject(AGameObject* object);
	void deleteObjectByName(std::string name);

	void setSelectedObject(std::string name);
	void setSelectedObject(AGameObject* object);
	AGameObject* getSelectedObject();

	AGameObject* findObjectByName(std::string name);
	std::vector<AGameObject*> getAllObjects();
private:
	std::vector<AGameObject*> m_gameobject_list;
	std::unordered_map<std::string, AGameObject*> m_gameobject_table;

	AGameObject* m_selectedGameObject = nullptr;
};

