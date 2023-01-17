#pragma once
class Quad : public GameObject {
public:
	Quad(Vector2 size, Vector2 pos = {});
	Quad(wstring file, Vector2 pos = {});
	~Quad();

	virtual void Render() override;

	void SetTexture(wstring file);

	Vector2 GetSize() { return size; }
	Vector2 Half() { return size * 0.5f; }
private:
	void CreateMesh(Vector2 size, Vector2 pos);

protected:
	Texture* texture = nullptr;
	Vector2 size;
};