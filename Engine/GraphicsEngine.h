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
	GraphicsEngine();

	GraphicsEngine(GraphicsEngine const&);
	GraphicsEngine& operator=(GraphicsEngine const&);
	static GraphicsEngine* sharedInstance;

	// initialize the GraphicsEngine and DirectX 11 Device
	bool init();
	// release all the resources loaded
	bool release();

	~GraphicsEngine();
private:
	RenderSystem* m_render_system = nullptr;
};

