#include "DX3D/Game/Game.h"
#include "DX3D/Game/Display.h"
#include "DX3D/Game/World.h"
#include <DX3D/Window/Window.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Texture.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Game/EditorCamera.h>
#include <DX3D/User Interface/UIHandler.h>
#include <DX3D/Commands/CommandInvoker.h>
#include <DX3D/Commands/SpawnObjectCommand.h>
#include <DX3D/Commands/DeleteObjectCommand.h>
#include <DX3D/Resource/PrimitiveFactory.h>

Game::Game() {
	m_graphicsEngine = std::make_unique<GraphicsEngine>(this);
	m_display = std::make_unique<Display>(this);
	m_input = std::make_unique<InputSystem>();
	m_resourceManager = std::make_unique<ResourceManager>(this);
	m_world = std::make_unique<World>(this);
	m_uiHandler = std::make_unique<UIHandler>(this, static_cast<HWND>(m_display->getHwnd()));
	m_editorCamera = std::make_unique<EditorCamera>();
	m_editorCamera->m_world = m_world.get();
	m_editorCamera->onCreate();
	m_editorCamera->getTransform()->setPosition(Vector3D(0, 2, 0));
	m_commandInvoker = std::make_unique<CommandInvoker>();
	bindCommands();

	m_input->setLockArea(m_display->getClientSize());
}

Game::~Game() {

}

void Game::onDisplaySize(const Rect& size) {
	m_input->setLockArea(m_display->getClientSize());
	onInternalUpdate();
}

void Game::onInternalUpdate() {
	auto currentTime = std::chrono::system_clock::now();
	auto elapsedSeconds = std::chrono::duration<double>();

	if (m_previous_time.time_since_epoch().count()) {
		elapsedSeconds = currentTime - m_previous_time;
	}

	m_previous_time = currentTime;

	auto deltaTime = (f32)elapsedSeconds.count();

	m_input->update();

	if (m_input->isKeyDown(Key::L_Ctrl)) {
		if (m_input->isKeyUp(Key::Z)) {
			m_commandInvoker->undo();
		}
		else if (m_input->isKeyUp(Key::Y)) {
			m_commandInvoker->redo();
		}
	}
	if (m_input->isKeyUp(Key::Delete)) {
		if(m_world->getSelectedGameObject())
			m_commandInvoker->executeBoundCommand(Action::DeleteObject);
	}

	onUpdate(deltaTime);
	m_world->update(deltaTime);

	m_editorCamera->update(deltaTime); 
	m_graphicsEngine->setActiveCamera(m_editorCamera->getComponent<CameraComponent>());

	m_graphicsEngine->update();
}

void Game::bindCommands() {
	m_commandInvoker->bindCommand(Action::DeleteObject, [this]() {
		return std::make_unique<DeleteObjectCommand>(m_world.get(), m_world->getSelectedGameObject()->getInstanceID());
		});

	m_commandInvoker->bindCommand(Action::SpawnCube, [this]() {
		return std::make_unique<SpawnObjectCommand<GameObject>>(m_world.get(), [this](GameObject* obj) {
			PrimitiveFactory::createCube(m_resourceManager.get(), obj);
			});
		});

	m_commandInvoker->bindCommand(Action::SpawnSphere, [this]() {
		return std::make_unique<SpawnObjectCommand<GameObject>>(m_world.get(), [this](GameObject* obj) {
			PrimitiveFactory::createSphere(m_resourceManager.get(), obj);
			});
		});

	m_commandInvoker->bindCommand(Action::SpawnCapsule, [this]() {
		return std::make_unique<SpawnObjectCommand<GameObject>>(m_world.get(), [this](GameObject* obj) {
			PrimitiveFactory::createCapsule(m_resourceManager.get(), obj);
			});
		});

	m_commandInvoker->bindCommand(Action::SpawnPlane, [this]() {
		return std::make_unique<SpawnObjectCommand<GameObject>>(m_world.get(), [this](GameObject* obj) {
			PrimitiveFactory::createPlane(m_resourceManager.get(), obj);
			});
		});
}

void Game::quit() {
	m_isRunning = false;
}
