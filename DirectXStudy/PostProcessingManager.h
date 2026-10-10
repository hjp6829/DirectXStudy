#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <memory>
#include <wrl/client.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

class Texture;
class VertexShader;
class PixelShader;
class Object;
class DirectXMain;

class PostProcessingManager
{
public:
	PostProcessingManager(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pContext);
	void PostProcessingRender();
	void OutlineRender(Object* currentObject, DirectXMain* dxdMain);
private:
	ComPtr<ID3D11RenderTargetView> pOutlineRenderTarget;
	ComPtr<ID3D11ShaderResourceView> pPostProcessOutlineSRV;
	ComPtr<ID3D11BlendState> outlineBlendState;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> context;
	std::unique_ptr<Texture> postProcessOutlineTexture;
	std::unique_ptr<VertexShader> postProcessVertexShader;
	std::unique_ptr<PixelShader> postProcessPixelShader;
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float blendFactor[4] = { 0, 0, 0, 0 };
};