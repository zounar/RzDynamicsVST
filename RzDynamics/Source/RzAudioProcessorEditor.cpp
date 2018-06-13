/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzAudioProcessorEditor.h"
#include "RzAudioProcessor.h"

// Create new editor
RzAudioProcessorEditor::RzAudioProcessorEditor(RzAudioProcessor& p) : AudioProcessorEditor(&p), processor(p)
{
	setSize(1000, 625);

	// Load background image
	cached_background = ImageCache::getFromMemory(this->background_image, this->background_size);
	
	// Envelope peak component
	this->envelope_peak = new RzGuiEnvelopeFollowerPeak(*processor.envelope_peak_attack, *processor.envelope_peak_release, *processor.envelope_peak_correction_gain);
	this->envelope_peak->setBounds(31, 95, 620, 113);
	addChildComponent(this->envelope_peak);

	// Add envelope RMS component
	this->envelope_rms = new RzGuiEnvelopeFollowerRMS(*processor.envelope_rms_averaging);
	this->envelope_rms->setBounds(31, 95, 620, 113);
	addChildComponent(this->envelope_rms);

	// Add FX core compressor component
	this->fx_core_comp = new RzGuiFxCoreComp(*processor.fx_core_comp_threshold, *processor.fx_core_comp_ratio);
	this->fx_core_comp->setBounds(31, 221, 620, 113);
	addChildComponent(this->fx_core_comp);

	// Add FX core expander component
	this->fx_core_exp = new RzGuiFxCoreExp(*processor.fx_core_expander_threshold, *processor.fx_core_expander_ratio, *processor.fx_core_expander_range);
	this->fx_core_exp->setBounds(31, 221, 620, 113);
	addChildComponent(this->fx_core_exp);

	// Add FX core limiter component
	this->fx_core_lim = new RzGuiFxCoreLim(*processor.fx_core_limiter_threshold);
	this->fx_core_lim->setBounds(31, 221, 620, 113);
	addChildComponent(this->fx_core_lim);

	// Add Fx core gate component
	this->fx_core_gate = new RzGuiFxCoreGate(*processor.fx_core_gate_threshold);
	this->fx_core_gate->setBounds(31, 221, 620, 113);
	addChildComponent(this->fx_core_gate);

	// Add dynamic filter AR component
	this->dyn_filter_ar = new RzGuiDynamicFilterAR(*processor.dyn_ar_attack, *processor.dyn_ar_release);
	this->dyn_filter_ar->setBounds(31, 347, 620, 113);
	addChildComponent(this->dyn_filter_ar);

	// Add dynamic filter ARH component
	this->dyn_filter_arh = new RzGuiDynamicFilterARH(*processor.dyn_arh_attack, *processor.dyn_arh_release, *processor.dyn_arh_hold);
	this->dyn_filter_arh->setBounds(31, 347, 620, 113);
	addChildComponent(this->dyn_filter_arh);

	// Add VCA component
	this->vca = new RzGuiVCA(*processor.markup_gain);
	this->vca->setBounds(31, 474, 620, 113);
	addAndMakeVisible(this->vca);
	
	// Add info button
	this->button_info = new RzButton("button_info", "i");
	this->button_info->setBounds(939, 35, 35, 35);
	addAndMakeVisible(this->button_info);
	this->button_info->addListener(this);

	// Add record button
	this->button_record = new RzButton("button_record", CharPointer_UTF8("\xe2\x97\x8f") , true, processor.param_is_recording);
	this->button_record->setBounds(900, 35, 35, 35);
	addAndMakeVisible(this->button_record);
	this->button_record->addListener(this);
	
	// Add label with last recorded (or currently recording) file name
	this->last_recorded_file_name = new Label("label_recorded_file_name", this->processor.getLastRecordingFileName());
	this->last_recorded_file_name->setBounds(600, 35, 293, 35);
	this->last_recorded_file_name->setColour(Label::textColourId, Colour(48, 48, 48));
	this->last_recorded_file_name->setFont(Font(12.0f, 0));
	this->last_recorded_file_name->setJustificationType(Justification::centredRight);
	addAndMakeVisible(this->last_recorded_file_name);
	
	//Add envelope visualiser
	this->visualiser_envelope = new RzAudioVisualiserComponent(1);
	this->visualiser_envelope->setBounds(684, 95, 285, 113);
	this->visualiser_envelope->setInputType(RzAudioVisualiserComponent::INPUT_TYPE::RECTLIFIED);
	addAndMakeVisible(this->visualiser_envelope);

	// Add core visualiser
	this->visualiser_core = new RzAudioVisualiserComponent(1);
	this->visualiser_core->setBounds(683, 222, 285, 113);
	this->visualiser_core->setInputType(RzAudioVisualiserComponent::INPUT_TYPE::NORMALIZED_LOG);
	addAndMakeVisible(this->visualiser_core);

	// Add dynamic filter visualiser
	this->visualiser_dyn = new RzAudioVisualiserComponent(1);
	this->visualiser_dyn->setBounds(683, 347, 285, 113);
	this->visualiser_dyn->setInputType(RzAudioVisualiserComponent::INPUT_TYPE::NORMALIZED_LOG);
	addAndMakeVisible(this->visualiser_dyn);

	// Add output visualiser
	this->visualiser_out = new RzAudioVisualiserComponent(1);
	this->visualiser_out->setBounds(683, 474, 285, 113);
	this->visualiser_out->setInputType(RzAudioVisualiserComponent::INPUT_TYPE::FULL_WAVE);
	addAndMakeVisible(this->visualiser_out);
	
	// Add envelope follower combo box
	this->combobox_envelope = new RzComboBox(*processor.selected_env);
	this->combobox_envelope->addItem("Envelope follower RMS", (int)ENVELOPE_TYPE::RMS);
	this->combobox_envelope->addItem("Envelope follower peak", (int)ENVELOPE_TYPE::PEAK);
	this->combobox_envelope->addListener(this);
	this->combobox_envelope->setSelectedId(this->processor.selected_env->get(), NotificationType::dontSendNotification);
	this->combobox_envelope->setBounds(60, 87, 230, 20);
	addAndMakeVisible(this->combobox_envelope);

	// Add fx core combo box
	this->combobox_core = new RzComboBox(*processor.selected_fx_core);
	this->combobox_core->addItem("Fx core compressor", (int)FX_CORE_TYPE::COMPRESSOR);
	this->combobox_core->addItem("Fx core expander", (int)FX_CORE_TYPE::EXPANDER);
	this->combobox_core->addItem("Fx core limiter", (int)FX_CORE_TYPE::LIMITER);
	this->combobox_core->addItem("Fx core gate", (int)FX_CORE_TYPE::GATE);
	this->combobox_core->addListener(this);
	this->combobox_core->setSelectedId(this->processor.selected_fx_core->get(), NotificationType::dontSendNotification);
	this->combobox_core->setBounds(60, 213, 230, 20);
	addAndMakeVisible(this->combobox_core);

	// Add dynamic filter combo box
	this->combobox_dyn = new RzComboBox(*processor.selected_dyn_filter);
	this->combobox_dyn->addItem("Dynamic filter AR", (int)DYN_FILTER_TYPE::AR);
	this->combobox_dyn->addItem("Dynamic filter ARH", (int)DYN_FILTER_TYPE::ARH);
	this->combobox_dyn->addListener(this);
	this->combobox_dyn->setSelectedId(this->processor.selected_dyn_filter->get(), NotificationType::dontSendNotification);
	this->combobox_dyn->setBounds(60, 339, 230, 20);
	addAndMakeVisible(this->combobox_dyn);
	
	// Add hidden info window
	this->info_window = new RzInfoWindow;
	this->info_window->setBounds(210, 100, 620, 445);
	addChildComponent(this->info_window);

	// Set GUIs by combobox values
	this->switchGUIS();

	// Start timer callback
	startTimer(0, 1000 * (int)(this->processor.getBlockSize() / ( 3 * this->processor.getSampleRate() )));
	startTimer(1, 6); // @16.6 Hz
}


