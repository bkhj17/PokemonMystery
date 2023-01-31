#include "Framework.h"
#include "Window0131.h"

Window0131::Window0131(Vector2 size)
	: Quad(size)
{
	isActive = false;

	collider = new RectCollider(size);
	collider->SetParent(this);

	closeButton = new Button(Vector2( 30.0f, 30.0f ));
	closeButton->SetTexture(Texture::Add(L"Textures/WindowTest/b2.png"));
	closeButton->Pos() = Half() - closeButton->Half();
	closeButton->SetParent(this);
	closeButton->SetEvent(bind(&Window0131::Close, this));

	auto texture = Texture::Add(L"Textures/WindowTest/w1.png");
	SetTexture(texture);
	UpdateWorld();
}

Window0131::~Window0131()
{
	delete closeButton;
}

void Window0131::Update()
{
	if (!isActive)
		return;

	closeButton->Update();
	UpdateWorld();
}

void Window0131::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
	closeButton->UpdateWorld();
}

void Window0131::Render()
{
	if (!isActive)
		return;

	if(texture != nullptr)
		__super::Render();
	
	collider->Render();
	closeButton->Render();
}

void Window0131::Close()
{
	Observer::Get()->ExecuteEvent("CloseWindow");
}
