#include "Framework.h"
#include "Transform0112.h"
#include "Plane0112.h"

Plane0112::Plane0112()
{
	transform = new Transform0112;
	transform->scale.x = SCALE_LENGTH;
	transform->scale.y = SCALE_WING;

	vertices.emplace_back(0.5f, 0.0f, 1.0f, 1.0f, 1.0f);
	vertices.emplace_back(-0.2f, -0.3f, 1.0f, 1.0f, 0.0f);
	vertices.emplace_back(-0.2f, 0.3f, 1.0f, 1.0f, 0.0f);
	indices = { 0, 1, 2 };

	shotPoint = vertices[0].pos;

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

Plane0112::~Plane0112()
{
	delete transform;
}

void Plane0112::Update()
{
	//movePos
	Vector2 move = { 0.0f, 0.0f };
	if (KEY_PRESS(VK_RIGHT) || KEY_PRESS('D'))	move.x += 1.0f;
	if (KEY_PRESS(VK_LEFT) || KEY_PRESS('A'))	move.x -= 1.0f;
	if (KEY_PRESS(VK_UP) || KEY_PRESS('W'))		move.y += 1.0f;
	if (KEY_PRESS(VK_DOWN) || KEY_PRESS('S'))	move.y -= 1.0f;
	transform->pos += move * moveSpeed * DELTA;

	if (KEY_DOWN('Q') && shotRate > MIN_SHOT_RATE) shotRate -= SHOT_RATE_ADD;
	if (KEY_DOWN('E') && shotRate < MAX_SHOT_RATE) shotRate += SHOT_RATE_ADD;
	
	//rotate by mousePos
	float cosValue = Dot((mousePos - transform->pos).GetNormalized(), { 1.0f, 0.0f });
	transform->angle = acosf(cosValue) * (mousePos.y < transform->pos.y ? -1.0f : 1.0f);

	//Shot
	if (coolTime > 0.0f)
		coolTime -= DELTA;
	if (KEY_PRESS(VK_SPACE))
   		Shot();

	if (transform->pos.x - COLLISION < 0.0f)
		transform->pos.x = COLLISION;
	if (transform->pos.x + COLLISION > WIN_WIDTH)
		transform->pos.x = WIN_WIDTH - COLLISION;
	if (transform->pos.y - COLLISION < 0.0f)
		transform->pos.y = COLLISION;
	if (transform->pos.y + COLLISION > WIN_HEIGHT)
		transform->pos.y = WIN_HEIGHT - COLLISION;
}

void Plane0112::Render()
{
	transform->Set();

	__super::Render();
}

void Plane0112::Shot()
{
	if (coolTime <= 0.0f) {
   		coolTime = shotRate;
		shotFunc(transform->pos, transform->angle);
	}
}

