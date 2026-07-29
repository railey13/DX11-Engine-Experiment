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
	void removeComponent(size_t id);
public:
	void setName(const std::string& name);

	void setActive(bool active);

	bool isActive() { return m_active; }
	TransformComponent* getTransform() { return m_transform; }
	std::string getName() { return m_name; }

	InputSystem* getInputSystem();
	World* getWorld() { return m_world; }
private:
	void setMeshData(const MeshPtr& mesh, const MaterialPtr& material = nullptr);
protected:
	std::map<size_t, std::unique_ptr<Component>> m_components;
	TransformComponent* m_transform = nullptr;

	size_t m_id = 0;
	World* m_world = nullptr;

	std::string m_name;

	bool m_active = true;
private:
	friend class Component;
	friend class World;
	friend class PrimitiveFactory;
};

