/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzAudioVisualiserComponent.h"

// Create new component
RzAudioVisualiserComponent::RzAudioVisualiserComponent(int initialNumChannels) : AudioVisualiserComponent(initialNumChannels)
{
	this->setColours(Colour(48, 48, 48), Colour(82, 192, 56));
	this->setRepaintRate(30);
	this->setSamplesPerBlock(150);
	this->setBufferSize(450);
}


// Get sample channel as vector path
void RzAudioVisualiserComponent::getChannelAsPath(Path& path, const Range<float>* levels, int numLevels, int nextSample)
{
	path.preallocateSpace(4 * numLevels + 8);
	
	switch (this->_input_type)
	{
		case INPUT_TYPE::NORMALIZED_LOG:
		{
			for (int i = 0; i < numLevels; ++i)
			{
				if (i == 0)
					path.startNewSubPath(0.0f, -1.0f);
				else
					path.lineTo((float)i, 1 - 2 * (levels[(nextSample + i) % numLevels].getEnd()));
			}

			path.lineTo((float)numLevels - 1, -1.0f);
			path.closeSubPath();
			break;
		}
		case INPUT_TYPE::RECTLIFIED:
		{
			for (int i = 0; i < numLevels; ++i)
			{
				if (i == 0)
					path.startNewSubPath(0.0f, 1.0f);
				else
					path.lineTo((float)i, 1 - 2 * (levels[(nextSample + i) % numLevels].getEnd()) );
			}

			path.lineTo((float)numLevels - 1, 1.0f);
			path.closeSubPath();
			break;
		}
		case INPUT_TYPE::FULL_WAVE:
		{
			for (int i = 0; i < numLevels; ++i)
			{
				const float level = -0.01f-(levels[(nextSample + i) % numLevels].getEnd());

				if (i == 0)
					path.startNewSubPath(0.0f, level);
				else
					path.lineTo((float)i, level);
			}

			for (int i = numLevels; --i >= 0;)
				path.lineTo((float)i, 0.01f-(levels[(nextSample + i) % numLevels].getStart()));

			path.closeSubPath();
			break;
		}
	}
}


// Paint one channel into area
void RzAudioVisualiserComponent::paintChannel(Graphics& g, Rectangle<float> area,
	const Range<float>* levels, int numLevels, int nextSample)
{
	Path p;
	getChannelAsPath(p, levels, numLevels, nextSample);

	g.fillPath(p, AffineTransform::fromTargetPoints(0.0f, -1.0f, area.getX(), area.getY(),
		0.0f, 1.0f, area.getX(), area.getBottom(),
		(float)numLevels, -1.0f, area.getRight(), area.getY()));

	// Draw grid
	if (this->_showDbGrid)
	{
		const float start = 70.0f;
		const float max = (float)numLevels - 1;
		Path gridDb;
		PathStrokeType pst(1.0f);

		if (INPUT_TYPE::FULL_WAVE == this->_input_type)
		{
			gridDb.startNewSubPath(start, -0.25f);
			gridDb.lineTo(max, -0.25f);
			gridDb.closeSubPath();

			gridDb.startNewSubPath(start, -0.5f);
			gridDb.lineTo(max, -0.5f);
			gridDb.closeSubPath();

			gridDb.startNewSubPath(start, 0.5f);
			gridDb.lineTo(max, 0.5f);
			gridDb.closeSubPath();

			gridDb.startNewSubPath(start, 0.25f);
			gridDb.lineTo(max, 0.25f);
			gridDb.closeSubPath();

			g.setColour(Colour(70, 150, 70));
			g.strokePath(gridDb, pst, AffineTransform::fromTargetPoints(0.0f, -1.0f, area.getX(), area.getY(),
				0.0f, 1.0f, area.getX(), area.getBottom(),
				(float)numLevels, -1.0f, area.getRight(), area.getY()));

			g.setFont(12.0f);
			g.drawSingleLineText("-6 dB", 3, (1 * (int)area.getHeight() / 4) + 5);
			g.drawSingleLineText("-12 dB", 3, (3 * (int)area.getHeight() / 8) + 5);
			g.drawSingleLineText("-12 dB", 3, (5 * (int)area.getHeight() / 8) + 5);
			g.drawSingleLineText("-6 dB", 3, (3 * (int)area.getHeight() / 4) + 5);
		}
		else
		{
			gridDb.startNewSubPath(start, -0.0f);
			gridDb.lineTo(max, -0.0f);
			gridDb.closeSubPath();

			gridDb.startNewSubPath(start, 0.5f);
			gridDb.lineTo(max, 0.5f);
			gridDb.closeSubPath();

			gridDb.startNewSubPath(start, 0.75f);
			gridDb.lineTo(max, 0.75f);
			gridDb.closeSubPath();

			g.setColour(Colour(70, 150, 70));
			g.strokePath(gridDb, pst, AffineTransform::fromTargetPoints(0.0f, -1.0f, area.getX(), area.getY(),
				0.0f, 1.0f, area.getX(), area.getBottom(),
				(float)numLevels, -1.0f, area.getRight(), area.getY()));

			g.setFont(12.0f);
			g.drawSingleLineText("-6 dB", 3, ((int)area.getHeight() / 2) + 5);
			g.drawSingleLineText("-12 dB", 3, (3 * (int)area.getHeight() / 4) + 5);
			g.drawSingleLineText("-18 dB", 3, (7 * (int)area.getHeight() / 8) + 5);
		}
	}
}