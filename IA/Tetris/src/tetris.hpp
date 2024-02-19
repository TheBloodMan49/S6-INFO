#pragma once

#include "piece.hpp"
#include "shape.hpp"
#include <cstdint>
#include <array>
#include <random>

static const int FREE = -1;
static const int BOARD_HEIGHT = 20;
static const int BOARD_WIDTH = 10;
static const int PIECE_I = -1;
static const int PIECE_J = 3;


/*
                        TOP

                    0   1   2   3         BOARD_WIDTH - 1
                  +---+---+---+---+           +---+
 0                |   |   |   |   | ...       |   |
                  +---+---+---+---+           +---+
 1                |   |   |   |   | ...       |   |
                  +---+---+---+---+           +---+
 2                |   |   |   |   | ...       |   |
                  +---+---+---+---+           +---+
                    .
                    .
                    .
                  +---+---+---+---+           +---+
 BOARD_HEIGHT - 1 |   |   |   |   | ...       |   |
                  +---+---+---+---+           +---+

                        BOTTOM

 */

class tetris
{
  piece current_piece;
  piece next_piece;
  std::array<std::array<int8_t, BOARD_WIDTH>, BOARD_HEIGHT> area;
  unsigned int score;
  unsigned int level;
  unsigned int lines;
  bool game_over;
public:
  enum action { ROTATE_LEFT, ROTATE_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, DROP };
  tetris(std::mt19937& engine);
  int8_t get(int i, int j) const;
  void set_current_piece(const piece& p);
  const piece& get_current_piece() const;
  void set_next_piece(const piece& p);
  void set_next_piece_at_random(std::mt19937& engine);
  const piece& get_next_piece() const;
  bool collision(int i, int j, int kind, int orientation) const;
  void fill(int i, int j, int kind, int orientation, int8_t filling);
  void draw_piece(const piece& p);
  void clear_piece(const piece& p);
  void clear();
  bool is_current_piece_movable(int i, int j);
  bool is_current_piece_rotable(int o);
  bool do_action(action a);
  bool move_current_piece(int dir);
  bool move_current_piece_down();
  bool move_current_piece_left();
  bool move_current_piece_right();
  bool rotate_current_piece(int dir);
  bool rotate_current_piece_left();
  bool rotate_current_piece_right();
  void delete_line(int y);
  int delete_possible_lines();
  int count_possible_lines();
  bool drop_current_piece();
  bool is_current_piece_fallen();
  bool is_game_over();
  unsigned int get_score() const;
  unsigned int get_level() const;
  unsigned int get_lines() const;
  void add_to_score(unsigned int score);
  void set_level(unsigned int level);
  void add_to_lines(unsigned int lines);
  bool get_game_over() const;
};
