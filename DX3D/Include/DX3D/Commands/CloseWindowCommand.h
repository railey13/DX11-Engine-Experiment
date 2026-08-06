#pragma once
#include "Command.h"
#include "AppWindow.h"

class CloseWindowCommand : public Command {
public:
	CloseWindowCommand(AppWindow* receiver) : receiver(receiver) {

	}
	// Inherited via Command
	void execute() override {
		if (receiver) {
			receiver->m_is_run = false;
		}
	}

	void undo() override {

	}
private:
	AppWindow* receiver;
};