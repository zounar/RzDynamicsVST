/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzGuiFxCoreExp.h"


// Create new GUI for fx core expander
RzGuiFxCoreExp::RzGuiFxCoreExp(RzAudioParameterFloat& parameter_threshold, RzAudioParameterFloat& parameter_ratio, RzAudioParameterFloat& parameter_range)
{
	knob_threshold = new RzKnob(parameter_threshold);
	knob_threshold->setRange(-50.0f, 0.0f, 0.1f);
	knob_threshold->setSkewFactor(2.0f);
	knob_threshold->setTextValueSuffix(" dB");
	addAndMakeVisible(knob_threshold);

	knob_ratio = new RzKnob(parameter_ratio);
	knob_ratio->setRange(0.05f, 1.0f, 0.01f);
	knob_ratio->setSkewFactor(2.0f);
	knob_ratio->setTextValueSuffix(" :1");
	addAndMakeVisible(knob_ratio);

	knob_range = new RzKnob(parameter_range);
	knob_range->setRange(-72.0f, 0.0f, 0.1f);
	knob_range->setSkewFactor(2.0f);
	knob_range->setTextValueSuffix(" dB");
	addAndMakeVisible(knob_range);

	setSize(620, 113);
}


// Destructor
RzGuiFxCoreExp::~RzGuiFxCoreExp()
{
	knob_threshold = nullptr;
	knob_ratio = nullptr;
	knob_range = nullptr;
}


// Paint the GUI
void RzGuiFxCoreExp::paint (Graphics& g)
{
    g.setColour (Colour (0xff535353));
	g.drawRoundedRectangle(0.0f, 0.0f, 620.0f, 113.0f, 10.000f, 1.000f);

	g.setColour(Colour(48, 48, 48));
    g.setFont (Font ("Arial", 15.0f, Font::bold));
    g.drawText ("Fx core expander",
				212, 16, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Threshold",
                90, 38, 78, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Ratio",
				217, 38, 200, 30,
                Justification::centred, true);

    g.setFont (Font ("Arial", 13.00f, Font::bold));
    g.drawText ("Range",
                452, 38, 78, 30,
                Justification::centred, true);
}


// On resize event
void RzGuiFxCoreExp::resized()
{
	knob_threshold->setBounds(60, 60, 116, 48);
	knob_ratio->setBounds(245, 60, 116, 48);
	knob_range->setBounds (430, 60, 116, 48);
}