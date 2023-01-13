#include "Framework.h"
#include "VertexShader.h"

VertexShader::VertexShader(wstring file)
{

#pragma region LoadVS
	//VS 로드
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	
	D3DCompileFromFile(file.c_str(), nullptr, nullptr, "VS", "vs_5_0", flags, 0, &blob, nullptr);
	DEVICE->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);

	D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
		{
			"POSITION",                 //넣을 변수의 시멘틱 이름. hlsl과 일치해야함
			0,                          //시멘틱 색인(시멘틱 뒤에 숫자를 붙이면 구분해서 넣을 수 있다
			DXGI_FORMAT_R32G32B32_FLOAT, //들어가는 값의 형식
			0,                          //UINT InputSlot;   
			0,                          //UINT AlignedByteOffset; 
			D3D11_INPUT_PER_VERTEX_DATA,//D3D11_INPUT_CLASSIFICATION InputSlotClass;
			0                           //UINT InstanceDataStepRate;
		},
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT layoutSize = ARRAYSIZE(layoutDesc);
	DEVICE->CreateInputLayout(layoutDesc, layoutSize, blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);

#pragma endregion
}

VertexShader::~VertexShader()
{
	shader->Release();
	inputLayout->Release();
}

void VertexShader::Set()
{
	DC->IASetInputLayout(inputLayout);
	DC->VSSetShader(shader, nullptr, 0);
}
