#include "Shape/full_textbox.h"

Full_Textbox::Full_Textbox(const char* text, float x, float y, float w, float h, float size) {
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