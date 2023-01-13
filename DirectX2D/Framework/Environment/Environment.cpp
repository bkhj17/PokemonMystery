#include "Framework.h"
#include "Environment.h"

Environment::Environment()
{
	CreateProjection();
	CreateSamplerState();
	CreateBlendState();
}

Environment::~Environment()
{
	delete projectionBuffer;

	samplerState->Release();
	blendState->Release();
}

void Environment::CreateProjection()
{
	//�޼� ��ǥ��� ������ ���ͷ� ������ ������ ���ڴ�
	Matrix orthographic = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);

	if(projectionBuffer == nullptr)
		projectionBuffer = new MatrixBuffer();
	projectionBuffer->Set(orthographic);
	projectionBuffer->SetVS(2);
}

void Environment::CreateSamplerState()
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR; //���� �κ� ����
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	//�׸��� ���(uv�� 1�� ����� ���)
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;					//������. 0�� �ִ� ����Ƽ
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; //�ִ�. ����Ƽ ����
	//LOD(Level Of Detail) : ī�޶��� �Ÿ��� ���� ����Ƽ�� ����(0�� �ִ��̰� ���� ���� ������)

	DEVICE->CreateSamplerState(&samplerDesc, &samplerState);

	DC->PSSetSamplers(0, 1, &samplerState);
}

void Environment::CreateBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};	
	blendDesc.RenderTarget[0].BlendEnable = true;	//���� ������ ���
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //�ؽ�ó ���� �״��
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //��� 1-���� 
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;	//����

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DEVICE->CreateBlendState(&blendDesc, &blendState);

	float blendFactor[4] = {};
	DC->OMSetBlendState(blendState, blendFactor, 0xffffffff);
}