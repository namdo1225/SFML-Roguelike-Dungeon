/**
*
* File: spell_screen.cpp
* Description: Contain the implementation of the Spell_Screen class.
*/


#include "Manager/game_manager.h"
#include "Screen/spell_screen.h"
#include <format>
#include <Screen/screen.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <Shape/full_rectangle.h>
#include <string>
#include <Tool/spell.h>
#include <Tool/tool.h>

Spell_Screen::Spell_Screen() : Screen(true, false) {
	update = true;

	textRectH("Spells", 360.f, 20.f, 36.f, NULL);
	textRectH("0 / 32 spells", 340.f, 150.f, 24.f, NULL);
    textRectH("", 50.f, 720.f, 24.f, NULL);

    textboxH("Reset\nScroll", 10.f, 400.f, 120.f, 70.f, []() {
        viewSlots.reset(sf::FloatRect(0, 0, DEFAULT_SCREEN_X, DEFAULT_SCREEN_Y));
    });
    textboxH("^", 10.f, 500.f, 50.f, 50.f, []() {
        viewSlots.move(sf::Vector2f(0.f, 10.f));
    });
    textboxH("v", 80.f, 500.f, 50.f, 50.f, []() {
        viewSlots.move(sf::Vector2f(0.f, -10.f));
    });
    textboxH("Clear\nMessage", 10.f, 600.f, 120.f, 70.f, [this]() {
        texts[2].setString("");
    });

    textRectH(NULL, 50.f, -10.f, 650.f, 220.f, false);
}

bool Spell_Screen::handleClickEvent() {
    Spell* selected = Game_Manager::selectedSpell != SelectNone ? &Game_Manager::spells[Game_Manager::selectedSpell] : NULL;
    if (mouseInButton(ExitButton)) {
        viewSlots.reset(sf::FloatRect(0, 0, 1200, 800));
        Game_Manager::selectedSpell = SelectNone;
        texts[2].setString("");
        switchScreen(SpellScreen, GameScreen, false, true);
        return true;
    }
    else if (mouseInButton(UseButton) && selected) {
        if (Game_Manager::spells[Game_Manager::selectedSpell].getType() == Functional) {
            std::string spell_name = Game_Manager::spells[Game_Manager::selectedSpell].getName();
            texts[2].setString(Game_Manager::useSpell() ? std::format("You used {}.", spell_name) :
                    std::format("You failed to cast {}.", spell_name));
        }
        else {
            switchScreen(SpellScreen, GameScreen, false, true);
            openDialog(GameScreen, SpellAttackScreen);
        }
        return true;
    }
    else if (mouseInButton(DiscardButton) && selected) {
        Game_Manager::delSelectedTool(SpellTool);
        return true;
    }

    for (unsigned int i = 0; i < Game_Manager::spells.size(); i++) {
        Spell* spell = &Game_Manager::spells[i];
        if (spell->contains(slotX, slotY) && y > 220.f) {
            // Unselects spell
            if (selected == spell) {
                Game_Manager::selectedSpell = SelectNone;
                return true;
            }
            // Swaps spell position
            else if (selected && spell != selected) {
                int sx = selected->getPos('x'), sy = selected->getPos('y');
                selected->setPos(spell->getPos('x'), spell->getPos('y'));
                spell->setPos(sx, sy);
                Game_Manager::selectedSpell = SelectNone;
                return true;
            }
            // Selects a spell
            else if (!selected) {
                int sx = spell->getPos('x'), sy = spell->getPos('y');
                map_rects["inv_sp_cur_slot"].setPosition(sx - 5, sy - 5);
                map_txts["inv_sp_detail"].setString(spell->getDesc());
                Game_Manager::selectedSpell = i;
                return true;
            }
        }
    }
}

void Spell_Screen::handleHoverEvent() {
	hoverButton(UseButton);
	hoverButton(DiscardButton);

	for (unsigned int i = 0; i < MAX_INV_SPELL_SLOTS; i++)
		hoverSlot(i);
}

void Spell_Screen::handleMouseEvent() {
    if (event.type == sf::Event::MouseWheelScrolled) {
        const float delta = event.mouseWheelScroll.delta;
        viewSlots.move(sf::Vector2f(0.f, delta * 5));
    }
}

void Spell_Screen::draw() {
    window.draw(map_rects["background"]);

    window.setView(viewSlots);

    if (Game_Manager::selectedSpell != SelectNone)
        window.draw(map_rects["inv_sp_cur_slot"]);

	for (Full_Rectangle& rect : inv_sp_slots)
		window.draw(rect);

    for (Spell& sp : Game_Manager::spells)
        sp.draw();

    window.setView(viewUI);

    Screen::draw();

    if (Game_Manager::selectedSpell != SelectNone) {
        window.draw(map_rects["inv_sp_desc"]);
        window.draw(map_txts["inv_sp_desc"]);
        window.draw(map_rects["inv_sp_use"]);
        window.draw(map_txts["inv_sp_use"]);
        window.draw(map_rects["inv_sp_discard"]);
        window.draw(map_txts["inv_sp_discard"]);
        window.draw(map_txts["inv_sp_detail"]);
    }

    window.draw(map_txts["inv_sp_gold"]);
    window.draw(map_txts["inv_sp_gold_amount"]);
}

void Spell_Screen::updateDraw() {
	texts[1].setString(std::format("{} / {} spells",
		Game_Manager::spells.size(), Game_Manager::MAX_INV_SPELL_SLOTS));

    map_txts["inv_sp_gold_amount"].setString(std::to_string(Game_Manager::player.getGold()));
}