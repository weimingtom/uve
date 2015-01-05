
#include <pch.h>
#include "Direct3DBase.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;

// ���캯����
Direct3DBase::Direct3DBase()
{
}

// ��ʼ����������� Direct3D ��Դ��
void Direct3DBase::Initialize()
{
	CreateDeviceResources();
}

// �����������豸����Դ��
void Direct3DBase::CreateDeviceResources()
{
	// �˱�־Ϊ�� API Ĭ�����þ��в�ͬ��ɫ����˳���ͼ��
	// ���֧�֡�Ҫ�� Direct2D ���ݣ����������Ҫ��
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	// �����Ŀ���ڵ������ɹ����У���ͨ�����д˱�־�� SDK �����õ��ԡ�
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// �����鶨���Ӧ�ó���֧�ֵ� DirectX Ӳ�����ܼ����顣
	// ��ע�⣬Ӧ����˳��
	// �벻Ҫ������Ӧ�ó����˵���������������
	// ��͹��ܼ��𡣳�������˵��������ٶ�����Ӧ�ó����֧�� 9.3��
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_9_3
	};

	// ���� Direct3D 11 API �豸����Ͷ�Ӧ�������ġ�
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	DX::ThrowIfFailed(
		D3D11CreateDevice(
			nullptr, // ָ�� nullptr ��ʹ��Ĭ����������
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags, // ���õ��Ժ� Direct2D �����Ա�־��
			featureLevels, // ��Ӧ�ó������֧�ֵĹ��ܼ�����б�
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION, // ʼ�ս�������Ϊ D3D11_SDK_VERSION��
			&device, // ���ش����� Direct3D �豸��
			&m_featureLevel, // �����������豸�Ĺ��ܼ���
			&context // �����豸�ļ�ʱ�����ġ�
			)
		);

	// ��ȡ Direct3D 11.1 API �豸�������Ľӿڡ�
	DX::ThrowIfFailed(
		device.As(&m_d3dDevice)
		);

	DX::ThrowIfFailed(
		context.As(&m_d3dContext)
		);
}

// �����������ڴ��ڴ�С�������ڴ���Դ��
void Direct3DBase::CreateWindowSizeDependentResources()
{
	// Ϊ����Ŀ�껺����������������
	CD3D11_TEXTURE2D_DESC renderTargetDesc(
		DXGI_FORMAT_B8G8R8A8_UNORM,
		static_cast<UINT>(m_renderTargetSize.Width),
		static_cast<UINT>(m_renderTargetSize.Height),
		1,
		1,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		);
	renderTargetDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX | D3D11_RESOURCE_MISC_SHARED_NTHANDLE;

	// �� 2-D ͼ����Ϊ����Ŀ�껺�������䡣
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
			&renderTargetDesc,
			nullptr,
			&m_renderTarget
			)
		);

	DX::ThrowIfFailed(
		m_d3dDevice->CreateRenderTargetView(
			m_renderTarget.Get(),
			nullptr,
			&m_renderTargetView
			)
		);

	// �������ģ����ͼ��
	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT>(m_renderTargetSize.Width),
		static_cast<UINT>(m_renderTargetSize.Height),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
		);

	ComPtr<ID3D11Texture2D> depthStencil;
	DX::ThrowIfFailed(
		m_d3dDevice->CreateTexture2D(
			&depthStencilDesc,
			nullptr,
			&depthStencil
			)
		);

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateDepthStencilView(
			depthStencil.Get(),
			&depthStencilViewDesc,
			&m_depthStencilView
			)
		);

	// ��������ȷ���������ڵĳ���������
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		m_renderTargetSize.Width,
		m_renderTargetSize.Height
		);

	m_d3dContext->RSSetViewports(1, &viewport);
}

void Direct3DBase::UpdateForRenderResolutionChange(float width, float height)
{
	m_renderTargetSize.Width = width;
	m_renderTargetSize.Height = height;

	ID3D11RenderTargetView* nullViews[] = {nullptr};
	m_d3dContext->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
	m_renderTarget = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilView = nullptr;
	m_d3dContext->Flush();
	CreateWindowSizeDependentResources();
}

void Direct3DBase::UpdateForWindowSizeChange(float width, float height)
{
	m_windowBounds.Width  = width;
	m_windowBounds.Height = height;
}
#include <pch.h>