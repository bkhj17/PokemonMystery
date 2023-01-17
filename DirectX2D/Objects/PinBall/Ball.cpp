#include "Framework.h"
#include "Ball.h"

Ball::Ball()
	: Quad(L"Textures/Pinball/Ball.png")
{
	collider = new CircleCollider(size.x * 0.35f);
	collider->SetParent(this);

	localPosition = { 200.0f, WIN_HEIGHT - 100.0f };
	localScale *= 0.15f;
}

Ball::~Ball()
{
	delete collider;
}

void Ball::Update()
{
	CollisionBorder();

	velocity.y -= GRAVITY * DELTA;

	localPosition += velocity * DELTA;

	UpdateWorld();
	collider->UpdateWorld();
}

void Ball::Render()
{
	__super::Render();
	collider->Render();
}

void Ball::CollisionBat(Bat* bat)
{
	Vector2 normal = bat->IsRight() ? bat->Down() : bat->Up();

	Vector2 direction = velocity + normal * 2.0f * (Dot(velocity * -1.0f, normal));
	direction.Normalize();

	float power = 0.0f;
	float distanceRate = 0.0f;

	if (bat->IsSwing())
	{
		float rate = bat->GetInterpolationValue() + 1.0f;

		Vector2 pos = bat->Pos();
		bat->IsRight() ? pos.x = WIN_WIDTH : pos.x = 0.0f;
		distanceRate += Distance(pos, localPosition) / bat->GetSize().x;

		Clamp(0.0f, 1.0f, distanceRate);

		power = rate * velocity.Length() * 0.6f + distanceRate;
		velocity = normal * power;
	}
	else
	{
		power = velocity.Length() * BOUND;
		velocity = direction * power;
	}

	RectCollider* rect = (RectCollider*)bat->GetCollider();

	Vector2 contactPos;

	Vector2 edgeToBall = Pos() - rect->GlobalPos();
	if (bat->IsRight())
	{

		float distance = Clamp(0.0f, bat->GetSize().x, Dot(edgeToBall, bat->Right()));

		contactPos = rect->LeftBottom() + bat->Right() * distance;
	}
	else
	{
		Vector2 edgeToBall = Pos() - rect->LeftTop();
		float distance = Clamp(0.0f, bat->GetSize().x, Dot(edgeToBall, bat->Right()));

		contactPos = rect->LeftTop() + bat->Right() * distance;
	}

	Pos() = contactPos + normal * collider->Radius();
}

void Ball::CollisionBorder()
{
	Vector2 halfSize = { Half().x * localScale.x, Half().y * localScale.y };

	if (localPosition.x < halfSize.x) {		
		//위치 보정
		localPosition.x = halfSize.x;
		//힘 계산
		float power = velocity.Length() * BOUND;
		velocity.x *= - 1.0f;
		velocity.Normalize();
		velocity *= power;
	}
	if (localPosition.x + halfSize.x > WIN_WIDTH) {
		//위치 보정
		localPosition.x = WIN_WIDTH - halfSize.x;
		//힘 계산
		float power = velocity.Length() * BOUND;
		velocity.x *= -1.0f;
		velocity.Normalize();
		velocity *= power;
	}
	if (localPosition.y + halfSize.y > WIN_HEIGHT) {
		//위치 보정
		localPosition.y = WIN_HEIGHT - halfSize.y;
		//힘 계산
		float power = velocity.Length() * BOUND;
		velocity.y *= -1.0f;
		velocity.Normalize();
		velocity *= power;
	}
}
