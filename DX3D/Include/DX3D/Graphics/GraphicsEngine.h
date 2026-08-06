#pragma once
#include <d3d11.h> 
#include <wrl.h>
#include <set>
#include <DX3D/Prerequisites.h>

struct MeshData {
	MeshPtr mesh;
	MaterialPtr material;
};

class GraphicsEngine {
public:
	// initialize the GraphicsEngine and DirectX 11 Device
	GraphicsEngine(Game* game);
	// release all the resources loaded
	~GraphicsEngine();

	void update();
public:
	RenderSystem* getRenderSystem();
	void addComponent(Component* component);
	void removeComponent(Component* component);
	void setEditorCamera(CameraComponent* camera) { m_editorCamera = camera; }
private:
	std::unique_ptr<RenderSystem> m_render_system = nullptr;
	Game* m_game = nullptr;

	std::set<MeshComponent*> m_meshes;
	std::set<CameraComponent*> m_cameras;
	std::set<LightComponent*> m_lights;

	CameraComponent* m_editorCamera = nullptr;
};

