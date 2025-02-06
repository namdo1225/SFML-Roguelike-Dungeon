#include "Floor/door.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <Manager/sf_manager.h>

Door::Door(float x, float y, float size, Direction direction, unsigned int fromRoomId, unsigned int toRoomId, bool load) {
	if (!load)
		setPosition(direction == Left || direction == Right ? x + 6 : x, direction == Top || direction == Bottom ? y - 6 : y);
	else
		setPosition(x, y);

	// To make it less confusing, size of the longest side of the door is always the width or x value.
	setSize(sf::Vector2f(size, 9.f));

	if (direction == Left || direction == Right)
		setRotation(90.f);

	this->direction = direction;
	this->fromRoomId = fromRoomId;
	this->toRoomId = toRoomId;
}

bool Door::canPass(float x, float y, bool haveToTouch) {
	float doorPosition = (direction == Top || direction == Bottom) ? getPosition().x : getPosition().y;
	float passPosition = (direction == Top || direction == Bottom) ? x : y;

	return doorPosition <= passPosition && doorPosition + getSize().x >= passPosition &&
		(!haveToTouch || intersects(sf::FloatRect(x, y, SF_Manager::TILE, SF_Manager::TILE)));
}

Direction Door::getDirection() {
	return direction;
}

unsigned int Door::getFromRoomId() {
	return fromRoomId;
}

unsigned int Door::getToRoomId() {
	return toRoomId;
}