/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzAudioProcessor.h"
#include "RzAudioProcessorEditor.h"

using namespace RZ;
using std::sqrtf;

// Create new audio processor
RzAudioProcessor::RzAudioProcessor()
{
	// Create all processor parameters
	this->addParameter(this->selected_env =				
		new RzAudioParameterInt("selected_env",			"Sel env foll",		1,		(int)ENVELOPE_TYPE::ENUM_END-1,		1, this));
	this->addParameter(this->selected_fx_core =			
		new RzAudioParameterInt("selected_fx_core",		"Sel fx core",			1,		(int)FX_CORE_TYPE::ENUM_END-1,		1, this));
	this->addParameter(this->selected_dyn_filter =		
		new RzAudioParameterInt("selected_dyn_filter",	"Sel dyn filter",		1,		(int)DYN_FILTER_TYPE::ENUM_END-1,	1, this));
	
	this->addParameter(this->envelope_rms_averaging =		
		new RzAudioParameterFloat("env_rms_averaging",	"Env RMS avg",	0.01f,	100.0f, 10.0f, this)); //ms
	this->addParameter(this->envelope_peak_attack =			
		new RzAudioParameterFloat("env_peak_attact",		"Env peak attack",		0.01f,	500.0f, 1.0f, this)); //ms
	this->addParameter(this->envelope_peak_release =		
		new RzAudioParameterFloat("env_peak_release",		"Env peak release",		0.01f,	500.0f, 1.0f, this)); //ms
	this->addParameter(this->envelope_peak_correction_gain =
		new RzAudioParameterFloat("env_peak_corr_gain", "Env peak gain",		-12.0f, 12.0f,	0.0f, this)); //db

	this->addParameter(this->fx_core_comp_threshold =		
		new RzAudioParameterFloat("fx_core_comp_thresh",	"Comp thresh",			-50.0f, 0.0f,	-20.0f, this)); //db
	this->addParameter(this->fx_core_comp_ratio =			
		new RzAudioParameterFloat("fx_core_comp_ratio",	"Comp ratio",			1.0f,	20.0f,	4.0f, this)); //:1
	this->addParameter(this->fx_core_expander_threshold =	
		new RzAudioParameterFloat("fx_core_exp_thresh",	"Exp thresh",			-50.0f, 0.0f,	-40.0f, this)); //db
	this->addParameter(this->fx_core_expander_ratio =
		new RzAudioParameterFloat("fx_core_exp_ratio",	"Exp ratio",			0.05f,	1.0f,	0.25f, this)); //:1
	this->addParameter(this->fx_core_expander_range = 
		new RzAudioParameterFloat("fx_core_range_range",	"Exp range",			-72.0f, 0.0f,	-40.0f, this)); //db
	this->addParameter(this->fx_core_limiter_threshold =
		new RzAudioParameterFloat("fx_core_lim_thresh",	"Lim thresh",			-50.0f, 0.0f,	0.0f, this)); //db
	this->addParameter(this->fx_core_gate_threshold =
		new RzAudioParameterFloat("fx_core_gate_thresh",	"Gate thresh",			-50.0f, 0.0f, -20.0f, this)); //db

	this->addParameter(this->dyn_ar_attack = 
		new RzAudioParameterFloat("dyn_ar_attack",		"Dyn AR attack",		0.1f,	500.0f, 10.0f, this));  //ms
	this->addParameter(this->dyn_ar_release = 
		new RzAudioParameterFloat("dyn_ar_release",		"Dyn AR release",		0.1f,	500.0f, 10.0f, this));//ms
	this->addParameter(this->dyn_arh_attack = 
		new RzAudioParameterFloat("dyn_arh_attack",		"Dyn ARH attack",		0.1f,	500.0f, 10.0f, this));//ms
	this->addParameter(this->dyn_arh_release = 
		new RzAudioParameterFloat("dyn_arh_release",		"Dyn ARH release",		0.1f,	500.0f, 10.0f, this));//ms
	this->addParameter(this->dyn_arh_hold = 
		new RzAudioParameterFloat("dyn_arh_hold",			"Dyn ARH hold",			0.1f,	500.0f, 10.0f, this));//ms

	this->addParameter(this->markup_gain =
		new RzAudioParameterFloat("markup_gain", "Markup gain", -20.0f, 20.0f, 0.0f, this));//db

	this->addParameter(this->param_is_recording =
		new RzAudioParameterBool("is_recording", "Recording", false, this));


	// Initialize current input data for use in visualisers
	const int bs = this->getBlockSize();
	this->current_input = new AudioSampleBuffer(1, bs);
	this->current_envelope = new AudioSampleBuffer(1, bs);
	this->current_core = new AudioSampleBuffer(1, bs);
	this->current_dyn = new AudioSampleBuffer(1, bs);
	this->current_out = new AudioSampleBuffer(1, bs);
	
	// Listen to your own changes
	this->addListener(this);
}


