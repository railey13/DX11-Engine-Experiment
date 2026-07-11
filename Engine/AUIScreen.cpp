#include "AUIScreen.h"

AUIScreen::AUIScreen(std::string name) : m_name(name){

}

AUIScreen::~AUIScreen() {

}

std::string AUIScreen::getName() {
	return m_name;
}

void AUIScreen::setActive(bool flag) {
	m_isActive = flag;
}
