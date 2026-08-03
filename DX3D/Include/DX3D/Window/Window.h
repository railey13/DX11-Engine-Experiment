#pragma once
#include <DX3D/Math/Rect.h>
#include <DX3D/Settings.h>
#include <DX3D/Prerequisites.h>

#define NOMINMAX
#include <Windows.h>

class Window {
public:
	// initialize the window
	Window();
	// release the window
	virtual ~Window();

	Rect getClientSize();
	Rect getSizeScreen();

	void* getHwnd() const { return m_hwnd; }
	// EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onResize(const Rect& size);
protected:
	void* m_hwnd;
	Rect m_size = { 0, 0, Settings::WindowWidth, Settings::WindowHeight };
};