// Destructor
RzAudioProcessor::~RzAudioProcessor()
{
	delete this->recording_file;
}


// Initialize random seed
Random RzAudioProcessor::random_seed = Random();


// Update all processor counting parts parameters - get real value from GUI/processor parameters, pass them to counting cores
void RzAudioProcessor::updateParameters()
{
	double sample_rate = this->getSampleRate();

	this->envelope_peak.setTimeConstants(sample_rate, this->envelope_peak_attack->get()/1000, this->envelope_peak_release->get()/1000, this->envelope_peak_correction_gain->get());
	this->envelope_rms.setTimeConstants(sample_rate, this->envelope_rms_averaging->get()/1000);

	this->fx_core_compressor.setParams(sample_rate, this->fx_core_comp_threshold->get(), this->fx_core_comp_ratio->get());
	this->fx_core_expander.setParams(sample_rate, this->fx_core_expander_threshold->get(), this->fx_core_expander_ratio->get(), this->fx_core_expander_range->get());
	this->fx_core_limiter.setParams(sample_rate, this->fx_core_limiter_threshold->get(), 1000.0f);
	this->fx_core_gate.setParams(sample_rate, this->fx_core_gate_threshold->get(), 0.0001f);

	this->dynamic_filter_ar.setTimeConstants(sample_rate, this->dyn_ar_attack->get()/1000, this->dyn_ar_release->get()/1000);
	this->dynamic_filter_arh.setTimeConstants(sample_rate, this->dyn_arh_attack->get()/1000, this->dyn_arh_release->get()/1000, this->dyn_arh_hold->get()/1000);

	
	if (this->param_is_recording->get() == true)
	{
		if (this->recording_status == RECORDING_STATUS::NOT_RECORDING)
		{
			this->recording_status = RECORDING_STATUS::BEFORE_START;
		}
	}
	else
	{
		if (this->recording_status != RECORDING_STATUS::NOT_RECORDING)
		{
			this->recording_status = RECORDING_STATUS::BEFORE_END;
		}
	}

	// Set if filters filter signal from compressor or limiter
	switch ((FX_CORE_TYPE)this->selected_fx_core->get())
	{
	case FX_CORE_TYPE::COMPRESSOR:
	case FX_CORE_TYPE::LIMITER:
		this->dynamic_filter_ar.setIsCompressorFilter(true);
		this->dynamic_filter_arh.setIsCompressorFilter(true);
		break;
	default:
		this->dynamic_filter_ar.setIsCompressorFilter(false);
		this->dynamic_filter_arh.setIsCompressorFilter(false);
	}

	this->parameters_changed = false;
}


// Update core parameters on GUI/processor parameter change
void RzAudioProcessor::audioProcessorParameterChanged(AudioProcessor*, int, float)
{
	//Recount all
	this->notifyParameterChanged();
}


// Update core parameters on processor change
void RzAudioProcessor::audioProcessorChanged(AudioProcessor* )
{
	//Notify sample rate
	this->notifyParameterChanged();
}


// Get plugin name
const String RzAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


// Returns true if processor accepts MIDI
bool RzAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}


// Returns true if processor produces MIDI
bool RzAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}


// Get tail length (set to 4 seconds, so that visualisers in non-DAW hosts will not repeat last signal block)
double RzAudioProcessor::getTailLengthSeconds() const
{
    return 4.0;
}


// Update parameters before playing
void RzAudioProcessor::prepareToPlay (double, int)
{
	this->updateParameters();
}


// Release resources
void RzAudioProcessor::releaseResources()
{

}


