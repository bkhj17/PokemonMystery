#pragma once
class ClearScene : public Scene
{
public:
	ClearScene();
	~ClearScene();
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Start() override;
private:
	class ClearUI* clearUI;
};

