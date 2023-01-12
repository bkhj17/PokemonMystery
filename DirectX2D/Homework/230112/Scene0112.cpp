#include "Framework.h"
#include "Plane0112.h"
#include "Scene0112.h"
#include "Bullet0112.h"
#include "Transform0112.h"

Scene0112::Scene0112()
{
	Matrix orthographic = XMMatrixOrthographicOffCenterLH(
		0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);

	projectionBuffer = new MatrixBuffer();
	projectionBuffer->Set(orthographic);
	projectionBuffer->SetVS(2);

	plane = new Plane0112;
	plane->transform->pos = { CENTER_X, CENTER_Y };
	plane->SetShotFunc(bind(&Scene0112::ShotBullet, this, placeholders::_1, placeholders::_2));

	bullets.resize(MAX_BULLET);
	for (auto& bullet : bullets)
		bullet = new Bullet0112();
}

Scene0112::~Scene0112()
{
	delete plane;
	delete projectionBuffer;
}

void Scene0112::Update()
{
	for (auto bullet : bullets)
		bullet->Update();
	plane->Update();

}

void Scene0112::Render()
{
	for (auto bullet : bullets)
		bullet->Render();
	plane->Render();
}

void Scene0112::PostRender()
{
}

void Scene0112::ShotBullet(Vector2 pos, float angle)
{
	for (auto bullet : bullets) {
		if (!bullet->IsActive()) {
			bullet->Spawn(pos, angle);
			return;
		}
	}
}
