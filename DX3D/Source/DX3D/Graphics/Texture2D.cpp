#include <DX3D/Graphics/Texture2D.h>
#include <DirectXTex.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/RenderSystem.h>

Texture2D::Texture2D(const wchar_t* full_path, RenderSystem* system) : m_system(system) {
	
	DirectX::ScratchImage image_data;
	HRESULT res = DirectX::LoadFromWICFile(full_path, DirectX::WIC_FLAGS_NONE, nullptr, image_data);

	if (SUCCEEDED(res)) {
		res = DirectX::CreateTexture(
			m_system->m_d3d_device.Get(),
			image_data.GetImages(),
			image_data.GetImageCount(),
			image_data.GetMetadata(), 
			&m_texture);

		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = image_data.GetMetadata().format;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = image_data.GetMetadata().mipLevels;
		desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.MaxLOD = (FLOAT)image_data.GetMetadata().mipLevels;

		res = m_system->m_d3d_device->CreateSamplerState(&samplerDesc, &m_sampler_state);
		if (FAILED(res)) DX3DError("Texture not created successfully.");

		res = m_system->m_d3d_device->CreateShaderResourceView(m_texture.Get(), &desc, &m_shader_res_view);
		if (FAILED(res)) DX3DError("Texture not created successfully.");
	}
	else {
		DX3DError("Texture was not created successfully.");
	}

}

Texture2D::Texture2D(const Rect& size, Texture2DType type, RenderSystem* system) : m_system(system) {
	D3D11_TEXTURE2D_DESC tex_desc = {};

	tex_desc.Width = size.width;
	tex_desc.Height = size.height;
	
	switch (type) {
		case Normal: {
			tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			break;
		}
		case RenderTarget: {
			tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			break;
		}
		case DepthStencil: {
			tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			break;
		}
	}

	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;	

	auto hr = m_system->m_d3d_device->CreateTexture2D(&tex_desc, nullptr, (ID3D11Texture2D**)m_texture.GetAddressOf());

	if (FAILED(hr)) DX3DError("Texture not created successfully.");

	switch (type) {
		case RenderTarget: {
			hr = m_system->m_d3d_device->CreateShaderResourceView(this->m_texture.Get(), NULL, &this->m_shader_res_view);
			if (FAILED(hr)) DX3DError("Texture not created successfully.");

			hr = m_system->m_d3d_device->CreateRenderTargetView(this->m_texture.Get(), NULL, &this->m_render_target_view);
			if (FAILED(hr)) DX3DError("Texture not created successfully.");

			break;
		}
		case DepthStencil: {
			hr = m_system->m_d3d_device->CreateDepthStencilView(this->m_texture.Get(), NULL, &this->m_depth_stencil_view);
			if (FAILED(hr)) DX3DError("Texture not created successfully.");
			break;
		}
	}

	m_size = size;
	m_type = type;
} 