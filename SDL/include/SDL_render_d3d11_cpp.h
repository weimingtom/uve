/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#define RENDER_D3D11
#include "SDL_config.h"
#include <SDL_pixels.h>
#include <D3D11_1.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>
#include <d3d11_1.h>
#include <memory>
#include <agile.h>
#include "../src/render/SDL_sysrender.h"
//#include "F:\Documents\Open Source Projects\DavidLudwig-sdl-25883bdf3cab\VisualC-WinPhone\SDL\Direct3DBase.h"
//#include <F:\Documents\Open Source Projects\DavidLudwig-sdl-25883bdf3cab\VisualC-WinPhone\SDL\DirectXHelper.h>
#include <mutex>
#include "SDL.h"




struct SDL_VertexShaderConstants
{
    DirectX::XMFLOAT4X4 model;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 projection;
};
struct ModelViewProjectionConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};
struct Vertex	//Overloaded Vertex Structure
{
	Vertex(){}
	Vertex(float x, float y, float z,
		float u, float v)
		: pos(x,y,z), texCoord(u, v){}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texCoord;
};

typedef struct 
{
    Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> texturePixelShader;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backbuffer;
    D3D_FEATURE_LEVEL featureLevel;
}SharedData;
typedef struct
{
    Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mainRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> currentOffscreenRenderTargetView;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> texturePixelShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> colorPixelShader;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeBlend;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeAdd;
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendModeMod;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> mainSampler;
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> mainRasterizer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_backbuffer;
    D3D_FEATURE_LEVEL featureLevel;

    // Vertex buffer constants:
    SDL_VertexShaderConstants vertexShaderConstantsData;
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexShaderConstants;

    // Cached renderer properties.
    DirectX::XMFLOAT2 windowSizeInDIPs;
    DirectX::XMFLOAT2 renderTargetSize;
    Windows::Graphics::Display::DisplayOrientations orientation;

    // Transform used for display orientation.
    DirectX::XMFLOAT4X4 orientationTransform3D;
} D3D11_RenderData;

typedef struct
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> mainTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mainTextureResourceView;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mainTextureRenderTargetView;
    SDL_PixelFormat * pixelFormat;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
    DirectX::XMINT2 lockedTexturePosition;
} D3D11_TextureData;

struct VertexPositionColor
{
    DirectX::XMFLOAT3 pos;
    DirectX::XMFLOAT2 tex;
    DirectX::XMFLOAT4 color;
};



extern DECLSPEC  SDL_Renderer  *D3D11_CreateRenderer(SDL_Window * window, Uint32 flags);
extern DECLSPEC void  D3D11_WindowEvent(SDL_Renderer * renderer,
                            const SDL_WindowEvent *event);
extern DECLSPEC int D3D11_CreateTexture(SDL_Renderer * renderer, SDL_Texture * texture);
extern DECLSPEC int D3D11_UpdateTexture(SDL_Renderer * renderer, SDL_Texture * texture,
                             const SDL_Rect * rect, const void *srcPixels,
                             int srcPitch);
extern DECLSPEC int D3D11_LockTexture(SDL_Renderer * renderer, SDL_Texture * texture,
                             const SDL_Rect * rect, void **pixels, int *pitch);
extern DECLSPEC void D3D11_UnlockTexture(SDL_Renderer * renderer, SDL_Texture * texture);
extern DECLSPEC int D3D11_SetRenderTarget(SDL_Renderer * renderer, SDL_Texture * texture);
extern DECLSPEC int D3D11_UpdateViewport(SDL_Renderer * renderer);
extern DECLSPEC int D3D11_RenderClear(SDL_Renderer * renderer);
extern DECLSPEC int D3D11_RenderDrawPoints(SDL_Renderer * renderer,
                                  const SDL_FPoint * points, int count);
extern  DECLSPEC int D3D11_RenderDrawLines(SDL_Renderer * renderer,
                                 const SDL_FPoint * points, int count);
extern DECLSPEC int D3D11_RenderFillRects(SDL_Renderer * renderer,
                                 const SDL_FRect * rects, int count);
extern DECLSPEC int D3D11_RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture,
                            const SDL_Rect * srcrect, const SDL_FRect * dstrect);
extern DECLSPEC int D3D11_RenderCopyEx(SDL_Renderer * renderer, SDL_Texture * texture,
                              const SDL_Rect * srcrect, const SDL_FRect * dstrect,
                              const double angle, const SDL_FPoint * center, const SDL_RendererFlip flip);
extern DECLSPEC int D3D11_RenderReadPixels(SDL_Renderer * renderer, const SDL_Rect * rect,
                                  Uint32 format, void * pixels, int pitch);
extern DECLSPEC void D3D11_RenderPresent(SDL_Renderer * renderer);
extern DECLSPEC void D3D11_DestroyTexture(SDL_Renderer * renderer,
                                 SDL_Texture * texture);
extern DECLSPEC void D3D11_DestroyRenderer(SDL_Renderer * renderer);
extern DECLSPEC  int D3D11_UpdateVertexBuffer(SDL_Renderer *renderer,const void * vertexData, unsigned int dataSizeInBytes);
extern DECLSPEC void D3D11_RenderStartDrawOp(SDL_Renderer * renderer);
extern DECLSPEC void
D3D11_SetPixelShader(SDL_Renderer * renderer,
                     ID3D11PixelShader * shader,
                     ID3D11ShaderResourceView * shaderResource,
                     ID3D11SamplerState * sampler);
extern DECLSPEC void
D3D11_RenderFinishDrawOp(SDL_Renderer * renderer,
                         D3D11_PRIMITIVE_TOPOLOGY primitiveTopology,
                         UINT vertexCount);

/* Direct3D 11.1 Internal Functions */
extern DECLSPEC HRESULT D3D11_CreateDeviceResources(SDL_Renderer * renderer);
extern DECLSPEC HRESULT D3D11_CreateWindowSizeDependentResources(SDL_Renderer * renderer);
extern DECLSPEC HRESULT D3D11_UpdateForWindowSizeChange(SDL_Renderer * renderer);
extern DECLSPEC HRESULT D3D11_HandleDeviceLost(SDL_Renderer * renderer);


namespace SDL_Render
{
	
}

/* vi: set ts=4 sw=4 expandtab: */
