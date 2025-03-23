/**
*
* File: full_textinput.cpp
* Description: Contain the implementation of the Full_TextInput class.
*
*/

#include "Shape/full_textinput.h"
#include <cctype>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <Shape/full_rectangle.h>
#include <Shape/full_text.h>
#include <string>

Full_TextInput* Full_TextInput::chosenInput = NULL;

Full_TextInput::Full_TextInput(const char* defaultText, unsigned int length, float x, float y, float w, float h, InputValidation validation,
	float fontSize, float fontOutline, int minNumber, int maxNumber) :
	maxLength(length), rect(Full_Rectangle(x, y, w, h)), validation(validation) {
	text = Full_Text(x + 10, y + 10, fontSize, defaultText);
	text.setPhysical(x + 10, y + 10, defaultText, fontSize, fontOutline);
	text.setOutlineThickness(fontOutline);

	this->minNumber = minNumber;
	this->maxNumber = maxNumber;
}

bool Full_TextInput::click() {
	const bool oldFocus = focused;
	focused = rect.getGlobalBounds().contains(sf::Vector2f(x, y));

	if (chosenInput != NULL && chosenInput != this)
		chosenInput->click();

	if (focused)
		chosenInput = this;

	if (oldFocus != focused)
		rect.setOutlineThickness(focused ? 10.f : 2.f);
	return focused;
}

void Full_TextInput::handleTextEvent() {
	if (focused) {
		char letter{ static_cast<char>(event.text.unicode) };
		sf::String cur = text.getString();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && !text.getString().isEmpty())
			text.setString(cur.substring(0, cur.getSize() - 1));
		else if (cur.getSize() < maxLength)
		{
			std::string temp = cur + letter;
			bool isNum = validation == NumberValidation && '0' <= letter && letter <= '9';
			int tempNum = isNum ? std::stoi(temp) : -1;

			if (validation == AllValidation ||
				(validation == AlphanumericValidation && isalnum(letter)) ||
				(validation == AlphanumspaceValidation && (isalnum(letter) || letter == ' ')) ||
				(validation == AlphabetValidation && isalpha(letter)) ||
				(isNum && tempNum <= maxNumber && tempNum >= minNumber) ||
				(validation == TrueFalseValidation && '0' <= letter && letter <= '1'))
				text.setString(cur + letter);
		}
	}
}

void Full_TextInput::clear() {
	text.setString("");
}

void Full_TextInput::unfocus() {
	chosenInput = NULL;
}
