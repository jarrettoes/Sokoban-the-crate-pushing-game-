// Copyright [2024] Jarrett Williams

#include <iostream>
#include <string>
#include "Sokoban.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/test/unit_test.hpp>
using boost::test_tools::output_test_stream;
using SB::Sokoban;

BOOST_AUTO_TEST_CASE(test_cant_move_player) {
  Sokoban sb("level1.lvl");
  sb.movePlayer(SB::Direction::Left);
  BOOST_CHECK_EQUAL(sb.playerLoc().x, 2);
}

BOOST_AUTO_TEST_CASE(test_ignore_boxes) {
  Sokoban sb("level1.lvl");
  sb.movePlayer(SB::Direction::Down);
  BOOST_CHECK_EQUAL(sb.playerLoc().y, 7);
}

BOOST_AUTO_TEST_CASE(test_box_to_wall) {
  Sokoban sb("level1.lvl");
  sb.movePlayer(SB::Direction::Right);
  BOOST_CHECK_EQUAL(sb.playerLoc().x, 4);
}

BOOST_AUTO_TEST_CASE(test_box_to_box) {
  Sokoban sb("level2.lvl");

  auto inital_playerLoc = sb.playerLoc();
  sb.movePlayer(SB::Direction::Up);
  BOOST_CHECK_EQUAL(sb.playerLoc().y, inital_playerLoc.y);
}

BOOST_AUTO_TEST_CASE(test_moveOff_screen) {
  Sokoban sb("swapoff.lvl");

  sb.movePlayer(SB::Direction::Right);
  BOOST_CHECK_EQUAL(sb.playerLoc().x, 3);
}

BOOST_AUTO_TEST_CASE(test_pushOff_screen) {
  Sokoban sb("pushleft.lvl");

  sb.movePlayer(SB::Direction::Right);
  auto player_loc = sb.playerLoc();
  BOOST_CHECK_EQUAL(sb.playerLoc().x, player_loc.x);
}
