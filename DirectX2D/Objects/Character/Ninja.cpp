#include "Framework.h"

Ninja::Ninja()
{
	vertexShader = Shader::AddVS(L"VertexUV.hlsl");
	pixelShader = Shader::AddPS(L"PixelUV.hlsl");

	Pos() = { CENTER_X, CENTER_Y };

	LoadClips();

	curClip = clips[IDLE];
	curClip->Play();

	worldBuffer = new MatrixBuffer();
}

Ninja::~Ninja()
{
	delete worldBuffer;

	for (auto& clip : clips)
		delete clip.second;
}

void Ninja::Update()
{
	if (KEY_DOWN('1'))
		curClip = clips[IDLE];
	if (KEY_DOWN('2')) {
		curClip = clips[ATTACK];
		curClip->Play();
	}

	if (!curClip->IsPlay())
		curClip = clips[IDLE];

	curClip->Update();

	UpdateWorld();
}

void Ninja::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	vertexShader->Set();
	pixelShader->Set();

	curClip->Render();
}

void Ninja::LoadClips()
{
	string path = "Textures/Ninja/";
	LoadClip(IDLE, path, "Idle.xml", true, 1.0f);
	LoadClip(ATTACK, path, "Attack.xml", false, 3.0f);
}

void Ninja::LoadClip(ActionType type, string path, string file, bool isLoop, float speed)
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
