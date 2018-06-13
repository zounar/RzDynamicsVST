/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "RzComboBox.h"
#include "RzKnob.h"
#include "RzBuffer.h"
#include "RzConvertor.h"
#include "RzDynamicFilter.h"
#include "RzEnvelopeFollower.h"
#include "RzFunctions.h"
#include "RzFxCore.h"
#include "RzProcessorElement.h"
#include "RzSignalBlock.h"
#include "RzSignalBlockWithBuffer.h"
#include "RzAudioParameterFloat.h"
#include "RzAudioParameterInt.h"
#include "RzAudioParameterBool.h"

using namespace RZ;

namespace RZ
{
	// Enumerated envelope types
	enum class ENVELOPE_TYPE
	{
		RMS = 1,
		PEAK,
		ENUM_END
	};


	// Enumerated fx core types
	enum class FX_CORE_TYPE
	{
		COMPRESSOR = 1,
		EXPANDER,
		LIMITER,
		GATE,
		ENUM_END
	};


	// Enumerated dynamic filter types
	enum class DYN_FILTER_TYPE
	{
		AR = 1,
		ARH,
		ENUM_END
	};


	// Recording status types
	enum class RECORDING_STATUS
	{
		BEFORE_START = 0,
		STARTING,
		RECORDING,
		BEFORE_END,
		NOT_RECORDING
	};
}


class RzAudioProcessor  : public AudioProcessor, private AudioProcessorListener
{
public:	
	// Create new audio processor
    RzAudioProcessor();

	// Destructor
    ~RzAudioProcessor();
	
	// Update all processor counting parts parameters - get real value from GUI/processor parameters, pass them to counting cores
	void updateParameters();

	// Update core parameters on GUI/processor parameter change
	void audioProcessorParameterChanged(AudioProcessor *processor, int parameterIndex, float newValue);

	// Update core parameters on processor change
	void audioProcessorChanged(AudioProcessor *processor);

	// Get plugin name
	const String getName() const override;

	// Returns true if processor accepts MIDI
	bool acceptsMidi() const override;

	// Returns true if processor produces MIDI
	bool producesMidi() const override;

	// Get tail length
	double getTailLengthSeconds() const override;

	// Get number of programs
	int getNumPrograms() override;

	// Get current program
	int getCurrentProgram() override;

	// Set current program
	void setCurrentProgram(int index) override;

	// Get program name
	const String getProgramName(int index) override;

	// Change program name
	void changeProgramName(int index, const String& newName) override;

	// Update parameters before playing
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;

	// Release resources
    void releaseResources() override;

	// Set preffered bus arrangement
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet) override;
   #endif

	// Process block of data
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

	// Proccess block when bypassed
	void processBlockBypassed(AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

	// Process block of data
	void rzProcessBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages, bool bypassed = false);

	// Returns true if this processor has editor
	bool hasEditor() const override;

	// Create new editor instance
    AudioProcessorEditor* createEditor() override;
	
	// Get state information
	void getStateInformation(MemoryBlock& destData) override;

	// Set state information
	void setStateInformation(const void* data, int sizeInBytes) override;
	
	// Returns true if plugin is VST2
	bool isVST2() const noexcept { return (this->wrapperType == wrapperType_VST); }
	
	// Get recording status
	RECORDING_STATUS getRecordingStatus();
	
	// Set file to record to
	bool setFileToRecordTo(File* file);

	// Start recording to CSV file
	bool startRecordingTo(File* file = nullptr);
	
	// Record new samples
	bool recordNewSamples(float* cols, int cols_number);

	// Stop recording
	void stopRecording();
	
	ScopedPointer<AudioSampleBuffer> current_input = nullptr;
	ScopedPointer<AudioSampleBuffer> current_envelope = nullptr;
	ScopedPointer<AudioSampleBuffer> current_core = nullptr;
	ScopedPointer<AudioSampleBuffer> current_dyn = nullptr;
	ScopedPointer<AudioSampleBuffer> current_out = nullptr;

	RzAudioParameterInt* selected_env;
	RzAudioParameterInt* selected_fx_core;
	RzAudioParameterInt* selected_dyn_filter;

	RzAudioParameterFloat* envelope_rms_averaging;
	RzAudioParameterFloat* envelope_peak_attack;
	RzAudioParameterFloat* envelope_peak_release;
	RzAudioParameterFloat* envelope_peak_correction_gain;

	RzAudioParameterFloat* fx_core_comp_threshold;
	RzAudioParameterFloat* fx_core_comp_ratio;
	RzAudioParameterFloat* fx_core_expander_threshold;
	RzAudioParameterFloat* fx_core_expander_ratio;
	RzAudioParameterFloat* fx_core_expander_range;
	RzAudioParameterFloat* fx_core_limiter_threshold;
	RzAudioParameterFloat* fx_core_gate_threshold;

	RzAudioParameterFloat* dyn_ar_attack;
	RzAudioParameterFloat* dyn_ar_release;
	RzAudioParameterFloat* dyn_arh_attack;
	RzAudioParameterFloat* dyn_arh_release;
	RzAudioParameterFloat* dyn_arh_hold;

	RzAudioParameterFloat* markup_gain;

	RzAudioParameterBool* param_is_recording;

	// Random number seed
	static Random random_seed;

	// If called, all fx values will be updated when next process replacing is called
	void notifyParameterChanged() { this->parameters_changed = true; };

	// Get last recorded (or currently recording) file name
	String getLastRecordingFileName() const { return this->last_recording_file_name; };

	// True is processor is processing
	bool isProcessing() const { return this->is_processing; };

	// Get processing tick number (number in range of 0 - 255, can be used for synchronization purpose )
	unsigned char getProcessingTick() const { return this->processing_tick; };
private:
	// Current program (preset) index
	int current_program = 0;

	// If true, all fx values will be updated when next process replacing is called
	bool parameters_changed = true;

	// If true, processor is processing
	bool is_processing = false;

	// Processing tick number (number in range of 0 - 255, can be used for synchronization purpose )
	unsigned char processing_tick = 0;

	// Recording status
	RECORDING_STATUS recording_status = RECORDING_STATUS::NOT_RECORDING;

	// File to record to
	File* recording_file = nullptr;

	// Recording sample number
	unsigned int recording_sample_number = 0;

	// File output stream used for recording
	FileOutputStream* recording_stream = nullptr;
	
	// Recording file default name
	const String RECORDING_FILE_BASENAME = String("vst_output_");
	const String RECORDING_FILE_EXTENSION = String(".csv");

	// Name of last recorded (or currently recording) file
	String last_recording_file_name = "";

	EnvelopeFollowerPeak<float> envelope_peak;
	EnvelopeFollowerRms<float> envelope_rms;
	ConvertorLinSquareToLog<float> convertor_lin_square_to_log;
	ConvertorLinToLog<float> convertor_lin_to_log;
	ConvertorLogToLin<float> convertor_log_to_lin;
	FxCoreCompressor<float> fx_core_compressor;
	FxCoreExpander<float> fx_core_expander;
	FxCoreCompressor<float> fx_core_limiter;
	FxCoreExpander<float> fx_core_gate;
	DynamicFilterAR<float> dynamic_filter_ar;
	DynamicFilterARH<float> dynamic_filter_arh;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzAudioProcessor)
};

#endif