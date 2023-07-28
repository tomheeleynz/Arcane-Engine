#pragma once

#include <Arcane.h>
#include <imgui.h>
#include <ImSequencer.h>
#include <vector>

static const char* SequencerTypeNames[] = { "Animation" };

struct AnimationSequencer : public ImSequencer::SequenceInterface
{
	// Inherited via SequenceInterface
	virtual int GetFrameMin() const override
	{
		return m_FrameMin;
	}

	virtual int GetFrameMax() const override
	{
		return m_FrameMax;
	}

	virtual int GetItemCount() const override
	{
		return (int)keyFrameItems.size();
	}

	virtual void Get(int index, int** start, int** end, int* type, unsigned int* color) override
	{
		KeyFrameItem& item = keyFrameItems[index];
		if (color)
			*color = 0xFF0000; // same color for everyone, return color based on type
		if (start)
			*start = &item.frameStart;
		if (end)
			*end = &item.frameEnd;
		if (type)
			*type = 0;
	}

	virtual void Add(int type) { keyFrameItems.push_back(KeyFrameItem{ 0, 10 }); };

	AnimationSequencer() : m_FrameMin(0), m_FrameMax(0) {}
	int m_FrameMin, m_FrameMax;

	struct KeyFrameItem
	{
		int frameStart;
		int frameEnd;
	};

	std::vector<KeyFrameItem> keyFrameItems;
};

class AnimationPanel
{
public:
	AnimationPanel();

	void OnImGuiRender();
private:
	Arcane::Animation* m_Animation = nullptr;
	bool m_CreateAnimation = false;
	AnimationSequencer m_AnimationSequencer;
private:
	void CreateAnimation();
};