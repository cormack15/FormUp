#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

constexpr uint16_t gameWidth = 500;
constexpr uint16_t gameHeight = 900;

extern sf::Texture spritesheet;
extern sf::Texture modifierSpritesheet;
extern sf::Texture background;

extern sf::SoundBuffer modifierSFX;