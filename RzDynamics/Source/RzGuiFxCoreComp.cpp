/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzGuiFxCoreComp.h"


// Create new GUI for fx core compressor
RzGuiFxCoreComp::RzGuiFxCoreComp(RzAudioParameterFloat& parameter_threshold, RzAudioParameterFloat& parameter_ratio)
{
	knob_threshold = new RzKnob(parameter_threshold);
	knob_threshold->setRange(-50.0f, 0.0f, 0.1f);
	knob_threshold->setSkewFactor(2.0f);
	knob_threshold->setTextValueSuffix(" dB");
	addAndMakeVisible(knob_threshold);

	knob_ratio = new RzKnob(parameter_ratio);
	knob_ratio->setRange(1.0f, 20.0f, 0.1f);
	knob_ratio->setSkewFactor(0.3f);
	knob_ratio->setTextValueSuffix(":1");
	addAndMakeVisible(knob_ratio);

	setSize(620, 113);
}


// Destructor
RzGuiFxCoreComp::~RzGuiFxCoreComp()
{
	knob_threshold = nullptr;
	knob_ratio = nullptr;
}


// Paint the GUI
void RzGuiFxCoreComp::paint (Graphics& g)
{
    g.setColour (Colour (0xff535353));
	g.drawRoundedRectangle(0.0f, 0.0f, 620.0f, 113.0f, 10.000f, 1.000f);

	g.setColour(Colour(48, 48, 48));
    g.setFont (Font ("Arial", 15.0f, Font::bold));
    g.drawText ("Fx core compressor",
				212, 16, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Threshold",
				186, 38, 78, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Ratio",
				366, 38, 78, 30,
                Justification::centred, true);
}


// On resize event
void RzGuiFxCoreComp::resized()
{
	knob_threshold->setBounds(160, 60, 116, 48);
	knob_ratio->setBounds(340, 60, 116, 48);
}