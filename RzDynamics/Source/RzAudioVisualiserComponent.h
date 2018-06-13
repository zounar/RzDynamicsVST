/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_211AABS7123dCB828C__
#define __JUCE_HEADER_211AABS7123dCB828C__

#include "../JuceLibraryCode/JuceHeader.h"

// Audio visualiser overloaded
class RzAudioVisualiserComponent : public AudioVisualiserComponent
{
public:
	// Visualiser input types
	enum INPUT_TYPE
	{
		NORMALIZED_LOG = 0,
		RECTLIFIED,
		FULL_WAVE
	};

	// Create new component
	RzAudioVisualiserComponent(int initialNumChannels);

	// Destructor
	~RzAudioVisualiserComponent() {};

	// Set normalized mode
	void setInputType(INPUT_TYPE input_type) { this->_input_type = input_type; }

	// Set show db grid mode
	void setShowDbGrid(bool showDbGrid) { this->_showDbGrid = showDbGrid;  }

	// Get sample channel as vector path
	void getChannelAsPath(Path& path, const Range<float>* levels, int numLevels, int nextSample);

	// Paint one channel into area
	void paintChannel(Graphics& g, Rectangle<float> area,
		const Range<float>* levels, int numLevels, int nextSample) override;
private:
	// Current input type
	INPUT_TYPE _input_type = INPUT_TYPE::FULL_WAVE;

	// True if grid should be shown
	bool _showDbGrid = true;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RzAudioVisualiserComponent)
};
#endif