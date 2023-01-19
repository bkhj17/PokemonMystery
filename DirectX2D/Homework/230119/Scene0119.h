#pragma once

class Robot0119;
class Scene0119 : public Scene
{
public:
	Scene0119();
	~Scene0119();

	// Scene¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	void DragOn();
	void MoveRobots();
private:
	//∞¥√º
	vector<Robot0119*> robots;

	IntValueBuffer* intValueBuffer;
	ColorBuffer* colorBuffer;

	Vector2 clickedPos = { 0.0f, 0.0f };
	Vector2 postPos = { 0.0f, 0.0f };

	RectCollider* dragCollier;

	bool clickSelected = false;
};

