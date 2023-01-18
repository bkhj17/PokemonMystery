#include "Framework.h"
#include "Clay0113.h"

Clay0113::Clay0113()
	: Quad(L"Textures/Shooting/playerBullet.png")
{
	isActive = false;

}

Clay0113::~Clay0113()
{
}

void Clay0113::Update()
{
	if (!isActive)
		return;

	velocity.y += yAccel * DELTA;	//가속도 적용
	//각도 적용
	localRotation.z = velocity.Angle();
	//위치 변경
	localPosition += velocity * DELTA;

	Out();

	UpdateWorld();
}

void Clay0113::Spawn()
{
	if (isActive)
		return;
	isActive = true;
	
	//좌우 어디서 나올지 결정
	bool fromLeft = rand() % 2 == 0;

	//시작 위치
	localPosition.x = fromLeft ? -RADIUS : WIN_WIDTH + RADIUS;
	localPosition.y = Random(RADIUS, 500.0f);

	//출현 시간 - 끝에서 끝까지 보낼 생각이기에 시간을 기준으로 잡는다
	time = Random(2.0f, 3.0f);

	//x 속도 = 가로 거리 / 시간 => 제 시간에 목표지점에 도착. 등속
	xSpeed = (WIN_WIDTH + RADIUS * 2) / time;
	velocity.x = fromLeft ? xSpeed : -xSpeed;

	float halfTime = time * 0.5f;
	//제대로 계산하는데에 실패했는데 어찌어찌 돌아간다. 어떻게 된걸까
	float maxYSpeed = (WIN_HEIGHT - localPosition.y) * 2 / halfTime;
	velocity.y = Random(300.0f, maxYSpeed);
	yAccel = -velocity.y / (halfTime); 
}

void Clay0113::Hit()
{
	if (!isActive)
		return;

	isActive = false;
	if (hitFunc != nullptr)
		hitFunc();
}

void Clay0113::Out()
{
	//비활성 상태면 무시 : Update에서 확인하지만 기분상 해둔다
	if (!isActive)
		return;

	//나갔는지 검사
	isActive &= !(velocity.x < 0.0f && localPosition.x < -RADIUS);
	isActive &= !(velocity.x > 0.0f && localPosition.x > WIN_WIDTH + RADIUS);
	isActive &= !(localPosition.y < 0.0f);

	if (!isActive && outFunc != nullptr)
		outFunc();
}
