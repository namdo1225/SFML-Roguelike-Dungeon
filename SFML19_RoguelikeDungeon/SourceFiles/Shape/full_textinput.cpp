#include "Shape/full_textinput.h"
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <cctype>

Full_TextInput::Full_TextInput(const char* defaultText, unsigned int length, float x, float y, float w, float h, InputValidation validation, float fontSize, float fontOutline) :
	maxLength(length), rect(Full_Rectangle(x, y, w, h)), validation(validation) {
	text = Full_Text(x + 10, y + 10, fontSize, defaultText);
	text.setPhysical(x + 10, y + 10, defaultText, fontSize, fontOutline);
	text.setOutlineThickness(fontOutline);
}

void Full_TextInput::click() {
	const bool oldFocus = focused;
	focused = rect.getGlobalBounds().contains(sf::Vector2f(x, y));
	if (oldFocus != focused)
		rect.setOutlineThickness(focused ? 10.f : 2.f);
}

void Full_TextInput::handleTextEvent() {
	if (focused) {
		char letter{ static_cast<char>(event.text.unicode) };
		sf::String cur_string = text.getString();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !text.getString().isEmpty())
			text.setString(cur_string.substring(0, cur_string.getSize() - 1));
		else if (cur_string.getSize() < maxLength)
		{
			if (validation == AllValidation ||
				(validation == AlphanumericValidation && isalnum(letter)) ||
				(validation == AlphabetValidation && isalpha(letter)) ||
				(validation == NumberValidation && '0' <= letter && letter <= '9') ||
				(validation == TrueFalseValidation && '0' <= letter && letter <= '1'))
				text.setString(cur_string + letter);
		}
	}
}

void Full_TextInput::clear() {
	text.setString("");
}
