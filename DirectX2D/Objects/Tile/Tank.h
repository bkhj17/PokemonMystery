#pragma once

class Tank : public Quad
{
public:
	Tank();
	~Tank();

	virtual void Update();
	virtual void UpdateWorld();
	void Render();

	void PostRender();

	RectCollider* GetCollider() { return collider; }

	void SetPath(vector<Vector2> path) { this->path = path; }
	vector<Vector2>& GetPath() { return path; }

	virtual void Fire();
	string GetShooterTag() { return shooterTag; }


	void Destroy();
	void SetDestroyEvent(Event event) { destroyEvent = event; }


protected:
	Vector2 GetFirePoint();

	virtual void Control();
	virtual void HeadControl();

	void MovePath();

protected:
	string shooterTag = "Tank";

	float moveSpeed = 200.0f;
	float rotSpeed = 5.0f;

private:
	RectCollider* collider;

	Quad* head;

	vector<Vector2> path;


	Event destroyEvent;
};

