#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Game/Display.h>

#include <DX3D/User Interface/UIHandler.h>

#include <DX3D/Resource/Mesh.h>
#include <DX3D/Resource/Material.h>

#include <DX3D/GameObject/MeshComponent.h>
#include <DX3D/GameObject/TransformComponent.h>
#include <DX3D/GameObject/CameraComponent.h>
#include <DX3D/GameObject/LightComponent.h>
#include <DX3D/GameObject/GameObject.h>

#include <DX3D/Math/Matrix4x4.h>
#include <DX3D/Math/Vector4D.h>

__declspec(align(16))
struct LightData
{
    Vector4D color;
    Vector4D direction;
    Vector4D position;
    f32 radius;
    LightType type;
};

__declspec(align(16))
struct constant {
    Matrix4x4 m_world;
    Matrix4x4 m_view;
    Matrix4x4 m_proj;
    Vector4D cameraPosition;
    LightData lights[10];
    i32 num_active_lights;
};
    
GraphicsEngine::GraphicsEngine(Game* game) : m_game(game){
    m_render_system = std::make_unique<RenderSystem>();
}

GraphicsEngine::~GraphicsEngine() {

}

void GraphicsEngine::update() {
    auto swapChain = m_game->m_display->m_swapChain;

    auto context = m_render_system->getImmediateDeviceContext();

    context->ClearRenderTargetColor(swapChain, 0.55f, 0.68f, 0.76f, 1);

    auto winSize = m_game->m_display->getClientSize();

    context->setViewportSize(winSize.width, winSize.height);

    constant cc = {};

    if (m_activeCamera) {
        cc.cameraPosition = m_activeCamera->getGameObject()->getTransform()->getPosition();
        m_activeCamera->setScreenArea(winSize);
        m_activeCamera->getViewMatrix(cc.m_view);
        m_activeCamera->getProjectionMatrix(cc.m_proj);
    }

   //for (auto c : m_cameras) {
   //    c->setScreenArea(winSize);
   //    c->getViewMatrix(cc.m_view);
   //    c->getProjectionMatrix(cc.m_proj);
   //}
    i32 i = 0;
    for (auto l : m_lights) {
        auto t = l->getGameObject()->getTransform();
        Matrix4x4 world;
        t->getWorldMatrix(world);

        cc.lights[i].direction = world.getZDirection();
        cc.lights[i].color = l->getColor();
        cc.lights[i].position = l->getGameObject()->getTransform()->getPosition();
        cc.lights[i].type = l->getLightType();
        cc.lights[i].radius = l->getRadius();

        i++;
    }
    cc.num_active_lights = i;

   for (auto m : m_meshes) {
       auto transform = m->getGameObject()->getTransform();
       transform->getWorldMatrix(cc.m_world);

       auto mesh = m->getMesh().get();
       const auto materials = m->getMaterials();

       context->setVertexBuffer(mesh->m_vertex_buffer);
       context->setIndexBuffer(mesh->m_index_buffer);

       for (auto i = 0; i < mesh->getNumMaterialSlots(); i++) {

           if (i >= materials.size()) break;
           auto mat = materials[i].get();

           m_render_system->setCullMode(mat->getCullMode());

           mat->setData(&cc, sizeof(constant));

           context->setConstantBuffer(mat->m_constant_buffer);
           context->setVertexShader(mat->m_vertex_shader);
           context->setPixelShader(mat->m_pixel_shader);
           context->setTexutre(&mat->m_vec_textures[0], (ui32)mat->m_vec_textures.size());

           auto slot = mesh->getMaterialSlot(i);

           context->drawIndexedTriangleList((ui32)slot.num_indices, (ui32)slot.start_index, 0);
       }
   }

   m_game->m_uiHandler->draw();

   swapChain->present(true);
}

RenderSystem* GraphicsEngine::getRenderSystem() {
    return m_render_system.get();
}

void GraphicsEngine::addComponent(Component* component) {
    if (auto c = dynamic_cast<MeshComponent*>(component))
        m_meshes.emplace(c);
    else if (auto c = dynamic_cast<CameraComponent*>(component))
        m_cameras.emplace(c);
    else if (auto c = dynamic_cast<LightComponent*>(component))
        m_lights.emplace(c);
}

void GraphicsEngine::removeComponent(Component* component) {
    if (auto c = dynamic_cast<MeshComponent*>(component))
        m_meshes.erase(c);
    else if (auto c = dynamic_cast<CameraComponent*>(component))
        m_cameras.erase(c);
    else if (auto c = dynamic_cast<LightComponent*>(component))
        m_lights.erase(c);
}
