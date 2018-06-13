/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzGuiEnvelopeFollowerPeak.h"


// Create new GUI for envelope follower peak
RzGuiEnvelopeFollowerPeak::RzGuiEnvelopeFollowerPeak(RzAudioParameterFloat& parameter_attack, RzAudioParameterFloat& parameter_release, RzAudioParameterFloat& parameter_correction_gain)
{
	knob_attack = new RzKnob(parameter_attack);
	knob_attack->setRange(0.01f, 500.0f, 0.01f);
	knob_attack->setSkewFactor(0.3f);
	knob_attack->setTextValueSuffix(" ms");
	addAndMakeVisible(knob_attack);
	
	knob_release = new RzKnob(parameter_release);
	knob_release->setRange(0.01f, 500.0f, 0.01f);
	knob_release->setSkewFactor(0.3f);
	knob_release->setTextValueSuffix(" ms");
	addAndMakeVisible(knob_release);

	knob_correction_gain = new RzKnob(parameter_correction_gain);
	knob_correction_gain->setRange(-12.0f, 12.0f, 0.1f);
	knob_correction_gain->setTextValueSuffix(" dB");
	addAndMakeVisible(knob_correction_gain);

    setSize (620, 113);
}


// Destructor
RzGuiEnvelopeFollowerPeak::~RzGuiEnvelopeFollowerPeak()
{
	knob_attack = nullptr;
	knob_release = nullptr;
}


// Paint the GUI
void RzGuiEnvelopeFollowerPeak::paint (Graphics& g)
{
    g.setColour (Colour (0xff535353));
    g.drawRoundedRectangle (0.0f, 0.0f, 620.0f, 113.0f, 10.000f, 1.000f);

	g.setColour(Colour(48, 48, 48));
    g.setFont (Font ("Arial", 15.0f, Font::bold));
    g.drawText ("Envelope follower peak",
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

	g.setFont(Font("Arial", 13.00f, Font::bold));
	g.drawText("Correction gain",
				452, 38, 88, 30,
				Justification::centred, true);
}


// On resize event
void RzGuiEnvelopeFollowerPeak::resized()
{
	knob_attack->setBounds(60, 60, 116, 48);
	knob_release->setBounds(245, 60, 116, 48);
	knob_correction_gain->setBounds(430, 60, 116, 48);
}