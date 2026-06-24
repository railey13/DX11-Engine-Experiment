#include "GraphicsEngine.h"
#include <exception>

GraphicsEngine* GraphicsEngine::sharedInstance = NULL;

GraphicsEngine* GraphicsEngine::get() {
    return sharedInstance;
}

void GraphicsEngine::initialize() {
    if(sharedInstance != NULL) throw std::exception("Graphcis Engine already exists");

    sharedInstance = new GraphicsEngine();
}

void GraphicsEngine::destroy() {
    if (sharedInstance == NULL) return;

    delete sharedInstance;
}

RenderSystem* GraphicsEngine::getRenderSystem() {
    return m_render_system;
}

GraphicsEngine::GraphicsEngine() {
    try {
        m_render_system = new RenderSystem();
    }
    catch (...) {
        throw std::exception("Graphcis Engine did not initiate successfully");
    }
}

GraphicsEngine::~GraphicsEngine() {
    sharedInstance = nullptr;
    delete m_render_system;
}
