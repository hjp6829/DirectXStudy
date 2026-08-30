#pragma once
#include "Bindable.h"
#include "Surface.h"

class Texture : public Bindable {
public:
	Texture(DirectXMain& dxdMain, ComPtr<ID3D11ShaderResourceView> View,int idx);
	void Bind(DirectXMain& dxdMain) override;
	void ChangeNewTextureView(ComPtr<ID3D11ShaderResourceView> View);
	private:
	ComPtr<ID3D11ShaderResourceView> textureView;
	int index;
};