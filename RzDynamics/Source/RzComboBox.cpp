/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzComboBox.h"


// Create new combobox
RzComboBox::RzComboBox(RzAudioParameterInt& parameter) : parameter(parameter)
{
	this->setJustificationType(Justification::centred);
	this->setColour(ComboBox::backgroundColourId, Colour(170, 177, 186));
	this->setColour(ComboBox::buttonColourId, Colour(0xff535353));
	this->setColour(ComboBox::textColourId, Colour(48, 48, 48));
	startTimerHz(30);
	updateValue();
}


// Timer callback updates value from parameter value
void RzComboBox::timerCallback()
{
	updateValue();
}


// On value changed callback
void RzComboBox::valueChanged(Value& value)
{
	this->parameter = value.getValue();
}


// Update value from parameter value
void RzComboBox::updateValue()
{
	const int newValue = (int)parameter;

	if (newValue != (int)this->getSelectedId() && !isMouseButtonDown())
	{
		this->setSelectedId(newValue);
	}
}