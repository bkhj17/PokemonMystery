#pragma once

class Scene0120 : public Scene
{
public:
	Scene0120();
	~Scene0120();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;
private:



};

