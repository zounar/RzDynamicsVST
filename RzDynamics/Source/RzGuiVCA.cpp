/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzGuiVCA.h"


// Create new VCA GUI
RzGuiVCA::RzGuiVCA(RzAudioParameterFloat& parameter_markup_gain)
{
	knob_markup_gain = new RzKnob(parameter_markup_gain);
	knob_markup_gain->setRange(-20.0f, 20.0f, 0.1f);
	knob_markup_gain->setTextValueSuffix(" dB");
	addAndMakeVisible(knob_markup_gain);

	setSize(620, 113);
}


// Destructor
RzGuiVCA::~RzGuiVCA()
{
	knob_markup_gain = nullptr;
}


// Paint the GUI
void RzGuiVCA::paint (Graphics& g)
{
    g.setColour (Colour (0xff535353));
	g.drawRoundedRectangle(0.0f, 0.0f, 620.0f, 113.0f, 10.000f, 1.000f);

	g.setColour(Colour(48, 48, 48));
    g.setFont (Font ("Arial", 15.0f, Font::bold));
    g.drawText ("VCA",
				212, 16, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Markup gain",
				212, 38, 200, 30,
                Justification::centred, true);
}


// On resize event
void RzGuiVCA::resized()
{
	knob_markup_gain->setBounds(245, 60, 116, 48);
}