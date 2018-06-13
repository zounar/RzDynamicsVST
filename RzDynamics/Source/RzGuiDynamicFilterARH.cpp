/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzGuiDynamicFilterARH.h"


// Create new GUI for dynamic filter ARH
RzGuiDynamicFilterARH::RzGuiDynamicFilterARH(RzAudioParameterFloat& parameter_attack, RzAudioParameterFloat& parameter_release, RzAudioParameterFloat& parameter_hold)
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

	knob_hold = new RzKnob(parameter_hold);
	knob_hold->setRange(0.1f, 500.0f, 0.1f);
	knob_hold->setSkewFactor(0.3f);
	knob_hold->setTextValueSuffix(" ms");
	addAndMakeVisible(knob_hold);

	setSize(620, 113);
}


// Destructor
RzGuiDynamicFilterARH::~RzGuiDynamicFilterARH()
{
	knob_attack = nullptr;
	knob_release = nullptr;
	knob_hold = nullptr;
}


// Paint the GUI
void RzGuiDynamicFilterARH::paint (Graphics& g)
{
    g.setColour (Colour (0xff535353));
	g.drawRoundedRectangle(0.0f, 0.0f, 620.0f, 113.0f, 10.000f, 1.000f);

	g.setColour(Colour(48, 48, 48));
    g.setFont (Font ("Arial", 15.0f, Font::bold));
    g.drawText ("Dynamic filter ARH",
				212, 16, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Attack",
				90, 38, 78, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Release",
				217, 38, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Hold",
				452, 38, 78, 30,
                Justification::centred, true);
}


// On resize event
void RzGuiDynamicFilterARH::resized()
{
	knob_attack->setBounds(60, 60, 116, 48);
	knob_release->setBounds(245, 60, 116, 48);
	knob_hold->setBounds(430, 60, 116, 48);
}