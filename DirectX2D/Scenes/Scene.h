#pragma once

class Scene {

public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void Update() = 0;

	virtual void Render() = 0;		//오브젝트 출력용
	virtual void PostRender() = 0;	//UI 출력용

	virtual void Start() {};
	virtual void End() {};
};