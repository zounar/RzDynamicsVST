/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_F7536EB1221889BB4__
#define __JUCE_HEADER_F7536EB1221889BB4__

#include "../JuceLibraryCode/JuceHeader.h"
#include "RzAudioParameterBool.h"

class RzButton  : public TextButton, private Timer
{
public:
	// Create new button
	RzButton(String name, String text, bool is_rec = false, RzAudioParameterBool* parameter = nullptr);

	// Destructor
    ~RzButton();

	// On timer callback
	void timerCallback() override;

	// On button state change
	void buttonStateChanged() override;

	// Update toggle state
	void updateToggle();
private:
	// Bound parameter
	RzAudioParameterBool* parameter = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzButton)
};


// Custom look and feel for most of buttons
class RzButtonLookAndFeel : public LookAndFeel_V3
{
	void drawButtonText(Graphics& g, TextButton& button, bool /*isMouseOverButton*/, bool /*isButtonDown*/)
	{
		Font font(getTextButtonFont(button, button.getHeight() - 5));
		font.setBold(true);
		font.setItalic(true);
		font.setHeight(20.0f);
		g.setFont(font);
		g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
			: TextButton::textColourOffId)
			.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));
		

		const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
		const int cornerSize = jmin(button.getHeight() - 5, button.getWidth() - 5) / 2;

		const int fontHeight = roundToInt(font.getHeight() * 0.6f);
		const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
		const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
		const int textWidth = button.getWidth() - 5 - leftIndent - rightIndent;

		if (textWidth > 0)
			g.drawFittedText(button.getButtonText(),
				leftIndent, yIndent, textWidth, button.getHeight() - 5 - yIndent * 2,
				Justification::centred, 2);
	}

	void drawButtonBackground(Graphics& g,
		Button& button,
		const Colour& /*backgroundColour*/,
		bool isMouseOverButton,
		bool isButtonDown)
	{
		const float rect_width = (float)button.getWidth() - 5;
		const float rect_height = (float)button.getHeight() - 5;
		Rectangle<float> rect(rect_width, rect_height);
		Rectangle<int> recti((int)rect_width, (int)rect_height);	
		DropShadow shadow;
		shadow.colour = Colour(150, 150, 150);
		shadow.offset = Point<int>(2, 2);
		shadow.radius = 5;
		shadow.drawForRectangle(g, recti);

		if (isButtonDown)
		{
			g.setColour(Colour(140, 147, 157));
		}
		else if (isMouseOverButton)
		{
			g.setColour(Colour(155, 162, 172));
		}
		else
		{
			g.setColour(Colour(170, 177, 186));
		}
		g.fillRoundedRectangle(rect, 7.0f);

		g.setColour(Colour(0xff535353));
		g.drawRoundedRectangle(rect, 7.0f, 1.0f);
	}
};


// Custom look and feel for rec button
class RzButtonLookAndFeelRec : public LookAndFeel_V3
{
	void drawButtonText(Graphics& g, TextButton& button, bool /*isMouseOverButton*/, bool /*isButtonDown*/)
	{
		Font font(getTextButtonFont(button, button.getHeight() - 5));
		font.setBold(true);
		font.setHeight(25.0f);
		g.setFont(font);
		g.setColour(button.findColour(button.getToggleState() ? TextButton::textColourOnId
			: TextButton::textColourOffId)
			.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f));


		const int yIndent = jmin(4, button.proportionOfHeight(0.3f));
		const int cornerSize = jmin(button.getHeight() - 5, button.getWidth() - 5) / 2;

		const int fontHeight = roundToInt(font.getHeight() * 0.6f);
		const int leftIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnLeft() ? 4 : 2));
		const int rightIndent = jmin(fontHeight, 2 + cornerSize / (button.isConnectedOnRight() ? 4 : 2));
		const int textWidth = button.getWidth() - 5 - leftIndent - rightIndent;

		if (textWidth > 0)
			g.drawFittedText(button.getButtonText(),
				leftIndent, yIndent, textWidth, button.getHeight() - 5 - yIndent * 2,
				Justification::centred, 2);
	}

	void drawButtonBackground(Graphics& g,
		Button& button,
		const Colour& /*backgroundColour*/,
		bool isMouseOverButton,
		bool isButtonDown)
	{
		const float rect_width = (float)button.getWidth() - 5;
		const float rect_height = (float)button.getHeight() - 5;
		Rectangle<float> rect(rect_width, rect_height);
		Rectangle<int> recti((int)rect_width, (int)rect_height);
		DropShadow shadow;
		shadow.colour = Colour(150, 150, 150);
		shadow.offset = Point<int>(2, 2);
		shadow.radius = 5;
		shadow.drawForRectangle(g, recti);

		if (button.getToggleState())
		{
			g.setColour(Colour(193, 69, 74));
		}
		else if (isButtonDown)
		{
			g.setColour(Colour(140, 147, 157));
		}
		else if (isMouseOverButton)
		{
			g.setColour(Colour(155, 162, 172));
		}
		else
		{
			g.setColour(Colour(170, 177, 186));
		}
		g.fillRoundedRectangle(rect, 7.0f);

		g.setColour(Colour(0xff535353));
		g.drawRoundedRectangle(rect, 7.0f, 1.0f);
	}
};

#endif