/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_A9B06CF092CBA90C__
#define __JUCE_HEADER_A9B06CF092CBA90C__


#include "../JuceLibraryCode/JuceHeader.h"
#include "RzKnob.h"
#include "RzAudioParameterFloat.h"

class RzGuiDynamicFilterARH : public Component
{
public:
	// Create new GUI for dynamic filter ARH
	RzGuiDynamicFilterARH(RzAudioParameterFloat& parameter_attack, RzAudioParameterFloat& parameter_release, RzAudioParameterFloat& parameter_hold);

	// Destructor
    ~RzGuiDynamicFilterARH();

	// Paint the GUI
    void paint (Graphics& g) override;

	// On resize event
    void resized() override;
private:
	ScopedPointer<RzKnob> knob_attack;
	ScopedPointer<RzKnob> knob_release;
	ScopedPointer<RzKnob> knob_hold;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzGuiDynamicFilterARH)
};

#endif