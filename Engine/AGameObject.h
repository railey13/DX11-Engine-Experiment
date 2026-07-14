#pragma once
#include "Prerequisites.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"

#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "AComponent.h"
#include "TransformComponent.h"

#include <string>
#include <map>

struct vertex {
	Vector3D position;
	Vector2D texcoord;
};

__declspec(align(16))
struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	f32 m_time;
	f32 padding[3];
};

enum GAMEOBJECTS {
	CUBE,
	SPHERE,
	PLANE
};

class AGameObject {
public:
	AGameObject();
	virtual ~AGameObject();
public:
	virtual void update(f32 deltaTime) = 0;
	virtual void draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) = 0;
public:
	TransformComponent* getTransform();
public:
	template <typename T>
	T* createComponent() {
		static_assert(std::is_base_of <AComponent, T>::value, "T must be derive from AComponent Class");
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
		static_assert(std::is_base_of <AComponent, T>::value, "T must be derive from AComponent Class");
		auto id = typeid(T).hash_code();
		return static_cast<T*>(getComponentInternal(id));
	}
private:
	void createComponentInternal(AComponent* component, size_t id);
	AComponent* getComponentInternal(size_t id);
	void removeComponent(size_t id);
public:
	void setTexture(TexturePtr tex);
public:
	TexturePtr m_tex = nullptr;

	std::string m_name;
protected:
	TransformComponent* m_transform = nullptr;
protected:
	std::map<size_t, std::unique_ptr<AComponent>> m_components;
	
	friend class AComponent;
};

