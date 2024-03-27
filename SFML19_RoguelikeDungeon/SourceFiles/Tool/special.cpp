#include "Tool/special.h"
#include "Manager/game_manager.h"
#include <format>

Special::Special(unsigned int id, unsigned int buy, unsigned int sell,
	const char abbre[3], const char* desc) {
	this->id = id;
	this->buy_gd = buy;
	this->sell_gd = sell;
	this->desc.setString(std::format("{}\n\nBUY: {}G\n", desc, buy_gd));

	icon.setString(abbre);
	txt.setString(abbre);
}

std::shared_ptr<Special> Special::create_special(unsigned int id)
{
	switch (id) {
	case 1:
		return std::make_shared<Storage_Perk>();
	case 2:
		return std::make_shared<Floor_Perk>();
	}
}

Storage_Perk::Storage_Perk(): Special(1, 100, 0, "SP", "Storage Perk\n\nIncrease item limit by 1.\nMax storage is 32.") {}

void Storage_Perk::use() {
	if (Game_Manager::player.get_max_itm() < Game_Manager::MAX_INV_SPELL_SLOTS) {
		Game_Manager::player.set_max_item(Game_Manager::player.get_max_itm() + 1);
		Game_Manager::log_add("Your item limit increased by 1.");
	}
}

PH_Special::PH_Special() : Special(0, 0, 0, "PH", "Placeholder Special") {}

void PH_Special::use() {}

Floor_Perk::Floor_Perk() : Special(2, 75, 0, "FP",
	"Floor Perk\n\nBuy this to move up a floor."
	"\nYou can stay in the shop"
	"\nuntil you close the window."
) {}

void Floor_Perk::use() {
	Game_Manager::next_level(true);
}