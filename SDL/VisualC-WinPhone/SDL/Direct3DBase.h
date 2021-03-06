#pragma once
#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>

#include "DirectXHelper.h"

// 为 3D 呈现初始化 DirectX API 的帮助器类。
namespace SDL_Render
{
	ref class Direct3DBase abstract
	{
	internal:
		Direct3DBase();

	public:
		virtual void Initialize();
		virtual void CreateDeviceResources();
		virtual void CreateWindowSizeDependentResources();
		virtual void UpdateForRenderResolutionChange(float width, float height);
		virtual void UpdateForWindowSizeChange(float width, float height);
		virtual void Render() = 0;

	internal:
		virtual ID3D11Texture2D* GetTexture()
		{
			return m_renderTarget.Get();
		}

	protected private:
		// Direct3D 对象。
		Microsoft::WRL::ComPtr<ID3D11Device1> m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> m_d3dContext;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTarget;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

		// 缓存的呈现器属性。
		D3D_FEATURE_LEVEL m_featureLevel;
		Windows::Foundation::Size m_renderTargetSize;
		Windows::Foundation::Rect m_windowBounds;
	};
}