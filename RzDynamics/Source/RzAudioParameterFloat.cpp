/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzAudioParameterFloat.h"
#include "RzAudioProcessor.h"


// Creates a RzAudioParameterFloat with an ID, name, and range
RzAudioParameterFloat::RzAudioParameterFloat(String pid, String nm, NormalisableRange<float> r, float def, RzAudioProcessor* processor)
	: AudioProcessorParameterWithID(pid, nm), range(r), value(def), defaultValue(def), processor(processor)
{
}


// Creates a RzAudioParameterFloat with an ID, name, and range
RzAudioParameterFloat::RzAudioParameterFloat(String pid, String nm, float minValue, float maxValue, float def, RzAudioProcessor* processor)
	: AudioProcessorParameterWithID(pid, nm), range(minValue, maxValue), value(def), defaultValue(def), processor(processor)
{
}


// Destructor
RzAudioParameterFloat::~RzAudioParameterFloat() {}


// Get parameter value (0 - 1)
float RzAudioParameterFloat::getValue() const 
{ 
	return range.convertTo0to1(value); 
}


// Set parameter value
void RzAudioParameterFloat::setValue(float newValue) 
{ 
	value = range.convertFrom0to1(newValue); 
	this->processor->notifyParameterChanged(); 
}


// Get parameter default value
float RzAudioParameterFloat::getDefaultValue() const 
{ 
	return range.convertTo0to1(defaultValue); 
}


// Get number of parameter steps
int RzAudioParameterFloat::getNumSteps() const 
{ 
	return AudioProcessorParameterWithID::getNumSteps(); 
}


// Get value for text
float RzAudioParameterFloat::getValueForText(const String& text) const 
{ 
	return range.convertTo0to1(text.getFloatValue()); 
}


// Get text from value
String RzAudioParameterFloat::getText(float v, int length) const
{
	String asText(range.convertFrom0to1(v), 2);
	return length > 0 ? asText.substring(0, length) : asText;
}


// Changes the parameter's current value
RzAudioParameterFloat& RzAudioParameterFloat::operator= (float newValue)
{
	if (value != newValue)
		setValueNotifyingHost(range.convertTo0to1(newValue));

	return *this;
}