#pragma once
class Texture
{
private:
	Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file);
	~Texture();

public:
	void PSSet(UINT slot = 0);
	Vector2 GetSize() {
		return Vector2((float)image.GetMetadata().width, (float)image.GetMetadata().height);
	}

public:
	static Texture* Add(wstring file);
	static void Delete();
private:
	wstring file;
	ScratchImage image;
	ID3D11ShaderResourceView* srv;

	static unordered_map<wstring, Texture*> textures;
};