// Destructor - instance is using ScopedPointer, theres no need to care for its instances
RzAudioProcessorEditor::~RzAudioProcessorEditor()
{
}


// On timer callback - update visualisers
void RzAudioProcessorEditor::timerCallback(int timerID)
{
	switch (timerID)
	{
	case 0:
		if (!this->processor.isProcessing() && this->lastProcessingTick != this->processor.getProcessingTick())
		{
			this->lastProcessingTick = this->processor.getProcessingTick();

			this->visualiser_envelope->pushBuffer(*this->processor.current_envelope);
			this->visualiser_core->pushBuffer(*this->processor.current_core);
			this->visualiser_dyn->pushBuffer(*this->processor.current_dyn);
			this->visualiser_out->pushBuffer(*this->processor.current_out);
		}
		break;
	case 1:
		if (this->last_recorded_file_name->getText().compare(this->processor.getLastRecordingFileName()) != 0)
		{
			this->last_recorded_file_name->setText(this->processor.getLastRecordingFileName(), NotificationType::dontSendNotification);
		}
		break;
	}		
}


// Paint component
void RzAudioProcessorEditor::paint (Graphics& g)
{
	g.drawImage(cached_background,
		0, 0, 1000, 625,
		0, 0, cached_background.getWidth(), cached_background.getHeight());
	g.setColour(Colour(48,48,48));
	g.setFont(Font("Arial", 22.0f, Font::bold));
	g.drawText("Audio compressor/expander/limiter/gate",
		85, 32, 500, 50,
		Justification::centred, true);
}


