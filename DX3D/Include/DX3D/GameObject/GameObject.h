#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector2D.h>
#include <DX3D/Math/Matrix4x4.h>

#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/VertexShader.h>
#include <DX3D/Graphics/PixelShader.h>

#include <DX3D/GameObject/Component.h>

#include <string>
#include <map>
#include <vector>

class GameObjectManager;

class GameObject {
public:
	GameObject();
	virtual ~GameObject();
	void release();
protected:
	virtual void onCreate() {}
protected:
	virtual void update(f32 deltaTime) {};
public:
	template <typename T>
	T* createComponent() {
		static_assert(std::is_base_of <Component, T>::value, "T must be derive from AComponent Class");
		auto e = getComponent<T>();
		if (!e) {
			auto id = typeid(T).hash_code();
			auto c = new T();
			createComponentInternal(c, id);
			return c;
		}
		return e;
	}
	template <typename T>
	T* getComponent() {
		static_assert(std::is_base_of <Component, T>::value, "T must be derive from AComponent Class");
		auto id = typeid(T).hash_code();
		return static_cast<T*>(getComponentInternal(id));
	}
private:
	void createComponentInternal(Component* component, size_t id);
	Component* getComponentInternal(size_t id);
public:
	void removeComponent(size_t id);
	void toggleComponentsActive(bool flag);
	void toggleChildrenActive(bool flag);
public:
	void setName(const std::string& name);
	void setActive(bool active);
	void setParent(GameObject* parent);

	bool isActive() { return m_active; }
	TransformComponent* getTransform() { return m_transform; }
	std::string getName() { return m_name; }
	size_t getID() { return m_id; }
	size_t getInstanceID() { return m_instance_id; }
	World* getWorld() { return m_world; }
	GameObject* getParent() const { return m_parent; }
	const std::vector<GameObject*>& getChildren() const { return m_children; }
	InputSystem* getInputSystem();
private:
	void setMeshData(const MeshPtr& mesh, const MaterialPtr& material = nullptr);
	void addChild(GameObject* child);
	void removeChild(GameObject* child);
protected:
	virtual void onCollisionEnter(GameObject* obj) {}
	virtual void onCollisionStay(GameObject* obj) {}
	virtual void onCollisionExit(GameObject* obj) {}
protected:
	std::map<size_t, std::unique_ptr<Component>> m_components;
	std::string m_name;

	TransformComponent* m_transform = nullptr;
	World* m_world = nullptr;

	bool m_active = true;

	GameObject* m_parent = nullptr;
	std::vector<GameObject*> m_children;
private:
	size_t m_id = 0;
	size_t m_instance_id = 0;

	friend class Component;
	friend class World;
	friend class PrimitiveFactory;
	friend class PhysicsEngine;
};

