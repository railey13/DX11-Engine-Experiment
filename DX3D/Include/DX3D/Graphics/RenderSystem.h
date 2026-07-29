#pragma once
#include <d3d11.h> 
#include <wrl.h>
#include <DX3D/Prerequisites.h>
#include <DX3D/Graphics/Texture2D.h>

class RenderSystem {
public:
	RenderSystem();

	~RenderSystem();
public:
	SwapChainPtr createSwapChain(HWND hwnd, ui32  width, ui32  height);
	DeviceContextPtr getImmediateDeviceContext();
	VertexBufferPtr createVertexBuffer(void* list_vertices, ui32  size_vertex, ui32  size_list);
	IndexBufferPtr createIndexBuffer(void* list_indices, ui32  size_list);
	ConstantBufferPtr createConstantBuffer(void* buffer, ui32  size_buffer);
	VertexShaderPtr createVertexShader(const wchar_t* full_path, const char* entry_point);
	PixelShaderPtr createPixelShader(const wchar_t* full_path, const char* entry_point);
	Texture2DPtr createTexture(const wchar_t* full_path);
	Texture2DPtr createTexture(const Rect& size, Texture2D::Texture2DType type);

	void setCullMode(const CullMode& mode);
	ID3D11Device* getD11Device() { return m_d3d_device.Get(); }
private:
	void compilePrivateShaders();
	void initRasterizerStates();
private:
	DeviceContextPtr m_imm_device_context; 
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3d_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_imm_context;
private:
	Microsoft::WRL::ComPtr<IDXGIDevice> m_dxgi_device;
	Microsoft::WRL::ComPtr<IDXGIAdapter> m_dxgi_adapter;
	Microsoft::WRL::ComPtr<IDXGIFactory> m_dxgi_factory;
private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob = nullptr;

	Microsoft::WRL::ComPtr<ID3DBlob> m_vsblob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_psblob = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_ps = nullptr;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cull_front_state = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cull_back_state = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_cull_none_state = nullptr;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;
private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class GraphicsEngine;
	friend class Texture2D;
};

