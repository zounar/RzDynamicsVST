/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzAudioParameterInt.h"
#include "RzAudioProcessor.h"

// Creates an RzAudioParameterInt with an ID, name, and range
RzAudioParameterInt::RzAudioParameterInt(String pid, String nm, int mn, int mx, int def, RzAudioProcessor* processor)
	: AudioProcessorParameterWithID(pid, nm),
	minValue(mn), maxValue(mx),
	value((float)def),
	defaultValue(convertTo0to1(def)),
	processor(processor)
{
	jassert(minValue < maxValue); // must have a non-zero range of values!
}


// Destructor
RzAudioParameterInt::~RzAudioParameterInt() {}


// Get parameter value (0 - 1)
float RzAudioParameterInt::getValue() const 
{ 
	return convertTo0to1(roundToInt(value)); 
}


// Set parameter value
void RzAudioParameterInt::setValue(float newValue) 
{ 
	value = (float)convertFrom0to1(newValue); 
	this->processor->notifyParameterChanged(); 
}


// Get parameter default value
float RzAudioParameterInt::getDefaultValue() const 
{ 
	return defaultValue; 
}


// Get number of parameter steps
int RzAudioParameterInt::getNumSteps() const 
{ 
	return AudioProcessorParameterWithID::getNumSteps(); 
}


// Get value for text
float RzAudioParameterInt::getValueForText(const String& text) const 
{ 
	return convertTo0to1(text.getIntValue()); 
}


// Get text from value
String RzAudioParameterInt::getText(float v, int /*length*/) const 
{ 
	return String(convertFrom0to1(v)); 
}


// Changes the parameter's current value to a new integer
RzAudioParameterInt& RzAudioParameterInt::operator= (int newValue)
{
	if (get() != newValue)
		setValueNotifyingHost(convertTo0to1(newValue));

	return *this;
}


// Return value in between this parameter range
int RzAudioParameterInt::limitRange(int v) const noexcept 
{ 
	return jlimit(minValue, maxValue, v); 
}


// Covert parameter to value from 0 to 1
float RzAudioParameterInt::convertTo0to1(int v) const noexcept 
{ 
	return (limitRange(v) - minValue) / (float)(maxValue - minValue); 
}


// Convert parameter from 0-1 to full scale
int RzAudioParameterInt::convertFrom0to1(float v) const noexcept 
{ 
	return limitRange(roundToInt((v * (float)(maxValue - minValue)) + minValue)); 
}