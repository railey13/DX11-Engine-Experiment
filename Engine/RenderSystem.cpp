#include "RenderSystem.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <d3dcompiler.h>
#include <exception>

RenderSystem::RenderSystem() {

}

bool RenderSystem::init() {
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
    UINT num_driver_types = ARRAYSIZE(driver_types);

    D3D_FEATURE_LEVEL feature_levels[] = {
            D3D_FEATURE_LEVEL_11_0
    };

    UINT num_feature_levels = ARRAYSIZE(feature_levels);

    HRESULT res = 0;

    for (UINT driver_type_index = 0; driver_type_index < num_driver_types;) {
        res = D3D11CreateDevice(NULL,
            driver_types[driver_type_index],  // HARDWARE->WARP->REFERENCE
            NULL, NULL,
            feature_levels, num_feature_levels,
            D3D11_SDK_VERSION,
            &m_d3d_device, &m_feature_level, &m_imm_context
        );

        if (SUCCEEDED(res)) {
            break;
        }

        driver_type_index++;

    }

    if (FAILED(res)) {
        return false;
    }

    m_imm_device_context = new DeviceContext(m_imm_context, this);

    // to create a swapchain, get the dxgi factory first to call CreateSwapChain
    // note: DXGI is the DirectX Graphics Infrastracture that handles low level tasks like swap chain management
    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device); // get the dxgi device instance from the Direct3D device
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter); // get the dxgi adapter instance
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory); // get the dxgi factory instance

    return true;
}

bool RenderSystem::release() {
    m_dxgi_device->Release();
    m_dxgi_adapter->Release();
    m_dxgi_factory->Release();

    delete m_imm_device_context;

    m_d3d_device->Release();
    return true;
}

RenderSystem::~RenderSystem() {

}


SwapChain* RenderSystem::createSwapChain(HWND hwnd, UINT width, UINT height) {
    SwapChain* sc = nullptr;

    try {
        sc = new SwapChain(hwnd, width, height, this);
    }
    catch (...) {

    }

    return sc;
}

DeviceContext* RenderSystem::getImmediateDeviceContext() {

    return this->m_imm_device_context;
}

VertexBuffer* RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader) {
    VertexBuffer* vb = nullptr;

    try {
        vb = new VertexBuffer(list_vertices, size_vertex, size_list, shader_byte_code, size_byte_shader, this);
    }
    catch (...) {

    }

    return vb;
}

IndexBuffer* RenderSystem::createIndexBuffer(void* list_indices, UINT size_list) {
    IndexBuffer* ib = nullptr;
    try {
        ib = new IndexBuffer(list_indices, size_list, this);
    }
    catch (...) {

    }

    return ib;
}

ConstantBuffer* RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer) {
    ConstantBuffer* cb = nullptr;
    try {
        cb = new ConstantBuffer(buffer, size_buffer, this);
    }
    catch (...) {

    }
    
    return cb; 
}

VertexShader* RenderSystem::createVertexShader(void* shader_byte_code, size_t byte_code_size) {
    VertexShader* vs = nullptr;
    try {
        vs = new VertexShader(shader_byte_code, byte_code_size, this);
    }
    catch (...) {

    }

    return vs;
}

PixelShader* RenderSystem::createPixelShader(void* shader_byte_code, size_t byte_code_size) {
    PixelShader* ps = nullptr;
    try {
        ps = new PixelShader(shader_byte_code, byte_code_size, this);
    }
    catch (...) {

    }

    return ps;
}

bool RenderSystem::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size) {
    ID3DBlob* error_blob = nullptr;
    if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob))) {
        if (error_blob) {
            error_blob->Release();
        }
        return false;
    }

    *shader_byte_code = m_blob->GetBufferPointer();
    *byte_code_size = m_blob->GetBufferSize();

    return true;
}

bool RenderSystem::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size) {
    ID3DBlob* error_blob = nullptr;
    if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob))) {
        if (error_blob) {
            error_blob->Release();
        }
        return false;
    }

    *shader_byte_code = m_blob->GetBufferPointer();
    *byte_code_size = m_blob->GetBufferSize();

    return true;
}

void RenderSystem::releaseCompiledShader() {
    if (m_blob) {
        m_blob->Release();
    }
}