#pragma once

class LineRenderer0120
{
public:
	LineRenderer0120();
	~LineRenderer0120();

	void Set(deque<Vector2>& dq);
	void Render();

private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	VertexBuffer* vertexBuffer;
	vector<VertexPos> vertices;

	ColorBuffer* colorBuffer;
};

