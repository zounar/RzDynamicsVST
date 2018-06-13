/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_772E967BA755A558__
#define __JUCE_HEADER_772E967BA755A558__


#include "../JuceLibraryCode/JuceHeader.h"
#include "RzKnob.h"
#include "RzAudioParameterFloat.h"

class RzGuiFxCoreExp : public Component
{
public:
	// Create new GUI for fx core expander
	RzGuiFxCoreExp(RzAudioParameterFloat& parameter_threshold_open, RzAudioParameterFloat& parameter_threshold_close, RzAudioParameterFloat& parameter_range);

	// Destructor
    ~RzGuiFxCoreExp();

	// Paint the GUI
    void paint (Graphics& g) override;

	// On resize event
    void resized() override;
private:
	ScopedPointer<RzKnob> knob_threshold;
	ScopedPointer<RzKnob> knob_ratio;
	ScopedPointer<RzKnob> knob_range;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzGuiFxCoreExp)
};

#endif