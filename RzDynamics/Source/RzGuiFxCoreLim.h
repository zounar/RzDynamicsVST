/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_6FFF385D9FAE0A8C__
#define __JUCE_HEADER_6FFF385D9FAE0A8C__


#include "../JuceLibraryCode/JuceHeader.h"
#include "RzKnob.h"
#include "RzAudioParameterFloat.h"

class RzGuiFxCoreLim  : public Component
{
public:
	// Create new GUI for fx core limiter
	RzGuiFxCoreLim(RzAudioParameterFloat& parameter_threshold);

	// Destructor
    ~RzGuiFxCoreLim();

	// Paint the GUI
    void paint (Graphics& g) override;

	// On resize event
    void resized() override;
private:
	ScopedPointer<RzKnob> knob_threshold;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzGuiFxCoreLim)
};

#endif