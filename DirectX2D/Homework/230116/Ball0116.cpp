#include "Framework.h"
#include "Ball0116.h"
#include "Bar0116.h"
#include "Area0116.h"
#include "Block0116.h"

Ball0116::Ball0116(float radius)
	: Quad(Vector2(radius * 2, radius * 2), {}), radius(radius)
{
	SetTexture(L"Textures/Shooting/cursor.png");

	collider = new CircleCollider(radius);
	collider->SetParent(this);
	collider->UpdateWorld();
}

Ball0116::~Ball0116()
{
	delete collider;
}

void Ball0116::Update()
{
	ApplyGravity();
	Move();

	UpdateWorld();
	collider->UpdateWorld();
}

void Ball0116::Render()
{
	__super::Render();
	collider->Render();
}

void Ball0116::BarCollision(Bar0116* bar)
{
	if (!collider->IsRectCollision(bar->GetCollider(), nullptr))
		return;

	Vector2 normal = bar->Up();

	Vector2 direction = velocity + normal * 2.0f * (Dot(velocity * -1.0f, normal));
	direction.Normalize();

	float power = 0.0f;
	if (bar->IsSwing()) {
		//
		float rate = bar->GetInterpolation() + 1.0f;

		Vector2 pos(bar->GlobalPos().x, bar->GlobalPos().y);
		float distanceRate = Distance(pos, localPosition) / bar->GetSize().x;

		power = rate * velocity.Length() * 0.6f + distanceRate;
		velocity = normal * power;
	}
	else {
		power = velocity.Length() * BOUND;
		velocity = direction * power;
	}

	auto rect = bar->GetCollider();
	Vector2 contactPos = {};

	if (bar->IsLeft()) {
		Vector2 leftTop = rect->LeftTop();

		Vector2 edgeToBall = Pos() - leftTop;
		float dist = Clamp(0.0f, bar->GetSize().x, Dot(edgeToBall, bar->Right()));

		contactPos = leftTop + bar->Right() * dist;
	}
	else {
		Vector2 rightTop = rect->RightTop();

		Vector2 edgeToBall = Pos() - rightTop;
		float dist = Clamp(0.0f, bar->GetSize().x, Dot(edgeToBall, bar->Left()));

		contactPos = rightTop + bar->Left() * dist;
	}

	localPosition = contactPos + normal * collider->Radius();
}

void Ball0116::AreaCollision(Area0116* area)
{
	auto leftTop = area->LeftTop();
	auto rightBottom = area->RightBottom();

	if (Pos().x - collider->Radius() < leftTop.x) {
		Pos().x = leftTop.x + collider->Radius();
		velocity.x *= -1.0f;

		velocity *= 0.7f;
	}

	if (Pos().x + collider->Radius() > rightBottom.x) {
		Pos().x = rightBottom.x - collider->Radius();
		velocity.x *= -1.0f;

		velocity *= 0.7f;
	}

	if(Pos().y + collider->Radius() > leftTop.y) {
		Pos().y = leftTop.y - collider->Radius();
		velocity.y *= -1.0f;

		velocity *= 0.7f;
	}
}

void Ball0116::BlockCollision(Block0116* block)
{
	if (!block->Active() || block->IsDead())
		return;
	//AABB
	Vector2 overlap = {};
	auto blockCollider = block->GetCollider();

	if (collider->IsRectCollision(blockCollider, &overlap)) {
		block->Hit();

		auto rectCollider = (RectCollider*)block->GetCollider();
		float l = rectCollider->L();
		float r = rectCollider->R();
		float t = rectCollider->T();
		float b = rectCollider->B();

		float back = radius + 5.0f;

		if (overlap.x > overlap.y) {
			//»óÇÏ
			if (localPosition.y > blockCollider->GlobalPos().y) {
				localPosition.y += overlap.y;
				velocity.y = abs(velocity.y);
			}
			else {
				localPosition.y -= overlap.y;
				velocity.y = -abs(velocity.y);
			}
		}
		else {
			//ÁÂ¿ì
			if (localPosition.x < blockCollider->GlobalPos().x) {
				localPosition.x -= overlap.x;
				velocity.x = -abs(velocity.x);
			}
			else {
				localPosition.x += overlap.x;
				velocity.x = abs(velocity.x);
			}
		}

		velocity *= 0.7f;
	}
}

void Ball0116::ApplyGravity()
{
	velocity.y -= GRAVITY * DELTA;
}

void Ball0116::Move()
{
	localPosition += velocity * DELTA;
}
