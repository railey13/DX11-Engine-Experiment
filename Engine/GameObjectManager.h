#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "Prerequisites.h"
#include "Matrix4x4.h"

class GameObjectManager {
public:
	static GameObjectManager* get();

	static void initialize();
	static void destroy();
private:
	GameObjectManager();
	~GameObjectManager();
	
	GameObjectManager(GameObjectManager const&);
	GameObjectManager& operator=(GameObjectManager const&) {};
	static GameObjectManager* sharedInstance;
public:
	void update(f32 deltaTime);
	void draw(Matrix4x4 view, Matrix4x4 proj);

	GameObjectID insertGameObject(GameObjectPtr object, size_t index);

	GameObjectPtr removeObject(GameObjectID gameobject_id);

	void setSelectedGameObject(GameObjectID gameobject_id);

	GameObject* getSelectedGameObject() const;
	GameObjectID getSelectedGameObjectID() const;

	std::vector<GameObjectID> getAllObjects() const;
private:
	GameObjectID createGameObjectInternal(GameObjectPtr object);
	std::string generateUniqueName(const std::string& baseName);
public:
	template<typename T>
	GameObjectID createGameObjectID() {
		static_assert(std::is_base_of <GameObject, T>::value, "T must be derive from GameObject Class");

		auto c = std::make_unique<T>();
		return createGameObjectInternal(std::move(c));
	}

	template <typename T>
	T* createGameObject() {
		static_assert(std::is_base_of <GameObject, T>::value, "T must be derive from GameObject Class");

		auto c = std::make_unique<T>();
		GameObject* obj = c.get();
		createGameObjectInternal(std::move(c));
		return static_cast<T*>(obj);
	}

	template <typename T>
	T* getGameObject() const {
		static_assert(std::is_base_of<GameObject, T>::value, "T must be derive from GameObject Class");
		for (auto id : m_gameobject_list) {
			GameObject* temp = getGameObjectInternal(id);
			if (!temp) continue;
			if (auto object = dynamic_cast<T*>(temp)) return object;
		}
		return nullptr;
	}

	GameObject* getGameObjectInternal(GameObjectID id) const;
private:
	std::unordered_map<GameObjectID, GameObjectPtr> m_gameobject_table;
	std::vector<GameObjectID> m_gameobject_list;

	GameObjectID m_selected_gameObject;
private:
	GameObjectID m_nextId = 1;
};

