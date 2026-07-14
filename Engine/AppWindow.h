#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Camera.h"
#include "CommandInvoker.h"

#include "../IMGUI/imgui.h"
#include "../IMGUI/backends/imgui_impl_dx11.h"
#include "../IMGUI/backends/imgui_impl_win32.h"

#include "vector"

class SpawnObjectCommand;
class DeleteObjectCommand;
class CloseWindowCommand;

enum class Action {
	SpawnCube,
	SpawnSphere,
	SpawnPlane,
	DeleteSelectedObject,
	Undo,
	Redo,
	CloseWindow,
};

class AppWindow: public Window, public InputListener{
public:
	static AppWindow* get();

	static void initialize();
	static void destroy();
public:
	void createGraphicsWindow();
private:
	AppWindow(AppWindow const&);
	AppWindow& operator=(AppWindow const&) {};
	static AppWindow* sharedInstance;
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	void onFocus() override;
	void onKillFocus() override;
	void onResize() override;
	// Inherited via InputListener
	void onKeyDown(i32 key) override;
	void onKeyUp(i32 key) override;
	// Inherited via InputListener
	void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
public:
	CommandInvoker& getInvoker() { return m_invoker; }
	Camera* getCamera() { return m_sceneCamera; }
private:
	SwapChainPtr m_swap_chain;

	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	
	CommandInvoker m_invoker;
private:
	void* vs_byte_code = nullptr;
	size_t vs_size = 0;
	void* ps_byte_code = nullptr;
	size_t ps_size = 0;

	Camera* m_sceneCamera;
private:
	friend class SpawnObjectCommand;
	friend class DeleteObjectCommand;
	friend class CloseWindowCommand;
};

