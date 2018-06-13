/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzGuiDynamicFilterAR.h"


// Create new GUI for dynamic filter AR
RzGuiDynamicFilterAR::RzGuiDynamicFilterAR(RzAudioParameterFloat& parameter_attack, RzAudioParameterFloat& parameter_release)
{
	knob_attack = new RzKnob(parameter_attack);
	knob_attack->setRange(0.1f, 500.0f, 0.1f);
	knob_attack->setSkewFactor(0.3f);
	knob_attack->setTextValueSuffix(" ms");
	addAndMakeVisible(knob_attack);

	knob_release = new RzKnob(parameter_release);
	knob_release->setRange(0.1f, 500.0f, 0.1f);
	knob_release->setSkewFactor(0.3f);
	knob_release->setTextValueSuffix(" ms");
	addAndMakeVisible(knob_release);

	setSize(620, 113);
}


// Destructor
RzGuiDynamicFilterAR::~RzGuiDynamicFilterAR()
{
	knob_attack = nullptr;
	knob_release = nullptr;
}


// Paint the GUI
void RzGuiDynamicFilterAR::paint (Graphics& g)
{
    g.setColour (Colour (0xff535353));
	g.drawRoundedRectangle(0.0f, 0.0f, 620.0f, 113.0f, 10.000f, 1.000f);

	g.setColour(Colour(48, 48, 48));
    g.setFont (Font ("Arial", 15.0f, Font::bold));
    g.drawText ("Dynamic filter AR",
				212, 16, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Attack",
				182, 38, 78, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Release",
				366, 38, 78, 30,
                Justification::centred, true);
}


// On resize event
void RzGuiDynamicFilterAR::resized()
{
	knob_attack->setBounds(160, 60, 116, 48);
	knob_release->setBounds(340, 60, 116, 48);
}