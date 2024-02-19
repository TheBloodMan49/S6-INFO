#pragma once
#include <random>

class piece
{
  int kind;
  int orientation;
  int row_pos;
  int column_pos;
public:
  piece(int i, int j, std::mt19937& engine);
  void set_kind(int k);
  void set_orientation(int o);
  int get_kind() const;
  int get_orientation() const;
  void set_row(int row);
  int get_row() const;
  void set_column(int column);
  int get_column() const;
  int get_shape_value(int i, int j) const;
};