// Set preffered bus arrangement
#ifndef JucePlugin_PreferredChannelConfigurations
bool RzAudioProcessor::setPreferredBusArrangement(bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif


// Process block of data
void RzAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	this->rzProcessBlock(buffer, midiMessages);
}


// Proccess block when bypassed
void RzAudioProcessor::processBlockBypassed(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	this->rzProcessBlock(buffer, midiMessages, true);
}


// Process block of data
void RzAudioProcessor::rzProcessBlock(AudioSampleBuffer& buffer, MidiBuffer&, bool bypassed)
{
	// Update parameters if changed
	if (this->parameters_changed)
	{
		this->updateParameters();
	}
	
	// Processing starts
	this->is_processing = true;

	// Get data length
	int length = buffer.getNumSamples();
	
	// Get number of inputs
	int inputs_num = getTotalNumInputChannels();

	// Get number of outputs (should be 0)
	int outputs_num = getTotalNumOutputChannels();

	// Clear output buffers
	for (int i = outputs_num; i < outputs_num; ++i)
	{
		buffer.clear(i, 0, buffer.getNumSamples());
	}


	// Get input data write pointer
	float** inputs = new float*[inputs_num];
	for (int channel = 0; channel < inputs_num; ++channel)
	{
		inputs[channel] = buffer.getWritePointer(channel);
	}

	// Resize audio visualisers pointer buffers to match input sample number
	this->current_input->setSize(1, length, true, false, true);
	this->current_envelope->setSize(1, length, true, false, true);
	this->current_core->setSize(1, length, true, false, true);
	this->current_dyn->setSize(1, length, true, false, true);
	this->current_out->setSize(1, length, true, false, true);

	// Get write pointers for current samples (used for audio visualisers)
	float* current_input_data = this->current_input->getWritePointer(0);
	float* current_envelope_data = this->current_envelope->getWritePointer(0);
	float* current_core_data = this->current_core->getWritePointer(0);
	float* current_dyn_data = this->current_dyn->getWritePointer(0);
	float* current_output_data = this->current_out->getWritePointer(0);

	// Create buffer for sidechain smaples, and convert signals from input channel to mono
	float* sidechain_samples = new float[length];
	RZ::toMono(inputs, sidechain_samples, length, inputs_num);
	SignalBlock<float> sidechain = SignalBlock<float>(sidechain_samples, length);

	// Save input data for recording purposes
	for (int i = 0; i < length; ++i)
	{
		current_input_data[i] = sidechain[i];
	}


	// - - - - - - - - - - - - - - SIDECHAIN - - - - - - - - - - - - - - - -
	//Get input signal envelope
	switch ((ENVELOPE_TYPE)this->selected_env->get())
	{
	case ENVELOPE_TYPE::RMS:
		this->envelope_rms.countOutput(&sidechain);
		break;
	case ENVELOPE_TYPE::PEAK:
		this->envelope_peak.countOutput(&sidechain);
		break;
	}


	// Save after-envelope follower data for use of visualiser 
	switch ((ENVELOPE_TYPE)this->selected_env->get())
	{
	case ENVELOPE_TYPE::RMS:
		for (int i = 0; i < length; ++i)
		{
			current_envelope_data[i] = sqrtf(sidechain[i]);
		}
		break;
	case ENVELOPE_TYPE::PEAK:
		for (int i = 0; i < length; ++i)
		{
			current_envelope_data[i] = sidechain[i];
		}
		break;
	}
	


	// Lin/log
	switch ((ENVELOPE_TYPE)this->selected_env->get())
	{
	case ENVELOPE_TYPE::RMS:
		//Lin^2 / log
		this->convertor_lin_square_to_log.countOutput(&sidechain);
		break;
	case ENVELOPE_TYPE::PEAK:
		//Lin / log
		this->convertor_lin_to_log.countOutput(&sidechain);
		break;
	}


	//Get static gain to use for reduction
	switch ((FX_CORE_TYPE)this->selected_fx_core->get())
	{
	case FX_CORE_TYPE::COMPRESSOR:
		this->fx_core_compressor.countOutput(&sidechain);
		break;
	case FX_CORE_TYPE::EXPANDER:
		this->fx_core_expander.countOutput(&sidechain);
		break;
	case FX_CORE_TYPE::LIMITER:
		this->fx_core_limiter.countOutput(&sidechain);
		break;
	case FX_CORE_TYPE::GATE:
		this->fx_core_gate.countOutput(&sidechain);
		break;
	}


	//Log / lin
	this->convertor_log_to_lin.countOutput(&sidechain);


	// Save after-core data for use of visualiser
	for (int i = 0; i < length; ++i)
	{
		current_core_data[i] = sidechain[i];
	}

	// Perform dynamic filtering
	switch ((DYN_FILTER_TYPE)this->selected_dyn_filter->get())
	{
	case DYN_FILTER_TYPE::AR:
		// Get dynamic filter output
		this->dynamic_filter_ar.countOutput(&sidechain);
		break;
	case DYN_FILTER_TYPE::ARH:
		// Get dynamic filter output
		this->dynamic_filter_arh.countOutput(&sidechain);
		break;
	}

	// Save after-dynamic filter data for use of visualiser
	for (int i = 0; i < length; ++i)
	{
		current_dyn_data[i] = sidechain[i];
	}

	if (!bypassed)
	{
		// - - - - - - - - - - - - - - - VCA - - - - - - - - - - - - - - - - - -
		RZ::vca(inputs, inputs, sidechain.getData(), length, inputs_num, dbToLin(this->markup_gain->get()));

		// Save after-VCA data only if there are input channels
		if (inputs_num > 0)
		{
			for (int i = 0; i < length; ++i)
			{
				current_output_data[i] = 0.0f;

				for (int y = 0; y < inputs_num; ++y)
				{
					current_output_data[i] += inputs[y][i];
				}
				
				current_output_data[i] /= inputs_num;
			}
		}
		else
		{
			for (int i = 0; i < length; ++i)
			{
				current_output_data[i] = 0;
			}
		}
	}
	else
	{
		float** outputs = new float*[inputs_num];
		for (int i = 0; i < inputs_num; ++i)
		{
			outputs[i] = new float[length];
		}

		// - - - - - - - - - - - - - - - VCA - - - - - - - - - - - - - - - - - -
		RZ::vca(inputs, outputs, sidechain.getData(), length, inputs_num, dbToLin(this->markup_gain->get()));

		// Save after-VCA data only if there are input channels (merge channels)
		if (inputs_num > 0)
		{
			for (int i = 0; i < length; ++i)
			{
				current_output_data[i] = 0.0f;

				for (int y = 0; y < inputs_num; ++y)
				{
					current_output_data[i] += outputs[y][i];
				}
				
				current_output_data[i] /= inputs_num;
			}
		}
		else
		{
			for (int i = 0; i < length; ++i)
			{
				current_output_data[i] = 0;
			}
		}

		for (int i = 0; i < inputs_num; ++i)
		{
			delete[] outputs[i];
		}
		delete[] outputs;
	}


	// - - - - - - - - - - - - - - - RECORDING - - - - - - - - - - - - - - - - - -
	if (this->recording_status == RECORDING_STATUS::BEFORE_START)
	{
		this->startRecordingTo(this->recording_file);
	}

	// Record samples if recording enabled
	if (this->recording_status == RECORDING_STATUS::RECORDING)
	{
		float cols[5];
		for (int i = 0; i < length; ++i)
		{
			cols[0] = current_input_data[i];
			cols[1] = current_envelope_data[i];
			cols[2] = current_core_data[i];
			cols[3] = current_dyn_data[i];
			cols[4] = current_output_data[i];
		}
		this->recordNewSamples(cols, 5);
	}
	else if (this->recording_status == RECORDING_STATUS::BEFORE_END)
	{
		this->stopRecording();
	}

	// Processing is over
	this->processing_tick++;
	this->is_processing = false;

	// Cleanup
	delete[] sidechain_samples;
	delete[] inputs;
}


