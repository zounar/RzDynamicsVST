/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzButton.h"

// Create new button
RzButton::RzButton(String name, String text, bool is_rec, RzAudioParameterBool* parameter) : TextButton(name)
{
	this->setButtonText(text);

	this->setColour(TextButton::textColourOffId, Colour(48, 48, 48));

	if (is_rec)
	{
		this->setLookAndFeel(new RzButtonLookAndFeelRec);
	}
	else
	{
		this->setLookAndFeel(new RzButtonLookAndFeel);
	}


	if (nullptr != parameter)
	{
		this->parameter = parameter;
		startTimerHz(30);
	}
}


// Destructor
RzButton::~RzButton()
{

}


// On timer callback
void RzButton::timerCallback()
{
	this->updateToggle();
}


// On button state change
void RzButton::buttonStateChanged()
{
	if (nullptr != this->parameter)
	{
		*this->parameter = this->getToggleState();
	}
}


// Update toggle state
void RzButton::updateToggle()
{
	const bool newVal = this->parameter->get();

	if (newVal != this->getToggleState())
	{
		this->setToggleState(newVal, NotificationType::dontSendNotification);
	}
}