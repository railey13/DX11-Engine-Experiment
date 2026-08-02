#include "MainGame.h"

int main() {
	try {
		MainGame game;
		game.run();
	}
	catch (...) {
		return -1;
	}
	return 0;
}