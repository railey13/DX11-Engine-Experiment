#pragma once
#include <chrono>
#include <ctime>
#include "Prerequisites.h"

class EngineTime {
public:
	static void initialize();
	static d64 getDeltaTime();
	static d64 getTotalTime();
public:
	static void LimitFPS(d64 frameRateLimit);
	static void UpdateFPSCounter();
	static d64 getFPS();
private:
	EngineTime();
	~EngineTime();
	EngineTime(EngineTime const&);
	EngineTime& operator=(EngineTime const&);

	static EngineTime* sharedInstance;
private:
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;
private:
	d64 deltaTime = 0.0;
	d64 totalTime = 0.0;

	d64 fpsAccumulatedTime = 0.0;
	i32 fpsFrameCount = 0;
	d64 currentFPS = 0.0;
private:
	static void LogFrameStart();
	static void LogFrameEnd();
private:
	friend class Window;
};

