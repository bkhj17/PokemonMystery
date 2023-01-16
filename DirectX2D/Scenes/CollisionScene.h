#pragma once
class CollisionScene : public Scene
{
public:
	CollisionScene();
	~CollisionScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	//OBB충돌 : 회전 들어간 물체들 끼리의 충돌

	vector<Collider*> colliders;

};

