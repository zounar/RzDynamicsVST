/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzAudioParameterBool.h"
#include "RzAudioProcessor.h"


// Creates a AudioParameterBool with an ID and name
RzAudioParameterBool::RzAudioParameterBool(String pid, String nm, bool def, RzAudioProcessor* processor)
	: AudioProcessorParameterWithID(pid, nm),
	value(def ? 1.0f : 0.0f),
	defaultValue(value),
	processor(processor)
{
}


// Destructor
RzAudioParameterBool::~RzAudioParameterBool() {}


// Get parameter value (0 - 1)
float RzAudioParameterBool::getValue() const 
{ 
	return value; 
}


// Set parameter value
void RzAudioParameterBool::setValue(float newValue) 
{ 
	value = newValue; this->processor->notifyParameterChanged(); 
}


// Get parameter default value
float RzAudioParameterBool::getDefaultValue() const 
{ 
	return defaultValue; 
}


// Get number of parameter steps
int RzAudioParameterBool::getNumSteps() const 
{ 
	return 2; 
}


// Get value for text
float RzAudioParameterBool::getValueForText(const String& text) const 
{ 
	return text.getIntValue() != 0 ? 1.0f : 0.0f; 
}


// Get text from value
String RzAudioParameterBool::getText(float v, int /*length*/) const 
{ 
	return String((int)(v > 0.5f ? 1 : 0));
}


// Set parameter value and notify host
RzAudioParameterBool& RzAudioParameterBool::operator= (bool newValue)
{
	if (get() != newValue)
		setValueNotifyingHost(newValue ? 1.0f : 0.0f);

	return *this;
}