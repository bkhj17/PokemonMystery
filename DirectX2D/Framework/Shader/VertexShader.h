#pragma once
class VertexShader : public Shader
{
private:
	friend class Shader;
	VertexShader(wstring);
	~VertexShader();
public:
	virtual void Set() override;

private:
	void CreateInputLayout();

private:
	ID3D11VertexShader* shader;
	ID3D11InputLayout* inputLayout;     //셰이더 정보 입력

	//셰이더에 들어가야 할 입력 정보를 받아올 수 있다 -> 자동 설정 가능
	ID3D11ShaderReflection* reflection;
};

