// Copyright [2024] Jarrett Williams

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Sokoban.hpp"
using SB::Sokoban;
SB::Sokoban::Sokoban(const std::string &levels) {
  std::string folder = "sokoban/";
  std::ifstream file(levels);

  if (!file.is_open()) {
    std::cerr << "Cannot open the level file!" << std::endl;
    return;
  }

  m_texture_states[Direction::Down].loadFromFile(folder + "player_05.png");
  m_texture_states[Direction::Up].loadFromFile(folder + "player_08.png");
  m_texture_states[Direction::Left].loadFromFile(folder + "player_20.png");
  m_texture_states[Direction::Right].loadFromFile(folder + "player_17.png");

  if (texture_mapping.empty()) {
    texture_mapping['#'].loadFromFile(folder + "block_06.png");
    texture_mapping['.'].loadFromFile(folder + "ground_01.png");
    texture_mapping['a'].loadFromFile(folder + "ground_04.png");
    texture_mapping['A'].loadFromFile(folder + "crate_03.png");
    texture_mapping['@'].loadFromFile(folder + "player_05.png");
  }

  m_direction = Direction::Down;

  file >> m_height >> m_width;
  file.ignore();
  level_generation.resize(m_height, std::vector<char>(m_width, ' '));

  size_t row = 0;
  std::string line;
  while (getline(file, line)) {
    for (size_t column = 0; column < line.size(); ++column) {
      char symbol = line[column];
      level_generation[row][column] = symbol;
      if (level_generation[row][column] == 'A') {
        m_crate_locations.push_back(sf::Vector2u(column, row));
      }
      if (level_generation[row][column] == 'a') {
        m_crate_spots_locations.push_back(sf::Vector2u(column, row));
      }
    }

    row++;
  }
  init_level = level_generation;
  file.close();
}

sf::Vector2u SB::Sokoban::playerLoc() const {
  for (size_t i = 0; i < level_generation.size(); ++i) {
    for (size_t j = 0; j < level_generation[i].size(); ++j) {
      if (level_generation[i][j] == '@') {
        return m_player_location = sf::Vector2u(j, i);
      }
    }
  }
  return {0, 0};
}
//  this function is used to save the current state of the game so we can undo
//  it!
void SB::Sokoban::save_currentstate() {
  gamestate current_;
  current_.player_pos = playerLoc();
  current_.crate_pos = m_crate_locations;
  current_.direction_state = m_direction;

  state_history.push_back(current_);
}
void SB::Sokoban::movePlayer(Direction dir) {
  save_currentstate();
  auto new_pos = playerLoc();
  if (dir != m_direction) {
    m_direction = dir;
    character_sprite.setTexture(m_texture_states[m_direction]);
  }
  auto lambda_pos = [&](Direction dir) {
    switch (dir) {
    case Direction::Up: {
      return sf::Vector2u(new_pos.x, new_pos.y - 1);
      m_direction = Direction::Up;
      std::cout << "moving up" << std::endl;
    } break;
    case Direction::Down: {
      return sf::Vector2u(new_pos.x, new_pos.y + 1);
      m_direction = Direction::Down;
      std::cout << "moving down" << std::endl;
    } break;
    case Direction::Right: {
      return sf::Vector2u(new_pos.x + 1, new_pos.y);
      m_direction = Direction::Right;
      std::cout << "moving right" << std::endl;
    } break;
    case Direction::Left: {
      return sf::Vector2u(new_pos.x - 1, new_pos.y);
      m_direction = Direction::Left;
      std::cout << "moving left" << std::endl;
    } break;
    case Direction::NONE: {
      return sf::Vector2u(new_pos.x, new_pos.y);
      m_direction = Direction::NONE;
      std::cout << "cant move" << std::endl;
    }
    default:
      return new_pos = playerLoc();
    }
  };

  new_pos = lambda_pos(dir);
  /*the variaous of if statments below checks boundaries within the game
  and if the player is trying to run off screen*/
  if (new_pos.y < 0) {
    new_pos.y = 0;
  } else if (new_pos.y > m_height && new_pos.y <= m_height) {
    return;
  }
  if (new_pos.x < 0) {
    new_pos.x = 0;
  } else if (new_pos.x > m_width && new_pos.y <= m_width) {
    return;
  }
  if (new_pos.x < 0 || new_pos.x >= m_width || new_pos.y < 0 ||
      new_pos.y >= m_height || level_generation[new_pos.y][new_pos.x] == '#') {
    return;
  }
  sf::Vector2u crates_pos = new_pos;
  /*as the player pushed the crates the crates position will 
  changed along with the player*/
  if (level_generation[new_pos.y][new_pos.x] == 'A') {
    switch (dir) {
    case Direction::Up: {
      crates_pos.y--;
      std::cout << "crate is being pushed up" << std::endl;
    } break;
    case Direction::Down: {
      crates_pos.y++;
      std::cout << "crate is being pushed down" << std::endl;
    } break;
    case Direction::Right: {
      crates_pos.x++;
      std::cout << "crate is being pushed right" << std::endl;
    } break;
    case Direction::Left: {
      crates_pos.x--;
      std::cout << "crate is being pushed left" << std::endl;
    } break;
    case Direction::NONE:
      break;
    }
  }
  if (crates_pos.x >= m_width || crates_pos.y >= m_height || crates_pos.x < 0 ||
      crates_pos.y < 0 || level_generation[crates_pos.y][crates_pos.x] == '#' ||
      level_generation[crates_pos.y][crates_pos.x] == 'A') {
    std::cout << "Crate move blocked by wall or another crate." << std::endl;
    return;
  }
  /*for every position of the crates that changed
  keep updating it. this will be benifical in isWon 
  function*/
  for (auto &loc : m_crate_locations) {
    if (loc == new_pos) {
      loc = crates_pos;
      break;
    }
  }
  level_generation[crates_pos.y][crates_pos.x] = 'A';
  level_generation[new_pos.y][new_pos.x] = 'a';

  crate_sprite.setPosition(crates_pos.x * TILE_SIZE, crates_pos.y * TILE_SIZE);
  level_generation[playerLoc().y][playerLoc().x] = '.';
  level_generation[new_pos.y][new_pos.x] = '@';
  character_sprite.setPosition(playerLoc().x * TILE_SIZE,
                               playerLoc().y * TILE_SIZE);
}

