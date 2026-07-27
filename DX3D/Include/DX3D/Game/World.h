#pragma once
#include <vector>
#include <map>
#include <string>
#include <set>

#include <DX3D/Prerequisites.h>
#include <DX3D/Math/Matrix4x4.h>

class World {
public:
	World(Game* game);
	~World();
public:
	void update(f32 deltaTime);
	Game* getGame() { return m_game; }

	void setSeletectedGameObject(GameObject* obj);
	GameObject* getSelectedGameObject() { return m_selected_gameObject; }

	GameObject* getGameObject(std::string name);
	const std::vector<GameObject*>& getGameObjects() const { return m_game_objects_order; }
public:
	template <typename T>
	T* createGameObject() {
		static_assert(std::is_base_of<GameObject, T>::value, "T must derive from GameObject class");
		auto id = typeid(T).hash_code();
		auto e = new T();
		
		createGameObjectInternal(e, id);
		return e;
	}
private:
	void createGameObjectInternal(GameObject* object, size_t id);
	void removeGameObject(GameObject* object);

	std::string generateUniqueName(const std::string& baseName);
private:
	std::map<size_t, std::map<GameObject*, std::unique_ptr<GameObject>>> m_game_objects;
	std::vector<GameObject*> m_game_objects_order;
	std::set<GameObject*> m_game_objects_to_destroy;
	Game* m_game = nullptr;
	
	GameObject* m_selected_gameObject = nullptr;
private:
	friend class GameObject;
};



