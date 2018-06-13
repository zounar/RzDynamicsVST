/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef RzAudioParameterFloat_H_INCLUDED
#define RzAudioParameterFloat_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class RzAudioProcessor;

class  RzAudioParameterFloat : public AudioProcessorParameterWithID
{
public:
	// Creates a RzAudioParameterFloat with an ID, name, and range
	RzAudioParameterFloat(String parameterID, String name,
		NormalisableRange<float> normalisableRange,
		float defaultValue, RzAudioProcessor* processor);

	// Creates a RzAudioParameterFloat with an ID, name, and range
	RzAudioParameterFloat(String parameterID, String name,
		float minValue,
		float maxValue,
		float defaultValue, 
		RzAudioProcessor* processor);

	// Destructor
	~RzAudioParameterFloat();

	// Returns the parameter's current value
	float get() const noexcept { return value; }

	// Returns the parameter's current value
	operator float() const noexcept { return value; }

	// Changes the parameter's current value
	RzAudioParameterFloat& operator= (float newValue);

	// Provides access to the parameter's range
	NormalisableRange<float> range;
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

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RzAudioParameterFloat)
};

#endif