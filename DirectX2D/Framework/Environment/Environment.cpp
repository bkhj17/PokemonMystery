#include "Framework.h"
#include "Environment.h"

Environment::Environment()
{
	CreateProjection();
	CreateSamplerState();
	CreateBlendState();
	CreateRasterizerState();

	mainCamera = new Camera();
}

Environment::~Environment()
{
	delete projectionBuffer;
	delete mainCamera;

	samplerState->Release();
	alphaBlendState->Release();
	rasterizerState->Release();

}

void Environment::SetAlphaBlend()
{
	float blendFactor[4] = {};
	DC->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
}

void Environment::SetAdditiveBlend()
{
	float blendFactor[4] = {};
	DC->OMSetBlendState(additiveBlendState, blendFactor, 0xffffffff);
}

void Environment::CreateProjection()
{
	//왼손 좌표계로 원점이 센터로 맞춰진 설정을 끄겠다
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
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR; //깨진 부분 보정
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	//그리는 방식(uv가 1을 벗어났을 경우)
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDesc.MinLOD = 0;					//가깝다. 0은 최대 퀄리티
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX; //멀다. 퀄리티 최저
	//LOD(Level Of Detail) : 카메라의 거리에 따라 퀄리티를 조절(0이 최대이고 높을 수록 적어짐)

	DEVICE->CreateSamplerState(&samplerDesc, &samplerState);

	DC->PSSetSamplers(0, 1, &samplerState);
}

void Environment::CreateBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};	
	blendDesc.RenderTarget[0].BlendEnable = true;	//알파 블렌드 사용
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //텍스처 = 알파 그대로
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //배경 = 1-알파
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;	//덧셈

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DEVICE->CreateBlendState(&blendDesc, &alphaBlendState);

	float blendFactor[4] = {};
	DC->OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);

	//additive
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE; //배경 * color
	DEVICE->CreateBlendState(&blendDesc, &additiveBlendState);
}

void Environment::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;

	DEVICE->CreateRasterizerState(&rasterizerDesc, &rasterizerState);

	DC->RSSetState(rasterizerState);
}
