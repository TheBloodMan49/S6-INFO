#include "tetris.hpp"
#include <iostream>

using namespace std;

tetris::tetris(mt19937& engine)
  : current_piece(PIECE_I, PIECE_J, engine),
    next_piece(PIECE_I, PIECE_J, engine)
{
  for (int i = 0; i < BOARD_HEIGHT; ++i)
    {
      for (int j = 0; j < BOARD_WIDTH; ++j)
        {
          area[i][j] = FREE;
        }
    }
  score = 0;
  level = 0;
  lines = 0;
  game_over = false;
  draw_piece(current_piece);
}

int8_t tetris::get(int i, int j) const
{
  return area[i][j];
}

void tetris::set_current_piece(const piece& p)
{
  current_piece = p;
  draw_piece(current_piece);
}

const piece& tetris::get_current_piece() const
{
  return current_piece;
}

void tetris::set_next_piece(const piece& p)
{
  next_piece = p;
}

void tetris::set_next_piece_at_random(mt19937& engine)
{
  next_piece = piece(PIECE_I, PIECE_J, engine);
}

const piece& tetris::get_next_piece() const
{
  return next_piece;
}

bool tetris::collision(int y, int x, int kind, int orientation) const
{
  int i, j;
  for (i = 0; i < SIZE; ++i)
    {
      for (j = 0; j < SIZE; ++j)
        {
          if (PIECES[kind][orientation][i][j] != 0 &&
              (y + i < 0 || y + i >= BOARD_HEIGHT || x + j < 0 || x + j >= BOARD_WIDTH || area[y + i][x + j] != FREE))
            {
              return true;
            }
        }
    }
  return false;
}

void tetris::fill(int y, int x, int kind, int orientation, int8_t filling)
{
  int i, j;
  for (i = 0; i < SIZE; ++i)
    {
      for (j = 0; j < SIZE; ++j)
        {
          if (PIECES[kind][orientation][i][j] != 0)
            {
              area[y + i][x + j] = filling;
            }
        }
    }
}

void tetris::draw_piece(const piece& p)
{
  fill(p.get_row(), p.get_column(), p.get_kind(), p.get_orientation(), p.get_kind());
}

void tetris::clear_piece(const piece& p)
{
  int i = p.get_row();
  int j = p.get_column();

  int k = p.get_kind();
  int o = p.get_orientation();

  fill(i, j, k, o, FREE);
}

void tetris::clear()
{
  int i, j;
  for (i = 0; i < BOARD_HEIGHT; ++i)
    {
      for (j = 0; j < BOARD_WIDTH; ++j)
        {
          area[i][j] = FREE;
        }
    }
}

bool tetris::is_current_piece_movable(int i, int j)
{
  clear_piece(current_piece);
  bool movable = !collision(i, j, current_piece.get_kind(), current_piece.get_orientation());
  draw_piece(current_piece);
  return movable;
}

bool tetris::is_current_piece_rotable(int orientation)
{
  clear_piece(current_piece);
  bool rotable = !collision(current_piece.get_row(), current_piece.get_column(), current_piece.get_kind(), orientation);
  draw_piece(current_piece);
  return rotable;
}

bool tetris::do_action(action a)
{
  if (a == ROTATE_LEFT || a == ROTATE_RIGHT) return rotate_current_piece(a);
  if (a == DROP) return drop_current_piece();
  return move_current_piece(a);
}

bool tetris::move_current_piece(int dir)
{
  int i = current_piece.get_row();
  int j = current_piece.get_column();

  if (dir == MOVE_LEFT) --j;
  else if (dir == MOVE_RIGHT) ++j;
  else ++i;
  if (is_current_piece_movable(i, j))
    {
      clear_piece(current_piece);
      current_piece.set_row(i);
      current_piece.set_column(j);
      draw_piece(current_piece);
      return true;
    }
  return false;
}

bool tetris::move_current_piece_down()
{
  return move_current_piece(MOVE_DOWN);
}

bool tetris::move_current_piece_left()
{
  return move_current_piece(MOVE_LEFT);
}

bool tetris::move_current_piece_right()
{
  return move_current_piece(MOVE_RIGHT);
}

bool tetris::rotate_current_piece(int dir)
{
  int o = current_piece.get_orientation();
  if (dir == ROTATE_LEFT) o = o > 0 ? o - 1 : NB_ROTATIONS - 1;
  else o = o < NB_ROTATIONS - 1 ? o + 1 : 0;

  if (is_current_piece_rotable(o))
    {
      clear_piece(current_piece);
      current_piece.set_orientation(o);
      draw_piece(current_piece);
      return true;
    }
  return false;
}

bool tetris::rotate_current_piece_left()
{
  return rotate_current_piece(ROTATE_LEFT);
}

bool tetris::rotate_current_piece_right()
{
  return rotate_current_piece(ROTATE_RIGHT);
}

void tetris::delete_line(int row)
{
  int i, j;
  for (i = row; i > 0; --i)
    {
      for (j = 0; j < BOARD_WIDTH; ++j)
        {
          area[i][j] = area[i-1][j];
        }
    }
}

int tetris::delete_possible_lines()
{
  int nb_lines_deleted = 0;
  int i, j;
  for (i = 0; i < BOARD_HEIGHT; ++i)
    {
      for (j = 0; j < BOARD_WIDTH && area[i][j] != FREE; ++j);
      if (j == BOARD_WIDTH)
        {
          ++nb_lines_deleted;
          delete_line(i);
        }
    }
  return nb_lines_deleted;
}

int tetris::count_possible_lines()
{
  int nb_lines_deleted = 0;
  int i, j;
  for (i = 0; i < BOARD_HEIGHT; ++i)
    {
      for (j = 0; j < BOARD_WIDTH && area[i][j] != FREE; ++j);
      if (j == BOARD_WIDTH)
        {
          ++nb_lines_deleted;
        }
    }
  return nb_lines_deleted;
}

bool tetris::drop_current_piece()
{
  int i = current_piece.get_row();
  int j = current_piece.get_column();
  while (is_current_piece_movable(++i, j))
    move_current_piece_down();
  return true;
}

bool tetris::is_current_piece_fallen()
{
  int i = current_piece.get_row();
  int j = current_piece.get_column();

  if (is_current_piece_movable(i + 1, j)) return false;

  return true;
}

bool tetris::get_game_over() const
{
  return game_over;
}

bool tetris::is_game_over()
{
  if (game_over) return true;
  int j;
  for (j = 0; j < BOARD_WIDTH; ++j)
    {
      if (area[0][j] != FREE)
        return game_over = true;
    }
  return game_over = false;
}

unsigned int tetris::get_score() const
{
  return score;
}

unsigned int tetris::get_level() const
{
  return level;
}

unsigned int tetris::get_lines() const
{
  return lines;
}

void tetris::add_to_score(unsigned int score)
{
  this->score += score;
}

void tetris::set_level(unsigned int level)
{
  this->level = level;
}

void tetris::add_to_lines(unsigned int lines)
{
  this->lines += lines;
}
