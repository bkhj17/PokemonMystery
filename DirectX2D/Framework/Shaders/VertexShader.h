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
	ID3D11VertexShader* shader;
	ID3D11InputLayout* inputLayout;     //셰이더 정보 입력
};

