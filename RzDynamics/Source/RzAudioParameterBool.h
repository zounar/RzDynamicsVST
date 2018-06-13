/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef RzAudioParameterBool_H_INCLUDED
#define RzAudioParameterBool_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class RzAudioProcessor;

class RzAudioParameterBool : public AudioProcessorParameterWithID
{
public:
	// Creates a AudioParameterBool with an ID and name
	RzAudioParameterBool(String parameterID, String name, bool defaultValue, RzAudioProcessor* processor);

	// Destructor
	~RzAudioParameterBool();

	// Get parameter boolean value
	bool get() const noexcept { return value >= 0.5f; }

	// Get parameter boolean value
	operator bool() const noexcept { return get(); }

	// Set parameter value and notify host
	RzAudioParameterBool& operator= (bool newValue);
private:
	float value, defaultValue;

	// Processor to call when value is changed
	RzAudioProcessor* processor;

	// Get parameter value (0 - 1)
	float getValue() const override;

	// Set parameter value
	void setValue(float newValue) override;

	// Get parameter default value
	float getDefaultValue() const override;

	// Get number of parameter steps
	int getNumSteps() const override;

	// Get value for text
	String getText(float, int) const override;

	// Get text from value
	float getValueForText(const String&) const override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RzAudioParameterBool)
};

#endif