// Returns true if this processor has editor - yes, it has obviously
bool RzAudioProcessor::hasEditor() const
{
    return true;
}


// Create new editor instance
AudioProcessorEditor* RzAudioProcessor::createEditor()
{
    return new RzAudioProcessorEditor(*this);
}


// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RzAudioProcessor();
}



// Get recording status
RECORDING_STATUS RzAudioProcessor::getRecordingStatus()
{
	return this->recording_status;
}


// Set file to record to
bool RzAudioProcessor::setFileToRecordTo(File* file)
{
	if (this->recording_status == RECORDING_STATUS::NOT_RECORDING || this->recording_status == RECORDING_STATUS::BEFORE_START)
	{
		this->recording_file = file;
		return true;
	}
	else
	{
		return false;
	}
}


// Start recording to CSV file - prepare a file
bool RzAudioProcessor::startRecordingTo(File* file)
{
	if (this->recording_status == RECORDING_STATUS::BEFORE_START)
	{
		this->recording_status = RECORDING_STATUS::STARTING;

		// Create new file on desktop if not exists
		if (nullptr == file)
		{
			String full_name;
			
			Time t = Time::getCurrentTime();
			String str_date = t.toString(false, true, true, true).replaceCharacter(':', '-');

			File desktop = File::getSpecialLocation(File::userDesktopDirectory);

			for (int i = 1; i < 10; ++i)
			{
				String random_salt = String(RzAudioProcessor::random_seed.nextInt(9999));
				
				full_name.clear();
				full_name.append(this->RECORDING_FILE_BASENAME, this->RECORDING_FILE_BASENAME.length());
				full_name.append(str_date, str_date.length());
				full_name.append("_", 1);
				full_name.append(random_salt, random_salt.length());				
				full_name.append(this->RECORDING_FILE_EXTENSION, this->RECORDING_FILE_EXTENSION.length());

				File temp = desktop.getChildFile(full_name);
				if (!temp.exists() && temp.hasWriteAccess())
				{
					this->recording_file = new File(temp);
					this->recording_status = RECORDING_STATUS::RECORDING;
					break;
				}
			}
		}
		else
		{
			if (file->hasWriteAccess())
			{
				this->recording_file = file;
				this->recording_status = RECORDING_STATUS::RECORDING;
			}
		}
	}
	else if (this->recording_status == RECORDING_STATUS::NOT_RECORDING && nullptr != file)
	{
		this->stopRecording();

		this->recording_status = RECORDING_STATUS::STARTING;
		
		if (file->hasWriteAccess())
		{
			this->recording_file = file;
			this->recording_status = RECORDING_STATUS::RECORDING;
		}
	}


	if (this->recording_status == RECORDING_STATUS::RECORDING && nullptr != this->recording_file)
	{	
		this->recording_file->create();
		this->recording_file->replaceWithText("");
		this->last_recording_file_name = this->recording_file->getFileName();

		this->recording_stream = new FileOutputStream(*this->recording_file);
		this->recording_stream->writeText("n;input;envelope;fx_core;dyn_filter;output\n", false, false);

		this->recording_sample_number = 0;
		return true;
	}
	else
	{
		return false;
	}
}


