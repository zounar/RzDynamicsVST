/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzGuiEnvelopeFollowerRMS.h"


// Create new GUI for envelope follower RMS
RzGuiEnvelopeFollowerRMS::RzGuiEnvelopeFollowerRMS(RzAudioParameterFloat& parameter_averaging)
{
	knob_averaging = new RzKnob(parameter_averaging);
	knob_averaging->setRange(0.01f, 100.0f, 0.01f);
	knob_averaging->setSkewFactor(0.3f);
	knob_averaging->setTextValueSuffix(" ms");
    addAndMakeVisible (knob_averaging);
	
	setSize(620, 113);
}


// Destructor
RzGuiEnvelopeFollowerRMS::~RzGuiEnvelopeFollowerRMS()
{
    knob_averaging = nullptr;
}


// Paint the GUI
void RzGuiEnvelopeFollowerRMS::paint (Graphics& g)
{
    g.setColour (Colour (0xff535353));
	g.drawRoundedRectangle(0.0f, 0.0f, 620.0f, 113.0f, 10.000f, 1.000f);

	g.setColour(Colour(48, 48, 48));
    g.setFont (Font ("Arial", 15.0f, Font::bold));
    g.drawText ("Envelope follower RMS",
				212, 16, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Averaging",
				212, 38, 200, 30,
                Justification::centred, true);
}


// On resize event
void RzGuiEnvelopeFollowerRMS::resized()
{
	knob_averaging->setBounds (245, 60, 116, 48);
}