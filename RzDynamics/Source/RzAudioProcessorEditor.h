/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "RzAudioProcessor.h"
#include "RzComboBox.h"
#include "RzKnob.h"
#include "RzGuiEnvelopeFollowerPeak.h"
#include "RzGuiEnvelopeFollowerRMS.h"
#include "RzGuiFxCoreComp.h"
#include "RzGuiFxCoreExp.h"
#include "RzGuiFxCoreLim.h"
#include "RzGuiFxCoreGate.h"
#include "RzGuiDynamicFilterAR.h"
#include "RzGuiDynamicFilterARH.h"
#include "RzAudioVisualiserComponent.h"
#include "RzGuiVCA.h"
#include "RzButton.h"
#include "RzInfoWindow.h"

using namespace RZ;

class RzAudioProcessorEditor :	
	public AudioProcessorEditor, 
	private MultiTimer, 
	private ComboBox::Listener,
	private Button::Listener
{
public:
	// Create new processor editor
	RzAudioProcessorEditor(RzAudioProcessor &p);

	// Destructor - instance is using ScopedPointer, theres no need to care for its instances
    ~RzAudioProcessorEditor();

	// On timer callback - update visualisers
	void timerCallback(int timerID) override;
	
	// Paint component
	void paint(Graphics&) override;

	// Switch GUIS
	void switchGUIS();

	// Switch GUIs based on combo box values
	void comboBoxChanged(ComboBox* combobox) override;

	// On button click event
	void buttonClicked(Button* button) override;
private:
	unsigned char lastProcessingTick = 0;

	RzAudioProcessor& processor;

	ScopedPointer<RzComboBox> combobox_envelope;
	ScopedPointer<RzComboBox> combobox_core;
	ScopedPointer<RzComboBox> combobox_dyn;
	ScopedPointer<RzButton> button_info;
	ScopedPointer<RzButton> button_record;
	ScopedPointer<RzInfoWindow> info_window;

	ScopedPointer<RzGuiEnvelopeFollowerPeak> envelope_peak;
	ScopedPointer<RzGuiEnvelopeFollowerRMS> envelope_rms;
	ScopedPointer<RzGuiFxCoreComp> fx_core_comp;
	ScopedPointer<RzGuiFxCoreExp> fx_core_exp;
	ScopedPointer<RzGuiFxCoreLim> fx_core_lim;
	ScopedPointer<RzGuiFxCoreGate> fx_core_gate;
	ScopedPointer<RzGuiDynamicFilterAR> dyn_filter_ar;
	ScopedPointer<RzGuiDynamicFilterARH> dyn_filter_arh;
	ScopedPointer<RzGuiVCA> vca;
	
	ScopedPointer<RzAudioVisualiserComponent> visualiser_envelope;
	ScopedPointer<RzAudioVisualiserComponent> visualiser_core;
	ScopedPointer<RzAudioVisualiserComponent> visualiser_dyn;
	ScopedPointer<RzAudioVisualiserComponent> visualiser_out;

	ScopedPointer<Label> last_recorded_file_name;

	static const char* background_image;
	static const int background_size;
	Image cached_background;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzAudioProcessorEditor)
};

#endif