void SB::Sokoban::handle_inputs() {
  if (!isWon()) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      movePlayer(Direction::Up);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      movePlayer(Direction::Down);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      movePlayer(Direction::Right);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      movePlayer(Direction::Left);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U))
      undo();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
    reset();
}

bool SB::Sokoban::isWon() const {
  /*used the find function so that it will check how many crate spots
  there are and if theres crates in each of them. same for the if-
  statment below only difference is the if statment chekcs if theres
  more crates than targets*/
  if (m_crate_locations.size() == m_crate_spots_locations.size()) {
    for (const auto &crate : m_crate_locations) {
      if (std::find(m_crate_spots_locations.begin(),
                    m_crate_spots_locations.end(),
                    crate) == m_crate_spots_locations.end()) {
        return false;
      }
    }
  }

  if (m_crate_locations.size() <= m_crate_spots_locations.size()) {
    std::cout << "there are more targets than crates" << std::endl;
    for (const auto &crate : m_crate_locations) {
      if (std::find(m_crate_spots_locations.begin(),
                    m_crate_spots_locations.end(),
                    crate) == m_crate_spots_locations.end()) {
        return false;
      }
    }
  }
  /*tried to handle win if theres move crates with the find function
  but when it works it dosent work for say level6 so I did this 
  and it works with level 5*/
  if (m_crate_locations.size() > m_crate_spots_locations.size()) {
    std::cout << "theres more crates than targets!" << std::endl;
    for (const auto &targets : m_crate_spots_locations) {
      if (level_generation[targets.y][targets.x] != 'A') {
        return false;
      }
    }
  }
  /*if theres no crates or targets you automatically win!*/
  if (m_crate_locations.size() == 0 || m_crate_spots_locations.size() == 0) {
    return true;
  }
  /*if the player puts all the crates in the targets you'll win*/
  m_direction = Direction::Down;
  std::cout << "YOU WON!" << std::endl;
  return true;
}

void SB::Sokoban::reset() {
  level_generation = init_level;
  /* when we reset clear what was previously saved position 
  that we saved in the movePlayer function*/
  m_crate_locations.clear();
  m_direction = Direction::Down;

  // Reset player and crate locations
  for (size_t i = 0; i < level_generation.size(); ++i) {
    for (size_t j = 0; j < level_generation[i].size(); ++j) {
      if (level_generation[i][j] == '@') {
        m_player_location = sf::Vector2u(j, i);
        character_sprite.setPosition(j * TILE_SIZE, i * TILE_SIZE);
      } else if (level_generation[i][j] == 'A') {
        m_crate_locations.push_back(sf::Vector2u(j, i));
      }
    }
  }
  // Reset history or any tracked states if needed
  state_history.clear();
  gamestate initial_state = {m_player_location, m_crate_locations, m_direction};
  state_history.push_back(initial_state);
}

