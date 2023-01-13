#include "Framework.h"
#include "Texture.h"

unordered_map<wstring, Texture*> Texture::textures;

//move : 이동 생성자. 있는 변수를 그대로 옮김. 복사 불가능한 경우 사용
Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image, wstring file)
	: srv(srv), image(move(image)), file(file)
{
}

Texture::~Texture()
{
}

void Texture::PSSet(UINT slot)
{
	DC->PSSetShaderResources(slot, 1, &srv);
}

Texture* Texture::Add(wstring file)
{
	if (textures.count(file) > 0)
		return textures[file];

	ScratchImage image;
	LoadFromWICFile(file.c_str(), WIC_FLAGS_NONE, nullptr, image);

	ID3D11ShaderResourceView* srv;

	CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
		image.GetMetadata(), &srv);

	textures[file] = new Texture(srv, image, file);

	return textures[file];
}

void Texture::Delete()
{
	for (auto& texture : textures) {
		delete texture.second;
	}

}
