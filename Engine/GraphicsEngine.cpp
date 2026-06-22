#include "GraphicsEngine.h"

GraphicsEngine* GraphicsEngine::sharedInstance = NULL;

GraphicsEngine* GraphicsEngine::get() {
    return sharedInstance;
}

void GraphicsEngine::initialize() {
    sharedInstance = new GraphicsEngine();
    sharedInstance->init();
}

void GraphicsEngine::destroy() {
    if (sharedInstance != NULL) {
        sharedInstance->release();
    }
}

RenderSystem* GraphicsEngine::getRenderSystem() {
    return m_render_system;
}

GraphicsEngine::GraphicsEngine() {

}

bool GraphicsEngine::init() {
    m_render_system = new RenderSystem();
    m_render_system->init();
    return true;
}

bool GraphicsEngine::release() {
    m_render_system->release();
    delete m_render_system;
    return true;
}

GraphicsEngine::~GraphicsEngine() {

}
