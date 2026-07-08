#pragma once
#include <Windows.h>
#include "Settings.h"

class Window {

public:
	// initialize the window
	Window();
	
	bool isRun();

	RECT getClientWindowRect();

	// EVENTS
	virtual void onCreate()=0;
	virtual void onUpdate()=0;
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onResize(ui32 width, ui32 height) = 0;

	// release the window
	~Window();
private:
	bool broadcast();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;

	ui32 m_window_width = Settings::WindowWidth;
	ui32 m_window_height = Settings::WindowHeight;
};

