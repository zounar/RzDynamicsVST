/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzGuiFxCoreGate.h"


// Create new GUI for fx core gate
RzGuiFxCoreGate::RzGuiFxCoreGate(RzAudioParameterFloat& parameter_threshold)
{
	knob_threshold = new RzKnob(parameter_threshold);
	knob_threshold->setRange(-50.0f, 0.0f, 0.1f);
	knob_threshold->setSkewFactor(2.0f);
	knob_threshold->setTextValueSuffix(" dB");
	addAndMakeVisible(knob_threshold);

	setSize(620, 113);
}


// Destructor
RzGuiFxCoreGate::~RzGuiFxCoreGate()
{
	knob_threshold = nullptr;
}


// Paint the GUI
void RzGuiFxCoreGate::paint (Graphics& g)
{
    g.setColour (Colour (0xff535353));
	g.drawRoundedRectangle(0.0f, 0.0f, 620.0f, 113.0f, 10.000f, 1.000f);

	g.setColour(Colour(48, 48, 48));
    g.setFont (Font ("Arial", 15.0f, Font::bold));
    g.drawText ("Fx core gate",
				212, 16, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Threshold",
				212, 38, 200, 30,
                Justification::centred, true);
}


// On resize event
void RzGuiFxCoreGate::resized()
{
	knob_threshold->setBounds(245, 60, 116, 48);
}