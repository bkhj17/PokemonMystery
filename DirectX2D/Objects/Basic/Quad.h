#pragma once
class Quad : public GameObject {
public:
	Quad(Vector2 size, Vector2 pos = {});
	Quad(wstring file, Vector2 startUV = {}, Vector2 endUV = { 1.0f, 1.0f });
	~Quad();

	virtual void Render();

	void SetTexture(wstring file);
	void SetTexture(Texture* texture) { this->texture = texture; }

	Vector2 GetSize() { return size; }
	Vector2 Half() { return size * 0.5f; }

	Float4& GetColor() { return colorBuffer->Get(); }
private:
	void CreateMesh(Vector2 size, Vector2 startUV, Vector2 endUV);

protected:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ColorBuffer* colorBuffer;

	Texture* texture = nullptr;

	vector<Vertex> vertices;
	vector<UINT> indices;

	Vector2 size;
};