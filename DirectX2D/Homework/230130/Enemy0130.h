#pragma once
class Enemy0130 : public GameObject
{
public:
	enum ActionState {
		PATROL,
		CHASE,
		SCARED,
	};
public:
	Enemy0130();
	~Enemy0130();

	virtual void Update();
	virtual void UpdateWorld();
	void Render();

	void PostRender();

	RectCollider* GetCollider() { return collider; }

	void SetPath(vector<Vector2> path) { this->path = path; }
	vector<Vector2>& GetPath() { return path; }

	void Destroy();
	void SetDestroyEvent(Event event) { destroyEvent = event; }

	Vector2 GetTargetPos();
	void SetAction(ActionState state);
private:

	void CreateActions();

	void MovePath();

private:
	float moveSpeed = 100.0f;

	RectCollider* collider;
	vector<Vector2> path;
	Event destroyEvent;

	ActionState curAction = PATROL;
	map<UINT, Action*> actions;

	ColorBuffer* colorBuffer;
};