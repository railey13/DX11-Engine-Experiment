#include "AGameObject.h"

AGameObject::AGameObject() {
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/white.png");
}

AGameObject::~AGameObject() {

}

void AGameObject::setPosition(f32 x, f32 y, f32 z) {
	m_position = Vector3D(x,y,z);
}

void AGameObject::setPosition(Vector3D position) {
	m_position = position;
}

void AGameObject::setRotation(f32 x, f32 y, f32 z) {
	m_rotation = Vector3D(x, y, z);
}

void AGameObject::setRotation(Vector3D rotation) {
	m_rotation = rotation;
}

void AGameObject::setScale(f32 x, f32 y, f32 z) {
	m_scale = Vector3D(x, y, z);
}

void AGameObject::setScale(Vector3D scale) {
	m_scale = scale;
}

void AGameObject::setTexture(TexturePtr tex) {
	m_tex = tex;
}

