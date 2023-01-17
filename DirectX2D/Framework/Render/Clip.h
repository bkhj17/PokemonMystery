#pragma once
class Clip
{
private:
	const float FPS = 10.0f;

public:
	Clip(vector<Frame*> frames, bool isLoop = true, float speed = 1.0f);
	~Clip();

	void Update();
	void Render();

	void Play();
	void Stop() { isPlay = false; }
	bool IsPlay() { return isPlay; }

	Frame* GetCurFrame() { return frames[curFrameNum]; }

	map<UINT, function<void()>> events;
private:
	vector<Frame*> frames;
	UINT curFrameNum = 0;

	float frameTime = 0.0f;
	float speed = 0.0f;
	float delayTime = 0.0f;

	bool isLoop = false;
	bool isPlay = false;
};

