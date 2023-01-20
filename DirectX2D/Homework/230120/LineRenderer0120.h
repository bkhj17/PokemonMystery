#pragma once

class LineRenderer0120
{
public:
	LineRenderer0120();
	~LineRenderer0120();

	void Set(deque<Vector2>& dq);

	void Update();
	void Render();

	bool IsActive() { return showTime > 0.0f; }
private:
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	VertexBuffer* vertexBuffer;
	vector<VertexPos> vertices;

	MatrixBuffer* worldBuffer;
	ColorBuffer* colorBuffer;

	float showTime = 0.0f;
	float showRate = 1.0f;
};

