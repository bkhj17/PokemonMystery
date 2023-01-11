#pragma once
class Rect
{
public:
	Rect(Float2 pos, Float2 size);
	~Rect();

	void Update();
	void Render();
public:
	bool isActive = false;
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexColor> vertices;
	vector<UINT> indices;
};

