#pragma once
#include <DX3D/Math/Rect.h>
#include <DX3D/Settings.h>
#include <DX3D/Prerequisites.h>

#include <chrono>

class Game {
public:
	Game();
	virtual ~Game();

	void run(); 
public:
	GraphicsEngine* getGraphicsEngine() { return m_graphicsEngine.get(); }
	PhysicsEngine* getPhysicsEngne() { return m_physicsEngine.get(); }
	World* getWorld() { return m_world.get(); }
	ResourceManager* getResourceManager() { return m_resourceManager.get(); }
	InputSystem* getInputSystem() { return m_input.get(); }
	CommandInvoker* getCommandInvoker() { return m_commandInvoker.get(); }
protected:
	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}
	virtual void onQuit() {}
private:
	void onDisplaySize(const Rect& size);
	void onInternalUpdate();

	void bindCommands();
public:
	void quit();
private:
	std::unique_ptr<GraphicsEngine> m_graphicsEngine;
	std::unique_ptr<PhysicsEngine> m_physicsEngine;
	std::unique_ptr<Display> m_display;
	std::unique_ptr<World> m_world;
	std::unique_ptr<InputSystem> m_input;
	std::unique_ptr<ResourceManager> m_resourceManager;
	std::unique_ptr<UIHandler> m_uiHandler;

	std::chrono::system_clock::time_point m_previous_time;

	std::unique_ptr<EditorCamera> m_editorCamera;
	std::unique_ptr<CommandInvoker> m_commandInvoker;
public:
	EngineState m_state = EngineState::Edit;
protected:
	bool m_isRunning = true;
private:
	friend class GraphicsEngine;
	friend class Display;
	friend class UIHandler;
	friend class World;
};

