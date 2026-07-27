#pragma once
#include <DX3D/Resource/Resource.h>
#include <DX3D/Math/Rect.h>
#include <d3d11.h>
#include <wrl.h>

class Texture2D {
public:
	enum Texture2DType {
		Normal = 0,
		RenderTarget,
		DepthStencil
	};
public:
	Texture2D(const wchar_t* full_path, RenderSystem* system);
	Texture2D(const Rect&size, Texture2DType type, RenderSystem* system);
	Rect getSize() { return m_size; }
private:
	Microsoft::WRL::ComPtr<ID3D11Resource> m_texture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shader_res_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler_state = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depth_stencil_view = nullptr;

	Texture2DType m_type = Normal;

	Rect m_size;

	RenderSystem* m_system = nullptr;
private:
	friend class DeviceContext;
};

