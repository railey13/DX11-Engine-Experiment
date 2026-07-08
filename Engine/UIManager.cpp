#include "UIManager.h"

UIManager* UIManager::sharedInstance = NULL;

UIManager* UIManager::get() {
	return sharedInstance;
}

void UIManager::initialize(HWND hwnd) {

}

void UIManager::destroy() {
	if (sharedInstance == NULL) return;

	delete sharedInstance;
}

UIManager::UIManager(HWND hwnd) {

}

UIManager::~UIManager() {

}
