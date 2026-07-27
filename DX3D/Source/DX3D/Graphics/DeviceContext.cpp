#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/VertexShader.h>
#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Graphics/Texture2D.h>
//#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* system) : m_device_context(device_context), m_system(system){

}

void DeviceContext::ClearRenderTargetColor(const SwapChainPtr& swap_chain, f32 red, f32 green, f32 blue, f32 alpha) {
	f32 clear_color[] = {red, green, blue, alpha};

	auto rtv = swap_chain->m_rtv.Get();
	auto dsv = swap_chain->m_dsv.Get();

	m_device_context->ClearRenderTargetView(rtv, clear_color);
	m_device_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &rtv, dsv);
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertex_buffer) {
	ui32 stride = vertex_buffer->m_size_vertex;
	ui32 offset = 0;
	auto buffer = vertex_buffer->m_buffer.Get();
		
	m_device_context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

	m_device_context->IASetInputLayout(vertex_buffer->m_layout.Get());
}

void DeviceContext::setIndexBuffer(const IndexBufferPtr& index_buffer) {
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::setViewportSize(ui32  width, ui32  height) {
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(const VertexShaderPtr& vertex_shader) {
	m_device_context->VSSetShader(vertex_shader->m_vs.Get(), nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShaderPtr& pixel_shader) {
	m_device_context->PSSetShader(pixel_shader->m_ps.Get(), nullptr, 0);
}

void DeviceContext::setTexutre(const Texture2DPtr* texture, ui32 num_textures) {
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];

	for (ui32 i = 0; i < num_textures;i++) {
		list_res[i] = texture[i]->m_shader_res_view.Get();
		list_sampler[i] = texture[i]->m_sampler_state.Get();
	}

	m_device_context->VSSetShaderResources(0, num_textures, list_res);
	m_device_context->VSSetSamplers(0, num_textures, list_sampler);

	m_device_context->PSSetShaderResources(0, num_textures, list_res);
	m_device_context->PSSetSamplers(0, num_textures, list_sampler);

}

void DeviceContext::setConstantBuffer(const ConstantBufferPtr& constant_buffer) {
	auto buffer = constant_buffer->m_buffer.Get();
	m_device_context->VSSetConstantBuffers(0, 1, &buffer);
	m_device_context->PSSetConstantBuffers(0, 1, &buffer);
}

void DeviceContext::drawTriangleList(ui32  vertex_count, ui32  start_vertex_index) {
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(ui32  index_count, ui32  start_vertex_index, ui32  start_index_location) {
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(ui32  vertex_count, ui32  start_vertex_index){
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

ID3D11DeviceContext* DeviceContext::getContext() {

	return m_device_context.Get();
}