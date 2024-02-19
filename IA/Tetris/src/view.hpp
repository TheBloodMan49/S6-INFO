#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "tetris.hpp"

std::ostream& operator<<(std::ostream& os, const tetris& t);

struct ascii_view
{
  void draw(const tetris& t);
};

struct graphical_view
{
  sf::RenderWindow window;
  sf::Font font;
  static const sf::Color BACKGROUND;
  graphical_view();
  void draw(const tetris& t);
};
