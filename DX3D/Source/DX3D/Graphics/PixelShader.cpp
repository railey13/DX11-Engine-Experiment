#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <d3dcompiler.h>
#include <exception>

PixelShader::PixelShader(const wchar_t* full_path, const char* entry_point, RenderSystem* system) : m_system(system) {
    Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;

    D3DCompileFromFile(full_path, nullptr, nullptr, entry_point, "ps_5_0", 0, 0, &blob, &error_blob);

    if (error_blob) {
        DX3DWarning("PixelShader " << full_path << " compiled with errors: \n" << (char*)error_blob->GetBufferPointer());
    }
    if (!blob) {
        DX3DError("PixelShader " << full_path << " not created successfully");
    }
    if (FAILED(m_system->m_d3d_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_ps))) {
        DX3DError("PixelShader " << full_path << " not created successfully");
    }
}
