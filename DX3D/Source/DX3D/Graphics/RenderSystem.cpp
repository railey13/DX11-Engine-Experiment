#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/VertexShader.h>
#include <DX3D/Graphics/PixelShader.h>

#include <d3dcompiler.h>
#include <exception>

RenderSystem::RenderSystem() {
    // vector of driver types from best -> worst
       // HARDWARE does drawing calls mainly happens on the GPU
       // WARP does the drawing calls mainly happens on the CPU
       // REFERENCE does the drawing calls on the CPU but slower compared to the other two
    D3D_DRIVER_TYPE driver_types[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };

    // get the vector size  
    ui32  num_driver_types = ARRAYSIZE(driver_types);

    D3D_FEATURE_LEVEL feature_levels[] = {
            D3D_FEATURE_LEVEL_11_0
    };

    ui32  num_feature_levels = ARRAYSIZE(feature_levels);

    HRESULT res = 0;

    D3D_FEATURE_LEVEL featureLevel = {};

    for (ui32  driver_type_index = 0; driver_type_index < num_driver_types;) {
        res = D3D11CreateDevice(NULL,
            driver_types[driver_type_index],  // HARDWARE->WARP->REFERENCE
            NULL, NULL,
            feature_levels, num_feature_levels,
            D3D11_SDK_VERSION,
            &m_d3d_device, &featureLevel, &m_imm_context
        );

        if (SUCCEEDED(res)) {
            break;
        }

        driver_type_index++;

    }

    if (FAILED(res)) {
        DX3DError("Render System did not initiate successfully");
    }

    m_imm_device_context = std::make_shared<DeviceContext>(m_imm_context.Get(), this);

    // to create a swapchain, get the dxgi factory first to call CreateSwapChain
    // note: DXGI is the DirectX Graphics Infrastracture that handles low level tasks like swap chain management
    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device); // get the dxgi device instance from the Direct3D device
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter); // get the dxgi adapter instance
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory); // get the dxgi factory instance

    initRasterizerStates();
    compilePrivateShaders();
}

RenderSystem::~RenderSystem() {

}

SwapChainPtr RenderSystem::createSwapChain(HWND hwnd, ui32  width, ui32  height) {
    return std::make_shared<SwapChain>(hwnd, width, height, this);
}

DeviceContextPtr RenderSystem::getImmediateDeviceContext() {
    return this->m_imm_device_context;
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, ui32  size_vertex, ui32  size_list) {
    return std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, this);
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, ui32  size_list) {
    return std::make_shared<IndexBuffer>(list_indices, size_list, this);
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, ui32  size_buffer) {
    return std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
}

VertexShaderPtr RenderSystem::createVertexShader(const wchar_t* full_path, const char* entry_point) {
    return std::make_shared<VertexShader>(full_path, entry_point, this);
}

PixelShaderPtr RenderSystem::createPixelShader(const wchar_t* full_path, const char* entry_point) {
    return std::make_shared<PixelShader>(full_path, entry_point, this);
}

Texture2DPtr RenderSystem::createTexture(const wchar_t* full_path) {
    return std::make_shared<Texture2D>(full_path, this);
}

Texture2DPtr RenderSystem::createTexture(const Rect& size, Texture2D::Texture2DType type) {
    return std::make_shared<Texture2D>(size, type, this);
}

void RenderSystem::setCullMode(const CullMode& mode) {
    if (mode == CullMode::Front) {
        m_imm_context->RSSetState(m_cull_front_state.Get());
    }
    else if (mode == CullMode::Back) {
        m_imm_context->RSSetState(m_cull_back_state.Get());
    }
    else if (mode == CullMode::None) {
        m_imm_context->RSSetState(m_cull_none_state.Get());
    }
}

void RenderSystem::compilePrivateShaders() {
    Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

    auto meshLayoutCode = R"(
        struct VS_INPUT
        {
            float4 position : POSITION0;
            float2 texcoord : TEXCOORD0;
            float3 normal : NORMAL0;
	        float3 tangent: TANGENT0;
	        float3 binormal: BINORMAL0;
        };

        struct VS_OUTPUT
        {
            float4 position : SV_POSITION0;
            float2 texcoord : TEXCOORD0;
        };
    
        VS_OUTPUT vsmain(VS_INPUT input)
        {
            VS_OUTPUT output = (VS_OUTPUT) 0;
            return output;
        }
    )";

    auto codeSize = strlen(meshLayoutCode);

    if (FAILED(D3DCompile(meshLayoutCode, codeSize, "VertexMeshLayoutShader", nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &error_blob))) {
        DX3DError("VertexMeshLayoutShader did not compile successfully.");
    }

    memcpy(m_mesh_layout_byte_code, blob->GetBufferPointer(), blob->GetBufferSize());
    m_mesh_layout_size = blob->GetBufferSize();
}

void RenderSystem::initRasterizerStates() {
    D3D11_RASTERIZER_DESC desc = {};
    desc.DepthClipEnable = true;
    desc.FillMode = D3D11_FILL_SOLID;
    desc.FrontCounterClockwise = true;

    desc.CullMode = D3D11_CULL_FRONT;
    m_d3d_device->CreateRasterizerState(&desc, &m_cull_front_state);
    desc.CullMode = D3D11_CULL_BACK;
    m_d3d_device->CreateRasterizerState(&desc, &m_cull_back_state);
    desc.CullMode = D3D11_CULL_NONE;
    m_d3d_device->CreateRasterizerState(&desc, &m_cull_none_state);
}