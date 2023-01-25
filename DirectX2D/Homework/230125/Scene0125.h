#pragma once

class Scene0125 : public Scene
{
public:
	Scene0125();
	~Scene0125();
	// Scene을(를) 통해 상속됨
	virtual void Update() override;

	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

private:
	Quad* bg;

	RenderTarget* renderTarget;
	RenderTarget* renderTarget2;
	vector<Quad*> renderTextures;

	Plane* plane1;
	Plane* plane2;

	IntValueBuffer* intValueBuffer;
	FloatValueBuffer* sizeValueBuffer;
	FloatValueBuffer* posValueBuffer;

	Camera* cam1;
	Camera* cam2;
};

