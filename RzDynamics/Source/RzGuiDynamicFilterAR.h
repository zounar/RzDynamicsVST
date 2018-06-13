/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_F7536EB332419BB4__
#define __JUCE_HEADER_F7536EB332419BB4__

#include "../JuceLibraryCode/JuceHeader.h"
#include "RzKnob.h"
#include "RzAudioParameterFloat.h"

class RzGuiDynamicFilterAR : public Component
{
public:
	// Create new GUI for dynamic filter AR
	RzGuiDynamicFilterAR(RzAudioParameterFloat& parameter_attack, RzAudioParameterFloat& parameter_release);

	// Destructor
    ~RzGuiDynamicFilterAR();

	// Paint the GUI
    void paint (Graphics& g) override;

	// On resize event
    void resized() override;
private:
	ScopedPointer<RzKnob> knob_attack;
	ScopedPointer<RzKnob> knob_release;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzGuiDynamicFilterAR)
};

#endif