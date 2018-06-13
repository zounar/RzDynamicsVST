/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_46084801FE369594__
#define __JUCE_HEADER_46084801FE369594__


#include "../JuceLibraryCode/JuceHeader.h"
#include "RzKnob.h"
#include "RzAudioParameterFloat.h"

class RzGuiEnvelopeFollowerPeak  : public Component
{
public:
	// Create new GUI for envelope follower peak
	RzGuiEnvelopeFollowerPeak(RzAudioParameterFloat& parameter_attack, RzAudioParameterFloat& parameter_release, RzAudioParameterFloat& parameter_correction_gain);

	// Destructor
    ~RzGuiEnvelopeFollowerPeak();

	// Paint the GUI
    void paint (Graphics& g) override;

	// On resize event
    void resized() override;

private:
	ScopedPointer<RzKnob> knob_attack;
	ScopedPointer<RzKnob> knob_release;
	ScopedPointer<RzKnob> knob_correction_gain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzGuiEnvelopeFollowerPeak)
};

#endif