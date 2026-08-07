#pragma once
#include <unordered_map>
#include <stack>
#include <functional>
#include <iostream>

#include <DX3D/Commands/Command.h>
#include <DX3D/Prerequisites.h>

class CommandInvoker {
public:
	void bindCommand(Action key, std::function<std::unique_ptr<Command>()> command) {
		commands[key] = std::move(command);
	}

	void executeBoundCommand(Action key) {
		auto it = commands.find(key);
		if (it != commands.end()) {
			executeCommand(it->second());
		}
	}

	void undo() {
		if (!undoStack.empty()) {
			auto cmd = std::move(undoStack.top());
			undoStack.pop();
			cmd->undo();
			redoStack.push(std::move(cmd));

			std::cout << "UNDO" << std::endl;
		}
	}

	void redo() {
		if (!redoStack.empty()) {
			auto cmd = std::move(redoStack.top());
			redoStack.pop();
			cmd->execute();
			undoStack.push(std::move(cmd));
			std::cout << "REDO" << std::endl;
		}
	}
private:
	void executeCommand(std::unique_ptr<Command> cmd) {
		std::cout << "plane" << std::endl;
		cmd->execute();
		undoStack.push(std::move(cmd));

		// clear redo stack once a new command was executed
		while (!redoStack.empty()) {
			redoStack.pop();
		}
	}

private:
	std::unordered_map<Action, std::function<std::unique_ptr<Command>()>> commands;

	std::stack<std::unique_ptr<Command>> undoStack;
	std::stack<std::unique_ptr<Command>> redoStack;
};

