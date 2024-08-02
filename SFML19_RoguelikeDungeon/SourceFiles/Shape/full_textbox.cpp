#include "Shape/full_textbox.h"

Full_Textbox::Full_Textbox(const char* text, float x, float y, float w, float h, void (*func)(), float fontSize, float fontOutline) :
	callback(func), rect(Full_Rectangle(x, y, w, h)), text(Full_Text(x + 10, y + 10, fontSize, text)) {
	this->text.setOutlineThickness(fontOutline);
}

void Full_Textbox::hover() {
	bool in = rect.getGlobalBounds().contains(sf::Vector2f(x, y));
	bool hover = rect.getHover();

	if (in && !hover) {
		rect.highlight();
		text.highlight();
	}
	else if (!in && hover) {
		rect.highlight(false);
		text.highlight(false);
	}
}

void Full_Textbox::click() {
	if (rect.getGlobalBounds().contains(sf::Vector2f(x, y)))
		callback();
}
