/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef RzAudioParameterInt_H_INCLUDED
#define RzAudioParameterInt_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class RzAudioProcessor;

class RzAudioParameterInt : public AudioProcessorParameterWithID
{
public:
	// Creates an RzAudioParameterInt with an ID, name, and range
	RzAudioParameterInt(String parameterID, String name,
		int minValue, int maxValue,
		int defaultValue,
		RzAudioProcessor* processor);

	// Destructor
	~RzAudioParameterInt();

	// Returns the parameter's current value as an integer
	int get() const noexcept { return roundToInt(value); }

	// Returns the parameter's current value as an integer
	operator int() const noexcept { return get(); }

	// Changes the parameter's current value to a new integer
	RzAudioParameterInt& operator= (int newValue);

	// Returns the parameter's range
	Range<int> getRange() const noexcept { return Range<int>(minValue, maxValue); }
private:
	int minValue, maxValue;
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

	// Return value in between this parameter range
	int limitRange(int) const noexcept;

	// Covert parameter to value from 0 to 1
	float convertTo0to1(int) const noexcept;

	// Convert parameter from 0-1 to full scale
	int convertFrom0to1(float) const noexcept;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RzAudioParameterInt)
};

#endif