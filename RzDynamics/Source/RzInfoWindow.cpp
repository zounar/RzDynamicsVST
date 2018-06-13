/*
*	@author Robin Zon <xzonro00@vutbr.cz>
*	@uses JUCE
*/

#include "RzInfoWindow.h"


// Create new info window
RzInfoWindow::RzInfoWindow ()
{
	this->shadow_rectangle = new Rectangle<int>(585, 405);
	this->close_button = new RzButton("button_close", "X");
	this->shadow_outer = new DropShadow(Colour(10, 10, 10), 15, Point<int>(15, 15));

	this->close_button->setBounds(538, 30, 35, 35);
	this->close_button->addListener(this);
	addAndMakeVisible(this->close_button);

    setSize (600, 425);
}


// Destructor
RzInfoWindow::~RzInfoWindow()
{
}


// On button click event
void RzInfoWindow::buttonClicked(Button* /*button*/)
{
	this->setVisible(false);
}


// Paint component
void RzInfoWindow::paint (Graphics& g)
{
	this->shadow_outer->drawForRectangle(g, *this->shadow_rectangle);

    g.setColour (Colour (0xffaab1ba));
    g.fillRoundedRectangle (10.0f, 10.0f, 580.0f, 405.0f, 8.000f);

    g.setColour (Colour (0xff404040));
    g.drawRoundedRectangle (10.0f, 10.0f, 580.0f, 405.0f, 8.000f, 10.000f);
	
	g.setColour(Colour(48, 48, 48));
	g.setFont(Font("Arial", 22.0f, Font::bold));
	g.drawText(CharPointer_UTF8("N\xc3\xa1pov\xc4\x9b""da"),
		228, 30, 116, 36,
		Justification::centred, true);
	
    g.setFont (Font (15.00f, Font::plain));

    g.drawMultiLineText (CharPointer_UTF8 (
		"VST plugin modul s funkcemi compressor, expander, limiter a gate. Vstupn\xc3\xad sign\xc3\xa1l je z\xc3\xa1rove\xc5\x88 pou\xc5\xbeit i jako zdroj \xc5\x99\xc3\xad""d\xc3\xad""c\xc3\xadho sign\xc3\xa1lu.\n\n"
		"Sign\xc3\xa1l v \xc5\x99\xc3\xad""d\xc3\xad""c\xc3\xad v\xc4\x9btvi je zpracov\xc3\xa1n n\xc3\xa1sledovn\xc4\x9b:\n\n"
		"Odhad ob\xc3\xa1lky sign\xc3\xa1lu \xe2\x80\x93 peak nebo RMS\n"
		"V\xc3\xbdpo\xc4\x8d""et zesilovac\xc3\xadho \xc4\x8dinitele \xe2\x80\x93 compressor, expander, limiter nebo gate\n"
		"Vyhlazen\xc3\xad zesilovac\xc3\xadho \xc4\x8dinitele \xe2\x80\x93 attack/release nebo attack/release/hold filtr\n\n"
		"Visualis\xc3\xa9ry zobrazuj\xc3\xad v\xc5\xbe""dy sign\xc3\xa1l za p\xc5\x99\xc3\xadslu\xc5\xa1n\xc3\xbdm blokem, posledn\xc3\xad visualis\xc3\xa9r zobrazuje v\xc3\xbdstupn\xc3\xad sign\xc3\xa1l.\n\n"
		"Sign\xc3\xa1l jednotliv\xc3\xbd""ch blok\xc5\xaf je mo\xc5\xben\xc3\xa9 nahr\xc3\xa1t stiskem p\xc5\x99\xc3\xadslu\xc5\xa1""en\xc3\xa9ho tla\xc4\x8d\xc3\xadtka a v\xc3\xbd""b\xc4\x9brem souboru pro ulo\xc5\xbe""en\xc3\xad vzork\xc5\xaf. Data jsou ulo\xc5\xbe""ena ve form\xc3\xa1tu CSV, jednotliv\xc3\xa9 sloupce odpov\xc3\xad""daj\xc3\xad sign\xc3\xa1lu visualis\xc3\xa9r\xc5\xaf. Nahr\xc3\xa1v\xc3\xa1n\xc3\xad je mo\xc5\xben\xc3\xa9 takt\xc3\xa9\xc5\xbe spustit pomoc\xc3\xad automatizace, parametru recording. N\xc3\xa1zev souboru posledn\xc3\xadho nahr\xc3\xa1v\xc3\xa1n\xc3\xad se zobraz\xc3\xad vedle tla\xc4\x8d\xc3\xadtka nahr\xc3\xa1v\xc3\xa1n\xc3\xad. Soubory se ukl\xc3\xa1""daj\xc3\xad na plochu u\xc5\xbeivatele."
	),
    36, 100, 527);

	
	g.setColour(Colour(100, 100, 100));
	g.setFont(Font("Arial", 13.0f, Font::plain));
	g.drawMultiLineText(CharPointer_UTF8(
		"Autor:                    Robin Zo\xc5\x88 <xzonro00@vutbr.cz>\n"
		"Vedouc\xc3\xad pr\xc3\xa1""ce:     Ing. Ji\xc5\x99\xc3\xad Schimmel, Ph.D. <schimmel@feec.vutbr.cz>"
	),
		36, 380, 450);
}