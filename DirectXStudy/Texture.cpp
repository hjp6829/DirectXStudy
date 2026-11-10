#include "Texture.h"

Texture::Texture(ComPtr<ID3D11ShaderResourceView> View,int idx)
{
	this->index=idx;
	textureView= View;
}

void Texture::ChangeNewTextureView(ComPtr<ID3D11ShaderResourceView> View)
{
	textureView = View;
}

void Texture::Bind(ID3D11DeviceContext* context)
{
	if(textureView != nullptr)
		context->PSSetShaderResources(index,1, textureView.GetAddressOf());//2�� �Ű������� �󸶳� ���� view�� �ѹ��� �ѱ������ ���Ѱ�
}

