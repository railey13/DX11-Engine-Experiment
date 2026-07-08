#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine {
public: 
	static GraphicsEngine* get();

	static void initialize();
	static void destroy();
public:
	RenderSystem* getRenderSystem();
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
};