// Save line of values into recording file
bool RzAudioProcessor::recordNewSamples(float* cols, int cols_number)
{
	if (this->recording_status != RECORDING_STATUS::RECORDING )
	{
		return false;
	}

	const int num_decimals = 5;
	const int col_length = num_decimals + 20;

	String line;

	line.append(String(this->recording_sample_number++), sizeof(int));
	line.append(";", 1);

	for (int i = 0; i < cols_number; ++i)
	{	
		line.append(String(cols[i], num_decimals), col_length);
		line.append(";", 1);
	}
	
	line.append("\n", 2);

	this->recording_stream->writeText(line, false, false);
	
	return true;
}


// Stop recording
void RzAudioProcessor::stopRecording()
{
	if (nullptr != this->recording_stream)
	{
		this->recording_stream->flush();
		delete this->recording_stream;
		this->recording_stream = nullptr;
	}

	delete this->recording_file;
	this->recording_file = nullptr;
	this->recording_status = RECORDING_STATUS::NOT_RECORDING;
}


// Get state information
void RzAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	MemoryOutputStream stream(destData, true);

	stream.writeInt(this->current_program);

	stream.writeInt(this->selected_env->get());
	stream.writeInt(this->selected_fx_core->get());
	stream.writeInt(this->selected_dyn_filter->get());

	stream.writeFloat(this->envelope_rms_averaging->get());
	stream.writeFloat(this->envelope_peak_attack->get());
	stream.writeFloat(this->envelope_peak_release->get());
	stream.writeFloat(this->envelope_peak_correction_gain->get());

	stream.writeFloat(this->fx_core_comp_threshold->get());
	stream.writeFloat(this->fx_core_comp_ratio->get());
	stream.writeFloat(this->fx_core_expander_threshold->get());
	stream.writeFloat(this->fx_core_expander_ratio->get());
	stream.writeFloat(this->fx_core_expander_range->get());
	stream.writeFloat(this->fx_core_limiter_threshold->get());
	stream.writeFloat(this->fx_core_gate_threshold->get());

	stream.writeFloat(this->dyn_ar_attack->get());
	stream.writeFloat(this->dyn_ar_release->get());
	stream.writeFloat(this->dyn_arh_attack->get());
	stream.writeFloat(this->dyn_arh_release->get());
	stream.writeFloat(this->dyn_arh_hold->get());

	stream.writeFloat(this->markup_gain->get());
}


