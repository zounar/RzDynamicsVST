/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_6E0B49676D2AD5DA__
#define __JUCE_HEADER_6E0B49676D2AD5DA__


#include "../JuceLibraryCode/JuceHeader.h"
#include "RzKnob.h"
#include "RzAudioParameterFloat.h"

class RzGuiEnvelopeFollowerRMS  : public Component
{
public:
	// Create new GUI for envelope follower RMS
	RzGuiEnvelopeFollowerRMS(RzAudioParameterFloat& parameter_averaging);

	// Destructor
    ~RzGuiEnvelopeFollowerRMS();

	// Paint the GUI
    void paint (Graphics& g) override;

	// On resize event
    void resized() override;
private:
	ScopedPointer<RzKnob> knob_averaging;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzGuiEnvelopeFollowerRMS)
};

#endif