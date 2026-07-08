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

TextureManager* GraphicsEngine::getTextureManager() {
    return m_tex_manager;
}

GraphicsEngine::GraphicsEngine() {
    try {
        m_render_system = new RenderSystem();
    }
    catch (...) {
        throw std::exception("Render System did not initiate successfully");
    }

    try {
        m_tex_manager = new TextureManager();
    }
    catch (...) {
        throw std::exception("Texture manager did not initiate successfully");
    }
}

GraphicsEngine::~GraphicsEngine() {
    sharedInstance = nullptr;
    delete m_tex_manager;
    delete m_render_system;
}
