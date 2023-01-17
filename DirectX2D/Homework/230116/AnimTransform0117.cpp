#include "Framework.h"
#include "AnimTransform0117.h"

AnimTransform0117::AnimTransform0117()
{
	vertexShader = Shader::AddVS(L"VertexUV.hlsl");
	pixelShader = Shader::AddPS(L"PixelUV.hlsl");

	worldBuffer = new MatrixBuffer();
}

AnimTransform0117::~AnimTransform0117()
{
	delete worldBuffer;

	for (auto& clip : clips)
		delete clip.second;
}

void AnimTransform0117::Update()
{
	if (curClip == nullptr)
		return;

	curClip->Update();

	UpdateWorld();
}

void AnimTransform0117::Render()
{
	if (curClip == nullptr)
		return;

	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	vertexShader->Set();
	pixelShader->Set();

	curClip->Render();
}

void AnimTransform0117::LoadClip(int type, string path, string file, bool isLoop, float speed)
{
	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument;

	document->LoadFile((path + file).c_str());

	tinyxml2::XMLElement* atlas = document->FirstChildElement();
	string textureFile = path + atlas->Attribute("imagePath");

	vector<Frame*> frames;
	tinyxml2::XMLElement* sprite = atlas->FirstChildElement();

	while (sprite != nullptr) {
		float x, y, w, h;
		x = sprite->FloatAttribute("x");
		y = sprite->FloatAttribute("y");
		w = sprite->FloatAttribute("w");
		h = sprite->FloatAttribute("h");

		frames.push_back(new Frame(ToWString(textureFile), x, y, w, h));
		
		sprite = sprite->NextSiblingElement();
	}

	clips[type] = new Clip(frames, isLoop, speed);

	delete document;
}

void AnimTransform0117::SetClipEvent()
{


}

