#pragma once
#include <Windows.h>


class Window {

public:
	Window();

	// initialize the window
	bool init();
	bool broadcast();
	// release the window
	bool release();
	bool isRun();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	// EVENTS
	virtual void onCreate()=0;
	virtual void onUpdate()=0;
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

	~Window();
protected:
	HWND m_hwnd;
	bool m_is_run;

};

