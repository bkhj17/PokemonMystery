#include "Framework.h"
#include "VertexShader.h"

VertexShader::VertexShader(wstring file)
{

#pragma region LoadVS
	//VS 로드
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	
	D3DCompileFromFile(file.c_str(), nullptr, nullptr, "VS", "vs_5_0", flags, 0, &blob, nullptr);
	DEVICE->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);

	CreateInputLayout();
#pragma endregion
}

VertexShader::~VertexShader()
{
	shader->Release();
	inputLayout->Release();
	reflection->Release();
}

void VertexShader::Set()
{
	DC->IASetInputLayout(inputLayout);
	DC->VSSetShader(shader, nullptr, 0);
}

void VertexShader::CreateInputLayout()
{
	//셰이더 정보 불러오기
	D3DReflect(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		IID_ID3D11ShaderReflection, //인터페이스 ID
		(void**)&reflection			
	);
	//셰이더 정보 저장
	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);

	vector<D3D11_INPUT_ELEMENT_DESC> inputLayouts;
	inputLayouts.reserve(shaderDesc.InputParameters);

	for (UINT i = 0; i < shaderDesc.InputParameters; i++) {
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; //자동 맞춤
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		//포맷 설정
		if (paramDesc.Mask < 2) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if(paramDesc.Mask < 4) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;

		}
		else if (paramDesc.Mask < 8) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask < 16) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string temp = paramDesc.SemanticName;
		if (temp == "POSITION")
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		int n = (int)temp.find_first_of('_');

		if (temp.substr(0, n) == "INSTANCE") {
			elementDesc.InputSlot = 1;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}

		inputLayouts.push_back(elementDesc);
	}

	DEVICE->CreateInputLayout(inputLayouts.data(), (UINT)inputLayouts.size(), blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);
}
