#pragma once
#include <chrono>
#include <ctime>

class EngineTime {
public:
	static void initialize();
	static double getDeltaTime();
	static double getTotalTime();
private:
	EngineTime();
	~EngineTime();
	EngineTime(EngineTime const&);
	EngineTime& operator=(EngineTime const&);

	static EngineTime* sharedInstance;
private:
	std::chrono::system_clock::time_point start;
	std::chrono::system_clock::time_point end;
private:
	double deltaTime = 0.0;
	double totalTime = 0.0;
private:
	static void LogFrameStart();
	static void LogFrameEnd();
private:
	friend class Window;
};

