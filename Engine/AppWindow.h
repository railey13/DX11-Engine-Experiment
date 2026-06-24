#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"

#include "vector"
#include "TestObject.h"

class AppWindow: public Window, public InputListener{
public:
	static AppWindow* get();

	static void initialize();
public:
	void createGraphicsWindow();
private:
	AppWindow(AppWindow const&);
	AppWindow& operator=(AppWindow const&);
	static AppWindow* sharedInstance;
public:
	AppWindow();

	void update();

	~AppWindow();

	// Inherited via Window
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;
	void onFocus() override;
	void onKillFocus() override;
	// Inherited via InputListener
	void onKeyDown(int key) override;
	void onKeyUp(int key) override;
	// Inherited via InputListener
	void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;
private:
	void SpawnObject();
	void DestroyObject();
	void DestroyAllObjects();
private:
	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;

	std::vector<TestObject*> objects;
};

