#include "shape.hpp"
#include "piece.hpp"

using namespace std;

piece::piece(int i, int j, mt19937& engine)
{
  uniform_int_distribution<int> distribution(0, NB_KINDS - 1);
  kind = distribution(engine);
  orientation = 0;
  row_pos = i;
  column_pos = j;
}

int piece::get_kind() const
{
  return kind;
}

void piece::set_kind(int k)
{
  kind = k;
}

int piece::get_orientation() const
{
  return orientation;
}

void piece::set_orientation(int o)
{
  orientation = o;
}

void piece::set_row(int row)
{
  row_pos = row;
}

int piece::get_row() const
{
  return row_pos;
}

void piece::set_column(int column)
{
  column_pos = column;
}

int piece::get_column() const
{
  return column_pos;
}

int piece::get_shape_value(int i, int j) const
{
  return PIECES[kind][orientation][i][j];
}
