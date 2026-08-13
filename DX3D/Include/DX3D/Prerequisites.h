#pragma once
#include <memory>
#include <iostream>
#include <stdexcept>

class SwapChain;
class DeviceContext;
class VertexBuffer;	
class IndexBuffer;
class ConstantBuffer;
class VertexShader;
class PixelShader;
class Texture2D;
class RenderSystem;
class GraphicsEngine;
class Resource;
class Texture;
class ResourceManager;
class TextureManager;
class Mesh;
class MeshManager;
class Window;
class Game;
class Display;
class InputSystem;
class World;
class SceneHandler;

class PrimitiveFactory;

class GameObject;
class EditorCamera;

class Component;
class TransformComponent;
class MeshComponent;
class CameraComponent;
class LightComponent;
class RigidBodyComponent;
class ColliderComponent;

class Material;

class UIHandler;
class UI;
class HierarchyUI;
class InspectorUI;
class MainBarUI;
class DebugUI;

class CommandInvoker;
class Command;
class CloseWindowCommand;
template<typename T>
class SpawnObjectCommand;
class DeleteObjectCommand;
class TransformCommand;

class PhysicsEngine;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<Texture2D> Texture2DPtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Material> MaterialPtr;

typedef std::unique_ptr<GameObject> GameObjectPtr;

typedef unsigned int ui32;
typedef int	i32;
typedef float f32;
typedef double d64;

enum class LogLevel {
	Info = 0,
	Warning,
	Error
};

struct LogInfo{
	LogLevel level;
	std::string msg;
};

#include <DX3D/Math/Vector3D.h>
struct PendingTransformData {
	GameObject* object = nullptr;
	Vector3D oldPos, oldRot, oldScale;
	Vector3D newPos, newRot, newScale;
};

struct PendingParentData {
	GameObject* child = nullptr;
	GameObject* newParent = nullptr;
};

enum class MeshSource {
	Primitive = 0,
	File
};

enum class LightType {
	DirectionLight = 0,
	PointLight,
};

enum class PrimitiveType {
	Cube = 0,
	Sphere,
	Plane,
	Capsule
};

enum class EngineState {
	Edit = 0,
	Play,
	Pause
};

enum class CameraType {
	Orthographic = 0,
	Perspective
};

enum class CullMode {
	None = 0,
	Front,
	Back
};

enum class ColliderType {
	Box = 0,
	Sphere,
	Capsule
};

enum class RBType {
	Static = 0,
	Kinematic,
	Dynamic
};

enum class Key {
	A = 0,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	Escape,
	Shift,
	Space,
	Enter,
	Delete,
	R_Ctrl,
	L_Ctrl,
	LeftMouseButton,
	RightMouseButton,
	MiddleMouseButton
};

enum class Action {
	SpawnCube = 0,
	SpawnSphere,
	SpawnPlane,
	SpawnCapsule,
	SpawnDirLight,
	SpawnPointLight,
	DeleteObject,
	TransformObject,
	Parent,
	Undo,
	Redo,
	CloseWindow,
};

#define DX3DError(message)\
{\
std::wclog << "DX3D Error: " << message << std::endl;\
throw std::runtime_error("");\
}

#define DX3DWarning(message)\
std::wclog << "DX3D Warning: " << message << std::endl;\

#define DX3DInfo(message)\
std::wclog << "DX3D Info: " << message << std::endl;\
