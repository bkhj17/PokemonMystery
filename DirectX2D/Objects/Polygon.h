#pragma once
class SpawnPolygon
{
public:
	SpawnPolygon();
	~SpawnPolygon();

	void Update();
	void Render();

	void Spawn();
public:
	bool isActive = false;
private:
	const UINT POOLSIZE = 11;

	VertexShader* vertexShader;
	PixelShader* pixelShader;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexColor> vertices;
	vector<UINT> indices;

	float timeToDeath = 1.0f;
	float lifeTime = 0.0f;

	int use = 0;
};

