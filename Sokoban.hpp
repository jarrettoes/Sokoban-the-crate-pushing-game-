// Copyright [2024] Jarrett Williams
#pragma once

#ifndef SOKOBAN_HPP
#define SOKOBAN_HPP
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace SB {
enum class Direction { NONE, Up, Down, Left, Right };
struct gamestate {  // had to make a struct to keep track of the game state!
  sf::Vector2u player_pos;
  std::vector<sf::Vector2u> crate_pos;
  Direction direction_state;
};
class Sokoban : public sf::Drawable {
 public:
  static const int TILE_SIZE = 64;

  Sokoban() = default;
  explicit Sokoban(const std::string &levels);  // Optional

  unsigned int pixelHeight() const { return m_height * TILE_SIZE; }  // Optional
  unsigned int pixelWidth() const { return m_width * TILE_SIZE; }    // Optional

  unsigned int height() const { return m_height; }
  unsigned int width() const { return m_width; }

  sf::Vector2u playerLoc() const;

  friend std::istream &operator>>(std::istream &in, Sokoban &s);
  friend std::ostream &operator<<(std::ostream &out, const Sokoban &s);

  void handle_inputs();
  bool isWon() const;
  void save_currentstate();

  void movePlayer(Direction dir);
  void reset();

  void undo();  // Optional XC
  void redo();  // Optional XC

 protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

 private:
  // Any fields you need go here.
  mutable sf::Vector2u m_player_location;
  mutable std::vector<sf::Vector2u> m_crate_locations;
  mutable std::vector<sf::Vector2u> m_crate_spots_locations;
  unsigned int m_height;
  unsigned int m_width;
  mutable bool win_flag;
  bool m_reset;
  bool m_redo;
  bool m_undo;
  mutable sf::Sprite character_sprite;
  mutable sf::Sprite crate_sprite;
  mutable Direction m_direction;
  mutable sf::Text elapsedTimeText;
  mutable sf::Sprite ground_layer;
  mutable sf::Sprite target_layer;
  mutable sf::Sprite top_layer;
  mutable sf::Font main_font;
  mutable sf::Text win_text;

  std::map<char, sf::Texture> texture_mapping;
  std::vector<std::vector<char>> level_generation, level_history;
  std::vector<std::vector<char>> init_level;
  std::vector<gamestate> state_history;
  std::unordered_map<Direction, sf::Texture> m_texture_states;
};
std::ostream &operator<<(std::ostream &out, const Sokoban &s);
std::istream &operator>>(std::istream &in, Sokoban &s);
}  // namespace SB

#endif

