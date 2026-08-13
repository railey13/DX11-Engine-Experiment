#include "MainGame.h"
#include "SceneTester.h"

int main() {
	try {
		//MainGame game;
		//game.run();
		SceneTester test;
		test.run();
	}
	catch (...) {
		return -1;
	}
	return 0;
}