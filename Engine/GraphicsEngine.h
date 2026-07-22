#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "MeshManager.h"

class GraphicsEngine {
public: 
	static GraphicsEngine* get();

	static void initialize();
	static void destroy();
public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();
	ShaderManager* getShaderManager();
	MeshManager* getMeshManager();
private:
	// initialize the GraphicsEngine and DirectX 11 Device
	GraphicsEngine();

	GraphicsEngine(GraphicsEngine const&);
	GraphicsEngine& operator=(GraphicsEngine const&) {};
	static GraphicsEngine* sharedInstance;

	// release all the resources loaded
	~GraphicsEngine();
private:
	RenderSystem* m_render_system = nullptr;
	TextureManager* m_tex_manager = nullptr;
	ShaderManager* m_shader_manager = nullptr;
	MeshManager* m_mesh_manager = nullptr;
};

