#include "Framework.h"
#include "Isaac.h"

Isaac::Isaac()
	: RectCollider({30, 40.0f})
{
	head = new AnimObject;
	body = new AnimObject;
	CreateClips();

	head->SetParent(this);
	head->SetClip((int)Direction::DOWN);
	head->Pos().y = 5.0f;

	body->SetParent(this);
	body->SetClip((int)Direction::DOWN);
	body->Pos().y = -5.0f;
}

Isaac::~Isaac()
{
	delete head;
	delete body;
}

void Isaac::Update()
{
	Move();

	head->Update();
	body->Update();
	UpdateWorld();
}

void Isaac::Render()
{
	body->Render();
	head->Render();
	__super::Render();
}

void Isaac::Move()
{
	if (KEY_PRESS(VK_RIGHT))
	{
		Pos().x += speed * DELTA;
		body->Rot().y = 0.0f;

		body->SetClip(RIGHT);
		head->SetClip(RIGHT);
	}

	if (KEY_PRESS(VK_LEFT))
	{
		Pos().x -= speed * DELTA;
		body->Rot().y = XM_PI;

		body->SetClip(LEFT);
		head->SetClip(LEFT);
	}

	if (KEY_PRESS(VK_UP)) {

		Pos().y += speed * DELTA;
		body->Rot().y = 0.0f;

		body->SetClip(UP);
		head->SetClip(UP);
	}
	if (KEY_PRESS(VK_DOWN)) {

		Pos().y -= speed * DELTA;
		body->Rot().y = 0.0f;

		body->SetClip(DOWN);
		head->SetClip(DOWN);
	}
}

void Isaac::CreateClips()
{
	//Head
	wstring textureFile = L"Textures/Shooting/Isaac_Head.png";
	Vector2 cutSize = Texture::Add(textureFile)->GetSize() / Vector2(4.0f, 3.0f);

	vector<Frame*> frames;
	//UP;
	frames.push_back(new Frame(textureFile, cutSize.x * 3, 0.0f, cutSize.x, cutSize.y));
	head->AddClip(0, new Clip(frames));
	frames.clear();
	//Down
	frames.push_back(new Frame(textureFile, cutSize.x * 0, 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, 0, cutSize.x, cutSize.y));	
	head->AddClip(1, new Clip(frames));
	frames.clear();
	//Left
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 2, cutSize.x, cutSize.y));
	head->AddClip(2, new Clip(frames));
	frames.clear();
	//Right
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y, cutSize.x, cutSize.y));
	head->AddClip(3, new Clip(frames));
	frames.clear();

	//Body//////////////////////////////////////////////////////////////////////////////////////////////////////
	textureFile = L"Textures/Shooting/Isaac_Body.png";
	cutSize = Texture::Add(textureFile)->GetSize() / Vector2(8.0f, 4.0f);

	//UP;
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 3, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 4, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 5, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 6, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 7, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 3, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 3, cutSize.x, cutSize.y));
	body->AddClip(0, new Clip(frames));
	frames.clear();

	//Down;
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 3, cutSize.y * 2, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 4, cutSize.y * 2, cutSize.x, cutSize.y));
	body->AddClip(1, new Clip(frames));
	frames.clear();

	//Left;
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 3, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 4, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 5, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 6, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 7, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 1, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 1, cutSize.x, cutSize.y));
	body->AddClip(2, new Clip(frames));
	frames.clear();

	//Right
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 2, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 3, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 4, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 5, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 6, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 7, cutSize.y * 0, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 0, cutSize.y * 1, cutSize.x, cutSize.y));
	frames.push_back(new Frame(textureFile, cutSize.x * 1, cutSize.y * 1, cutSize.x, cutSize.y));
	body->AddClip(3, new Clip(frames));
	frames.clear();

}
