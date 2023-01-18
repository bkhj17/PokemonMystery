#pragma once
class Quad : public GameObject {
public:
	Quad(Vector2 size, Vector2 pos = {});
	Quad(wstring file, Vector2 pos = {});
	~Quad();

	virtual void Render();

	void SetTexture(wstring file);

	Vector2 GetSize() { return size; }
	Vector2 Half() { return size * 0.5f; }
private:
	void CreateMesh(Vector2 size, Vector2 pos);

protected:
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;
	Texture* texture = nullptr;

	vector<Vertex> vertices;
	vector<UINT> indices;

	Vector2 size;
};