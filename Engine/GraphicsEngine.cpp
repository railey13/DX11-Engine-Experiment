#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <d3dcompiler.h>

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

GraphicsEngine::GraphicsEngine() {

}

bool GraphicsEngine::init() {
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
                                NULL,NULL, 
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

    m_imm_device_context = new DeviceContext(m_imm_context);

    // to create a swapchain, get the dxgi factory first to call CreateSwapChain
    // note: DXGI is the DirectX Graphics Infrastracture that handles low level tasks like swap chain management
    m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device); // get the dxgi device instance from the Direct3D device
    m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter); // get the dxgi adapter instance
    m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory),(void**)&m_dxgi_factory); // get the dxgi factory instance

    return true;
}

bool GraphicsEngine::release() {
    m_dxgi_device->Release();
    m_dxgi_adapter->Release();
    m_dxgi_factory->Release();

    m_imm_device_context->release();
    m_d3d_device->Release();
    return true;
}

GraphicsEngine::~GraphicsEngine() {

}


SwapChain* GraphicsEngine::createSwapChain() {

    return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext() {

    return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer() {

    return new VertexBuffer();
}

IndexBuffer* GraphicsEngine::createIndexBuffer() {

    return new IndexBuffer();;
}

ConstantBuffer* GraphicsEngine::createConstantBuffer() {

    return new ConstantBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(void* shader_byte_code, size_t byte_code_size) {

    VertexShader* vs = new VertexShader();

    if (!vs->init(shader_byte_code, byte_code_size)) {
        vs->release();
        return nullptr;
    }

    return vs;
}

PixelShader* GraphicsEngine::createPixelShader(void* shader_byte_code, size_t byte_code_size) {

    PixelShader* ps = new PixelShader();

    if (!ps->init(shader_byte_code, byte_code_size)) {
        ps->release();
        return nullptr;
    }

    return ps;
}

bool GraphicsEngine::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size) {
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

bool GraphicsEngine::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size) {
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

void GraphicsEngine::releaseCompiledShader() {
    if (m_blob) {
        m_blob->Release();
    }
}