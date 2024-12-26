// Copyright [2024] Jarrett Williams

#include <iomanip>
#include <iostream>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Sokoban.hpp"
using SB::Sokoban;
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Error: Not enough input arguments provided!" << std::endl;
    return EXIT_FAILURE;
  }

  std::string desired_file = argv[1];
  SB::Sokoban sb(desired_file);

  sf::RenderWindow main_window(sf::VideoMode(sb.pixelWidth(), sb.pixelHeight()),
                               "sokoban");

  sf::Vector2u window_dim(sb.pixelWidth(), sb.pixelHeight());
  sf::Vector2u middle_window(window_dim.x / 2, window_dim.y / 2);

  sf::Font font;
  if (!font.loadFromFile("BNJinx.ttf")) {
    return EXIT_FAILURE;
  }

  /* the text below is all UI one for the tiem, undo and reset */
  sf::Text elapsedTimeText, reset_text, undo_text;
  elapsedTimeText.setFont(font);
  elapsedTimeText.setCharacterSize(24);
  elapsedTimeText.setFillColor(sf::Color::White);
  elapsedTimeText.setOutlineColor(sf::Color::Black);
  elapsedTimeText.setOutlineThickness(2);
  elapsedTimeText.setPosition(10, 10);

  reset_text.setFont(font);
  reset_text.setCharacterSize(24);
  reset_text.setFillColor(sf::Color::White);
  reset_text.setOutlineColor(sf::Color::Black);
  reset_text.setOutlineThickness(2);
  reset_text.setString("r: reset");
  reset_text.setPosition(middle_window.x + 160, 10);

  undo_text.setFont(font);
  undo_text.setCharacterSize(24);
  undo_text.setFillColor(sf::Color::White);
  undo_text.setOutlineColor(sf::Color::Black);
  undo_text.setOutlineThickness(2);
  undo_text.setString("u: undo");
  undo_text.setPosition(middle_window.x - 40, 10);

  sf::Clock clock;
  std::cout << sb;
  main_window.setFramerateLimit(10);

  while (main_window.isOpen()) {
    sf::Event events;

    while (main_window.pollEvent(events)) {
      if (events.type == sf::Event::Closed)
        main_window.close();
    }

    sb.handle_inputs();
    sf::Time elapsed = clock.getElapsedTime();
    int totalSeconds = static_cast<int>(elapsed.asSeconds());

    // Calculate minutes and seconds
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    // Format the elapsed time in MM:SS format
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2)
        << std::setfill('0') << seconds;
    elapsedTimeText.setString(oss.str());

    main_window.clear();
    main_window.draw(sb);
    main_window.draw(elapsedTimeText);
    main_window.draw(reset_text);
if(!sb.isWon()) {  // whats there to undo if you won?
        main_window.draw(undo_text);
      }
    main_window.display();
  }
  return 0;
}  // main.cpp



