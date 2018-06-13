/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_211AA73C54CB828C__
#define __JUCE_HEADER_211AA73C54CB828C__

#include "../JuceLibraryCode/JuceHeader.h"
#include "RzAudioParameterFloat.h"

class RzKnob  : public Slider, private Timer
{
public:
	// Create new knob
	RzKnob(RzAudioParameterFloat& parameter);

	// Destructor
    ~RzKnob();

	// On timer callback
	void timerCallback() override;

	// Set parameter on value change
	void valueChanged() override;

	// On drag start event
	void startedDragging() override;

	// On drag end event
	void stoppedDragging() override;

	// Update slider position by parameter value
	void updateSliderPos();

	// Paint the knob
	void paint(Graphics& g) override;
private:
	static const char* knobfull_png;
	static const int knobfull_pngSize;
    Image cachedImage_knobfull_png_1;

	// Knob indicator - small line showing current value
	Path knob_indicator;

	// Parameter bound
	RzAudioParameterFloat& parameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzKnob)
};

#endif