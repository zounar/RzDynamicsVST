/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#ifndef __JUCE_HEADER_A0EBA7238B0B80E8__
#define __JUCE_HEADER_A0EBA7238B0B80E8__


#include "../JuceLibraryCode/JuceHeader.h"
#include "RzButton.h"


class RzInfoWindow  : public Component, private Button::Listener
{
public:
    // Create new info window
    RzInfoWindow ();

	// Destructor
    ~RzInfoWindow();

	// On button click event
	void buttonClicked(Button* button) override;

	// Paint component
    void paint (Graphics& g) override;
private:
	// Rectangle used to generate shadow
	ScopedPointer<Rectangle<int>> shadow_rectangle;

	// Close button
	ScopedPointer<RzButton> close_button;

	// Window shadow
	ScopedPointer<DropShadow> shadow_outer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RzInfoWindow)
};

#endif