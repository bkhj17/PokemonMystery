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
	ID3D11InputLayout* inputLayout;     //���̴� ���� �Է�

	//���̴��� ���� �� �Է� ������ �޾ƿ� �� �ִ� -> �ڵ� ���� ����
	ID3D11ShaderReflection* reflection;
};