void SB::Sokoban::undo() {
  if (state_history.size() > 1) {
     // once undo is actiaved we're going to shrink the
    state_history.pop_back();

    // everythime undo is called keep going back further and further
    gamestate way_back_state = state_history.back();

    for (size_t i = 0; i < level_generation.size(); ++i) {
      for (size_t j = 0; j < level_generation[i].size(); ++j) {
        if (level_generation[i][j] == '@' || level_generation[i][j] == 'A') {
          /*check the level and if these things are out of place put them back
          to the ground*/
          level_generation[i][j] = '.';
        }
      }
    }
    /*just like the movePlyaer function. get the player to where it was before
     until it reaches its inital spawn point*/
    m_player_location = way_back_state.player_pos;
    level_generation[m_player_location.y][m_player_location.x] = '@';
    character_sprite.setPosition(m_player_location.y * TILE_SIZE,
                                 m_player_location.x * TILE_SIZE);
    /*same thing with the player we do with the crate*/
    m_crate_locations = way_back_state.crate_pos;
    for (const auto &crates : m_crate_locations) {
      level_generation[crates.y][crates.x] = 'A';
      crate_sprite.setPosition(crates.y * TILE_SIZE, crates.x * TILE_SIZE);
    }
    /*for the tiems we moved get the previous states that we had before*/
    m_direction = way_back_state.direction_state;
    character_sprite.setTexture(m_texture_states[m_direction]);
  } else {
    std::cout << "theres no turning back now!" << std::endl;
  }
}

void SB::Sokoban::draw(sf::RenderTarget &target,
                       sf::RenderStates states) const {
  std::string folder = "sokoban/";
  for (size_t i = 0; i < level_generation.size(); ++i) {
    for (size_t j = 0; j < level_generation[i].size(); ++j) {
      ground_layer.setTexture(texture_mapping.at('.'));
      ground_layer.setPosition(j * TILE_SIZE, i * TILE_SIZE);
      target.draw(ground_layer);
    }
  }
  for (const auto &pos : m_crate_spots_locations) {
    ground_layer.setTexture(texture_mapping.at('a'));
    ground_layer.setPosition(pos.x * TILE_SIZE, pos.y * TILE_SIZE);
    target.draw(ground_layer);
  }
  for (size_t i = 0; i < level_generation.size(); ++i) {
    for (size_t j = 0; j < level_generation[i].size(); ++j) {
      char symbol = level_generation[i][j];
      switch (symbol) {
      case '@': {
        character_sprite.setTexture(m_texture_states.at(m_direction));
        character_sprite.setPosition(j * TILE_SIZE, i * TILE_SIZE);
        target.draw(character_sprite);
      } break;
      case 'A': {
        crate_sprite.setTexture(texture_mapping.at('A'));
        crate_sprite.setPosition(j * TILE_SIZE, i * TILE_SIZE);
        target.draw(crate_sprite);
      } break;
      case '#': {
        top_layer.setTexture(texture_mapping.at('#'));
        top_layer.setPosition(j * TILE_SIZE, i * TILE_SIZE);
        target.draw(top_layer);
      } break;
      default:
        break;
      }
    }
  }

  if (!main_font.loadFromFile("BNJinx.ttf"))
    return;

  if (isWon()) {
    win_text.setFont(main_font);
    win_text.setCharacterSize(100);
    win_text.setFillColor(sf::Color::White);
    win_text.setOutlineColor(sf::Color::Black);
    win_text.setOutlineThickness(2);
    win_text.setString("YOU WIN!");
    sf::FloatRect text_bounds = win_text.getLocalBounds();
    float x_position =
        (target.getSize().x - text_bounds.width) / 2 - text_bounds.left;
    float y_position = (target.getSize().y / 3) - text_bounds.top;

    win_text.setPosition(x_position, y_position);
    target.draw(win_text);
  }
}

namespace SB {
std::istream &operator>>(std::istream &in, Sokoban &s) {
  sf::Vector2u location;
  in >> s.m_height >> s.m_width;
  std::string line;
  in.ignore();
  s.level_generation.resize(s.m_height, std::vector<char>(s.m_width, ' '));

  size_t row = 0;
  while (getline(in, line)) {
    for (size_t column = 0; column < line.size(); ++column) {
      char symbol = line[column];
      s.level_generation[row][column] = symbol;
      if (symbol == 'A') {
        s.m_crate_locations.push_back(sf::Vector2u(column, row));
      }
      if (symbol == 'a') {
        s.m_crate_spots_locations.push_back(sf::Vector2u(column, row));
      }
    }
    row++;
  }

  in >> s.m_player_location.x >> s.m_player_location.y;

  return in;
}

std::ostream &operator<<(std::ostream &out, const Sokoban &s) {
  out << s.height() << " " << s.width();
  for (const auto &row : s.level_generation) {
    for (const auto &lines : row) {
      out << lines;
    }
    out << "\n";
  }
  out << s.playerLoc().x << "," << s.playerLoc().y << "\n";

  for (const auto &crates : s.m_crate_locations) {
    out << "crates: " << crates.x << "," << crates.y << "\n";
  }

  for (const auto &targets : s.m_crate_spots_locations) {
    out << "targets: " << targets.x << "," << targets.y << "\n";
  }

  return out;
}
}  // namespace SB

