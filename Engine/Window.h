#pragma once
#include <Windows.h>


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

	// release the window
	~Window();
private:
	bool broadcast();
protected:
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;
};

