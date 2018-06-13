/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_211AABS73C54CB828C__
#define __JUCE_HEADER_211AABS73C54CB828C__

#include "../JuceLibraryCode/JuceHeader.h"
#include "RzAudioParameterInt.h"

class RzComboBox : public ComboBox, private Timer
{
public:
	// Create new combobox
	RzComboBox(RzAudioParameterInt& parameter);
	~RzComboBox() {}

	// Timer callback updates value from parameter value
	void timerCallback() override;

	// On value changed callback
	void valueChanged(Value &) override;

	// Update value from parameter value
	void updateValue();

private:
	// Parameter that controls value of this combobox
	RzAudioParameterInt& parameter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RzComboBox)
};

#endif