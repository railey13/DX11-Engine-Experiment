#include <DX3D/Graphics/VertexShader.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <d3dcompiler.h>
#include <exception>

VertexShader::VertexShader(const wchar_t* full_path, const char* entry_point, RenderSystem* system) : m_system(system) {
    Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
    Microsoft::WRL::ComPtr<ID3DBlob> error_blob = nullptr;
    
    D3DCompileFromFile(full_path, nullptr, nullptr, entry_point, "vs_5_0", 0, 0, &blob, &error_blob);

    if (error_blob) {
        DX3DWarning("VertexShader " << full_path << " compiled with errors: \n" << (char*)error_blob->GetBufferPointer());
    }
    if (!blob) {
        DX3DError("VertexShader " << full_path << " not created successfully");
    }
	if (FAILED(m_system->m_d3d_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vs))) {
        DX3DError("VertexShader " << full_path << " not created successfully");
	}
}

