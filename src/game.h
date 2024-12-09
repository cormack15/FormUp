#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

constexpr uint16_t gameWidth = 500;
constexpr uint16_t gameHeight = 900;

extern sf::Texture spritesheet;
extern sf::Texture modifierSpritesheet;
extern sf::Texture background;
extern sf::Texture tutorial;
extern sf::Texture pauseButton;
extern sf::Texture pauseMenu;
extern sf::Texture gameoverMenu;
extern sf::Texture optionsMenu;

extern sf::SoundBuffer modifierSFXBuffer;
extern sf::SoundBuffer targetSFXBuffer;
extern sf::SoundBuffer damageSFXBuffer;