// Switch GUIs
void RzAudioProcessorEditor::switchGUIS()
{
	switch ((ENVELOPE_TYPE)this->combobox_envelope->getSelectedId())
	{
	case ENVELOPE_TYPE::RMS:
		this->envelope_peak->setVisible(false);
		this->envelope_rms->setVisible(true);
		break;
	case ENVELOPE_TYPE::PEAK:
		this->envelope_peak->setVisible(true);
		this->envelope_rms->setVisible(false);
		break;
	default:
		this->envelope_peak->setVisible(false);
		this->envelope_rms->setVisible(false);
	}


	switch ((FX_CORE_TYPE)this->combobox_core->getSelectedId())
	{
	case FX_CORE_TYPE::COMPRESSOR:
		this->fx_core_comp->setVisible(true);
		this->fx_core_exp->setVisible(false);
		this->fx_core_lim->setVisible(false);
		this->fx_core_gate->setVisible(false);
		break;
	case FX_CORE_TYPE::EXPANDER:
		this->fx_core_comp->setVisible(false);
		this->fx_core_exp->setVisible(true);
		this->fx_core_lim->setVisible(false);
		this->fx_core_gate->setVisible(false);
		break;
	case FX_CORE_TYPE::LIMITER:
		this->fx_core_comp->setVisible(false);
		this->fx_core_exp->setVisible(false);
		this->fx_core_lim->setVisible(true);
		this->fx_core_gate->setVisible(false);
		break;
	case FX_CORE_TYPE::GATE:
		this->fx_core_comp->setVisible(false);
		this->fx_core_exp->setVisible(false);
		this->fx_core_lim->setVisible(false);
		this->fx_core_gate->setVisible(true);
		break;
	default:
		this->fx_core_comp->setVisible(false);
		this->fx_core_exp->setVisible(false);
	}

	switch ((DYN_FILTER_TYPE)this->combobox_dyn->getSelectedId())
	{
	case DYN_FILTER_TYPE::AR:
		this->dyn_filter_ar->setVisible(true);
		this->dyn_filter_arh->setVisible(false);
		break;
	case DYN_FILTER_TYPE::ARH:
		this->dyn_filter_ar->setVisible(false);
		this->dyn_filter_arh->setVisible(true);
		break;
	default:
		this->dyn_filter_ar->setVisible(false);
		this->dyn_filter_arh->setVisible(false);
	}
}


// Switch GUIs based on combo box values
void RzAudioProcessorEditor::comboBoxChanged(ComboBox*)
{
	this->switchGUIS();
}


// On button click event
void RzAudioProcessorEditor::buttonClicked(Button* button)
{
	if (button == this->button_info)
	{
		this->info_window->setVisible(true);
	}
	else if (button == this->button_record)
	{		
		//If button is ON
		if (this->button_record->getToggleState())
		{
			//Stop recording
			this->button_record->setToggleState(false, NotificationType::dontSendNotification);
		}
		else
		{
			//Show file picker
			FileChooser fileChooser("Choose file to save samples", File::getSpecialLocation(File::userDesktopDirectory), "*.csv");
			if (fileChooser.browseForFileToSave(true))
			{
				File* file = new File(fileChooser.getResult());

				//Start recording
				if (this->processor.setFileToRecordTo(file))
				{
					this->button_record->setToggleState(true, NotificationType::dontSendNotification);
				}
			}
		}
	}
}


#include "RzBackground.cpp"
const char* RzAudioProcessorEditor::background_image = (const char*)background;
const int RzAudioProcessorEditor::background_size = 11622;