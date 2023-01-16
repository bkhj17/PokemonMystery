#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
	

	colliders.push_back(new RectCollider({ 50.0f, 50.0f }));
	colliders.back()->SetTag("Rect1");
	colliders.back()->Pos() = {CENTER_X, CENTER_Y};

	colliders.push_back(new CircleCollider(50.0f));
	colliders.back()->SetTag("Cilcle1");
	colliders.back()->Pos() = { CENTER_X - 100.0f, CENTER_Y };

	//colliders.push_back(new RectCollider({ 50.0f, 50.0f }));
	//colliders.back()->SetTag("Rect2");
	//colliders.back()->Pos() = { CENTER_X - 100.0f, CENTER_Y };

}

CollisionScene::~CollisionScene()
{
	for (auto collider : colliders)
		delete collider;
}

void CollisionScene::Update()
{
	if (colliders[0]->IsCollision(colliders[1])) {
		colliders[0]->GetColor() = GREEN;
		colliders[1]->GetColor() = GREEN;
	}
	else {
		colliders[0]->GetColor() = RED;
		colliders[1]->GetColor() = RED;
	}
	
	
	//Vector2 overlap;
	//
	//if (colliders[0]->IsRectCollision((RectCollider*)colliders[1])) {
	//	colliders[0]->GetColor() = GREEN;
	//	colliders[1]->GetColor() = GREEN;
	//}
	//else {
	//	colliders[0]->GetColor() = RED;
	//	colliders[1]->GetColor() = RED;
	//}

	for (auto collider : colliders) {
		collider->UpdateWorld();
	}
}

void CollisionScene::Render()
{
	for (auto collider : colliders)
		collider->Render();
}

void CollisionScene::PostRender()
{
	for (auto collider : colliders)
		collider->RenderUI();
}
