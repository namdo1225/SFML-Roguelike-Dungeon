#include "Shape/full_textbox.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>

Full_Textbox::Full_Textbox(const char* text, float x, float y, float w, float h, void (*func)(), float fontSize, float fontOutline) :
	callback(func), rect(Full_Rectangle(x, y, w, h)) {
	this->text = Full_Text(x + 10, y + 10, fontSize, text);

	// Credits: https://stackoverflow.com/questions/14505571/centering-text-on-the-screen-with-sfml
	sf::FloatRect textRect = this->text.getLocalBounds();
	this->text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	this->text.setPosition(sf::Vector2f(w / 2.0f + x, h / 2.0f + y));

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
