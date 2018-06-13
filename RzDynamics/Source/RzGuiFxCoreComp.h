/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_6FFF385D8FAE0A8C__
#define __JUCE_HEADER_6FFF385D8FAE0A8C__


#include "../JuceLibraryCode/JuceHeader.h"
#include "RzKnob.h"
#include "RzAudioParameterFloat.h"

class RzGuiFxCoreComp  : public Component
{
public:
	// Create new GUI for fx core compressor
	RzGuiFxCoreComp(RzAudioParameterFloat& parameter_threshold, RzAudioParameterFloat& parameter_ratio);

	// Destructor
    ~RzGuiFxCoreComp();

	// Paint the GUI
    void paint (Graphics& g) override;

	// On resize event
    void resized() override;
private:
	ScopedPointer<RzKnob> knob_threshold;
	ScopedPointer<RzKnob> knob_ratio;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzGuiFxCoreComp)
};

#endif