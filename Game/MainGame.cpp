#include "MainGame.h"
#include "Player.h"
#include <DX3D/Resource/PrimitiveFactory.h>
#include <DX3D/GameObject/ObjectComponentFactory.h>
#include <reactphysics3d/reactphysics3d.h>

MainGame::MainGame() {

}

MainGame::~MainGame() {

}

void MainGame::onCreate() {
	Game::onCreate();


	auto sponza = getResourceManager()->createResourceFromFile<Mesh>(L"Game/Assets/Meshes/sponza_basic.obj");
	auto terrain = getResourceManager()->createResourceFromFile<Mesh>(L"Game/Assets/Meshes/terrain.obj");

	auto floor = getResourceManager()->createResourceFromFile<Texture>(L"Game/Assets/Textures/sponza_floor_a_diff.jpg");

	auto brick = getResourceManager()->createResourceFromFile<Texture>(L"Game/Assets/Textures/sponza_bricks_a_diff.jpg");
	auto arch = getResourceManager()->createResourceFromFile<Texture>(L"Game/Assets/Textures/sponza_arch_diff.jpg");
	auto colA = getResourceManager()->createResourceFromFile<Texture>(L"Game/Assets/Textures/sponza_column_a_diff.jpg");
	auto colB = getResourceManager()->createResourceFromFile<Texture>(L"Game/Assets/Textures/sponza_column_b_diff.jpg");
	auto colC = getResourceManager()->createResourceFromFile<Texture>(L"Game/Assets/Textures/sponza_column_c_diff.jpg");
	auto flagPole = getResourceManager()->createResourceFromFile<Texture>(L"Game/Assets/Textures/sponza_flagpole_diff.jpg");

	auto floorMat = getResourceManager()->createResourceFromFile<Material>(L"Assets/Shaders/Material.hlsl");
	floorMat->setMainTexture(floor);
	auto brickMat = getResourceManager()->createResourceFromFile<Material>(L"Assets/Shaders/Material.hlsl");
	brickMat->setMainTexture(brick);
	auto archMat = getResourceManager()->createResourceFromFile<Material>(L"Assets/Shaders/Material.hlsl");
	archMat->setMainTexture(arch);
	auto colAMat = getResourceManager()->createResourceFromFile<Material>(L"Assets/Shaders/Material.hlsl");
	colAMat->setMainTexture(colA);
	auto colBMat = getResourceManager()->createResourceFromFile<Material>(L"Assets/Shaders/Material.hlsl");
	colBMat->setMainTexture(colB);
	auto colCMat = getResourceManager()->createResourceFromFile<Material>(L"Assets/Shaders/Material.hlsl");
	colCMat->setMainTexture(colC);
	auto flagPoleMat = getResourceManager()->createResourceFromFile<Material>(L"Assets/Shaders/Material.hlsl");
	flagPoleMat->setMainTexture(flagPole);

	// dir light
	{
		auto obj = getWorld()->createGameObject<GameObject>();
		ObjectComponentFactory::CreateDirLight(obj);
	}

	// floor
	{
		auto obj = getWorld()->createGameObject<GameObject>();
		auto mesh = obj->createComponent<MeshComponent>();
		mesh->setMesh(terrain);
		mesh->addMaterial(floorMat);
		obj->getTransform()->setPosition(Vector3D(0, 0, 0));
		obj->setName("Floor");
		auto rb = obj->createComponent<RigidBodyComponent>();
		rb->setBodyType(RBType::Static);
		auto col = obj->createComponent<ColliderComponent>();
		col->setAsBox(mesh->getMesh()->getHalfExtents());
	}

	// sponza
	{
		auto obj = getWorld()->createGameObject<GameObject>();
		auto mesh = obj->createComponent<MeshComponent>();
		mesh->setMesh(sponza);
		mesh->addMaterial(archMat);
		mesh->addMaterial(brickMat);
		mesh->addMaterial(colAMat);
		mesh->addMaterial(colBMat);
		mesh->addMaterial(colCMat);
		mesh->addMaterial(flagPoleMat);
		obj->getTransform()->setPosition(Vector3D(0, 0, 0));
		obj->setName("Sponza");
	}
	// Player
	{
		auto obj = getWorld()->createGameObject<Player>();
		obj->getTransform()->setPosition(Vector3D(0, 0.5f, 0));
		obj->getTransform()->setScale(Vector3D(10, 10, 10));
		PrimitiveFactory::createCapsule(getResourceManager(), obj);
	}
}

void MainGame::onUpdate(f32 deltaTime) {
	Game::onUpdate(deltaTime);
	m_rotation += 1.57f * deltaTime;

	//m_gameObject->getTransform()->setRotation(Vector3D(-0.707f, m_rotation, 0));
}
