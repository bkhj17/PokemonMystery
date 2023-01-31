#pragma once

class Scene0131 : public Scene
{
public:
	Scene0131();
	~Scene0131();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
private:
	class WindowManager0131* windowManager;
	vector<Button*> buttons;
};

