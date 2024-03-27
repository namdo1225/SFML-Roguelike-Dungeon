#include "Manager/sf_manager.h"
#include "Manager/setting_manager.h"

const unsigned int SF_Manager::LOG_SIZE;
const unsigned int SF_Manager::MAX_INV_SPELL_SLOTS;

float SF_Manager::x;
float SF_Manager::y;

float SF_Manager::world_x;
float SF_Manager::world_y;

sf::RenderWindow SF_Manager::window = sf::RenderWindow();
sf::Event SF_Manager::event;

sf::Vector2i SF_Manager::mouse_pos;
sf::Vector2f SF_Manager::mouse_world;

sf::View SF_Manager::viewUI{ sf::FloatRect(0, 0, 1200, 800) };
sf::View SF_Manager::viewWorld{ sf::FloatRect(0, 0, 1200, 800) };
sf::View SF_Manager::viewLog{ sf::FloatRect(0, 0, 1200, 800) };

std::vector<Full_Text> SF_Manager::logs;

void SF_Manager::log_add(const char* log) {
    unsigned int size = logs.size();
    if (size == LOG_SIZE) {
        logs.erase(logs.begin());
        for (unsigned int i = 0; i < size; i++) {
            logs[i].setPosition(755, logs[i].getPosition().y - 30);
        }
    }
    logs.push_back(Full_Text(755.f, 465 + size * 30, 24.f, log, Setting_Manager::theme, false, Setting_Manager::light));
    logs.back().setThemeAndHover(Setting_Manager::theme, false, Setting_Manager::light, true);

    bool less_eight = logs.size() < 8;
    for (unsigned int i = less_eight ? 0 : logs.size() - 8, offset = 0; i < logs.size(); i++, offset++)
        logs[i].setPosition(755.f, 500.f + 30.f * offset);
}

void SF_Manager::log_view(bool screen) {
    if (screen) {
        for (unsigned int i = 0; i < logs.size(); i++)
            logs[i].setPosition(60.f, 120.f + 30.f * i);
    }
    else {
        bool less_eight = logs.size() < 8;
        for (unsigned int i = less_eight ? 0 : logs.size() - 8, offset = 0; i < logs.size(); i++, offset++)
            logs[i].setPosition(755.f, 500.f + 30.f * offset);
    }
}