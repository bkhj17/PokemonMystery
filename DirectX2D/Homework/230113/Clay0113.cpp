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

	direction.y += yAccel * DELTA;	//���ӵ� ����
	//���� ����
	localRotation.z = direction.Angle();
	//��ġ ����
	localPosition += direction * DELTA;

	Out();

	__super::Update();
}

void Clay0113::Spawn()
{
	if (isActive)
		return;
	isActive = true;
	
	//�¿� ��� ������ ����
	bool fromLeft = rand() % 2 == 0;

	//���� ��ġ
	localPosition.x = fromLeft ? -RADIUS : WIN_WIDTH + RADIUS;
	localPosition.y = Random(RADIUS, 500.0f);

	//���� �ð� - ������ ������ ���� �����̱⿡ �ð��� �������� ��´�
	time = Random(2.0f, 3.0f);

	//x �ӵ� = ���� �Ÿ� / �ð� => �� �ð��� ��ǥ������ ����. ���
	xSpeed = (WIN_WIDTH + RADIUS * 2) / time;
	direction.x = fromLeft ? xSpeed : -xSpeed;

	float halfTime = time * 0.5f;
	//����� ����ϴµ��� �����ߴµ� ������� ���ư���. ��� �Ȱɱ�
	float maxYSpeed = (WIN_HEIGHT - localPosition.y) * 2 / halfTime;
	direction.y = Random(300.0f, maxYSpeed);
	yAccel = -direction.y / (halfTime); 
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
	//��Ȱ�� ���¸� ���� : Update���� Ȯ�������� ��л� �صд�
	if (!isActive)
		return;

	//�������� �˻�
	isActive &= !(direction.x < 0.0f && localPosition.x < -RADIUS);
	isActive &= !(direction.x > 0.0f && localPosition.x > WIN_WIDTH + RADIUS);
	isActive &= !(localPosition.y < 0.0f);

	if (!isActive && outFunc != nullptr)
		outFunc();
}