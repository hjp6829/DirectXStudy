#pragma once
#include "Bindable.h"
#include "Surface.h"

class Texture : public Bindable {
public:
	Texture(ID3D11Device* device, ComPtr<ID3D11ShaderResourceView> View,int idx);
	void Bind(ID3D11DeviceContext* context) override;
	void ChangeNewTextureView(ComPtr<ID3D11ShaderResourceView> View);
	private:
	ComPtr<ID3D11ShaderResourceView> textureView;
	int index;
};