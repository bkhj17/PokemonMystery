#pragma once
class Quad : public GameObject {
public:
	Quad(Vector2 size);
	Quad(wstring file, Vector2 startUV = Vector2(), Vector2 endUV = Vector2(1.0f, 1.0f));
	~Quad();

	virtual void Render();
	virtual void PostRender();

	virtual void SetRender();

	void SetTexture(wstring file);
	void SetTexture(Texture* texture) { this->texture = texture; }

	void SetSize(Vector2 size);
	
	void UpdateVertices();
	void ModifyUV(Vector2 startUV, Vector2 endUV);

	Vector2 Half() { return size * 0.5f; }

	Float4& GetColor() { return colorBuffer->Get(); }
	Texture* GetTexture() { return texture; }

private:
	void CreateMesh();

protected:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ColorBuffer* colorBuffer;

	Texture* texture = nullptr;

	vector<Vertex> vertices;
	vector<UINT> indices;

	Vector2 startUV;
	Vector2 endUV;
};