// Set state information
void RzAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	MemoryInputStream stream(data, static_cast<size_t> (sizeInBytes), false);

	this->current_program = stream.readInt();

	*this->selected_env = stream.readInt();
	*this->selected_fx_core = stream.readInt();
	*this->selected_dyn_filter = stream.readInt();

	*this->envelope_rms_averaging = stream.readFloat();
	*this->envelope_peak_attack = stream.readFloat();
	*this->envelope_peak_release = stream.readFloat();
	*this->envelope_peak_correction_gain = stream.readFloat();

	*this->fx_core_comp_threshold = stream.readFloat();
	*this->fx_core_comp_ratio = stream.readFloat();
	*this->fx_core_expander_threshold = stream.readFloat();
	*this->fx_core_expander_ratio = stream.readFloat();
	*this->fx_core_expander_range = stream.readFloat();
	*this->fx_core_limiter_threshold = stream.readFloat();
	*this->fx_core_gate_threshold = stream.readFloat();

	*this->dyn_ar_attack = stream.readFloat();
	*this->dyn_ar_release = stream.readFloat();
	*this->dyn_arh_attack = stream.readFloat();
	*this->dyn_arh_release = stream.readFloat();
	*this->dyn_arh_hold = stream.readFloat();

	*this->markup_gain = stream.readFloat();
}


// Get number of programs
int RzAudioProcessor::getNumPrograms()
{
	return 14;
}


// Get current program
int RzAudioProcessor::getCurrentProgram()
{
	return this->current_program;
}


// Get program name
const String RzAudioProcessor::getProgramName(int index)
{
	switch (index)
	{
	case 0:
		return String("default");
	case 1:
		return String("Compressor short");
	case 2:
		return String("Compressor long");
	case 3:
		return String("Expander");
	case 4:
		return String("Limiter");
	case 5:
		return String("Gate");
	case 6:
		return String("E1-S1 - hard limiter");
	case 7:
		return String("E2-S1 - hard expander");
	case 8:
		return String("E3-S2 - compressor");
	case 9:
		return String("E4-S2 - expander");
	case 10:
		return String("E5-S3 - limiter");
	case 11:
		return String("E6-S3 - gate");
	case 12:
		return String("E7-S4 - comp & hold");
	case 13:
		return String("E8-S4 - gate & hold");
	default:
		return String();
	}
}


