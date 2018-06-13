/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_F7536EB122419BB4__
#define __JUCE_HEADER_F7536EB122419BB4__


#include "../JuceLibraryCode/JuceHeader.h"
#include "RzKnob.h"
#include "RzAudioParameterFloat.h"

class RzGuiVCA  : public Component
{
public:
	// Create new VCA GUI
	RzGuiVCA(RzAudioParameterFloat& parameter_markup_gain);

	// Destructor
    ~RzGuiVCA();

	// Paint the GUI
    void paint (Graphics& g) override;

	// On resize event
    void resized() override;
private:
	ScopedPointer<RzKnob> knob_markup_gain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzGuiVCA)
};

#endif