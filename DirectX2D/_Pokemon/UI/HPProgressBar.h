#pragma once

class HPProgressBar : public ProgressBar
{
public:
	HPProgressBar(wstring frontImageFile, wstring backImageFile);
	~HPProgressBar() = default;

	void SetRenderAmount(float renderAmount);
private:
	float renderAmount = 1.0f;
};