// Set current program
void RzAudioProcessor::setCurrentProgram(int index)
{
	if (this->current_program != index)
	{
		this->current_program = index;

		switch (index)
		{
		case 0:
		{
			//LET IT BE
			break;
		}
		// Compressor short
		case 1:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::COMPRESSOR;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 1.25f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -16.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = 0.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 15.0f;
			*this->dyn_ar_release = 35.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 6.0f;
			break;
		}
		// Compressor long
		case 2:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::COMPRESSOR;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 1.25f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -16.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = 0.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 80.0f;
			*this->dyn_ar_release = 240.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 3.0f;
			break;
		}
		//Expander
		case 3:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::EXPANDER;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -20.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -20.0f;
			*this->fx_core_expander_ratio = 0.80f;
			*this->fx_core_expander_range = -12.0f;
			*this->fx_core_limiter_threshold = 0.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 120.0f;
			*this->dyn_ar_release = 100.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Limiter
		case 4:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::PEAK;
			*this->selected_fx_core = (int)FX_CORE_TYPE::LIMITER;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 3.0f;
			*this->envelope_peak_release = 7.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -2.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = -12.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 5.0f;
			*this->dyn_ar_release = 0.1f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Gate
		case 5:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::GATE;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::ARH;

			*this->envelope_rms_averaging = 20.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -20.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = 0.0f;
			*this->fx_core_gate_threshold = -30.0f;

			*this->dyn_ar_attack = 17.0f;
			*this->dyn_ar_release = 3.0f;
			*this->dyn_arh_attack = 30.0f;
			*this->dyn_arh_release = 0.1f;
			*this->dyn_arh_hold = 50.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Ex 01 - sample 01 - hard limiter
		case 6:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::PEAK;
			*this->selected_fx_core = (int)FX_CORE_TYPE::LIMITER;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 6.0f;

			*this->fx_core_comp_threshold = -2.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = -6.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 1.0f;
			*this->dyn_ar_release = 1.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Ex 02 - sample 01 - hard expander
		case 7:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::EXPANDER;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -20.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -1.0f;
			*this->fx_core_expander_ratio = 0.05f;
			*this->fx_core_expander_range = -18.0f;
			*this->fx_core_limiter_threshold = 0.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 8.0f;
			*this->dyn_ar_release = 14.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Ex 03 - sample 02 - compressor
		case 8:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::COMPRESSOR;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -6.0f;
			*this->fx_core_comp_ratio = 2.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = 0.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 150.0f;
			*this->dyn_ar_release = 150.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Ex 04 - sample 02 - expander & hold
		case 9:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::EXPANDER;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -20.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -3.0f;
			*this->fx_core_expander_ratio = 0.5f;
			*this->fx_core_expander_range = -48.0f;
			*this->fx_core_limiter_threshold = 0.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 10.0f;
			*this->dyn_ar_release = 10.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Ex 05 - sample 03 - limiter
		case 10:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::PEAK;
			*this->selected_fx_core = (int)FX_CORE_TYPE::LIMITER;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::AR;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 0.01f;
			*this->envelope_peak_release = 10.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -2.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = -6.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 2.0f;
			*this->dyn_ar_release = 2.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 10.0f;
			*this->dyn_arh_hold = 10.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Ex 06 - sample 03 - gate
		case 11:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::GATE;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::ARH;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -2.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = -6.0f;
			*this->fx_core_gate_threshold = -6.0f;

			*this->dyn_ar_attack = 10.0f;
			*this->dyn_ar_release = 10.0f;
			*this->dyn_arh_attack = 50.0f;
			*this->dyn_arh_release = 250.0f;
			*this->dyn_arh_hold = 200.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Ex 07 - sample 04 - compressor & hold
		case 12:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::COMPRESSOR;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::ARH;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -12.0f;
			*this->fx_core_comp_ratio = 2.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = 0.0f;
			*this->fx_core_gate_threshold = -20.0f;

			*this->dyn_ar_attack = 10.0f;
			*this->dyn_ar_release = 10.0f;
			*this->dyn_arh_attack = 10.0f;
			*this->dyn_arh_release = 120.0f;
			*this->dyn_arh_hold = 400.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		// Ex 08 - sample 04 - gate & hold
		case 13:
		{
			*this->selected_env = (int)ENVELOPE_TYPE::RMS;
			*this->selected_fx_core = (int)FX_CORE_TYPE::GATE;
			*this->selected_dyn_filter = (int)DYN_FILTER_TYPE::ARH;

			*this->envelope_rms_averaging = 10.0f;
			*this->envelope_peak_attack = 1.0f;
			*this->envelope_peak_release = 1.0f;
			*this->envelope_peak_correction_gain = 0.0f;

			*this->fx_core_comp_threshold = -2.0f;
			*this->fx_core_comp_ratio = 4.0f;
			*this->fx_core_expander_threshold = -40.0f;
			*this->fx_core_expander_ratio = 0.25f;
			*this->fx_core_expander_range = -40.0f;
			*this->fx_core_limiter_threshold = -6.0f;
			*this->fx_core_gate_threshold = -6.0f;

			*this->dyn_ar_attack = 2.0f;
			*this->dyn_ar_release = 2.0f;
			*this->dyn_arh_attack = 60.0f;
			*this->dyn_arh_release = 150.0f;
			*this->dyn_arh_hold = 280.0f;

			*this->markup_gain = 0.0f;
			break;
		}
		}
	}
}


// Change program name
void RzAudioProcessor::changeProgramName(int, const String&)
{
}