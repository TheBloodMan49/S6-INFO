#include "control.hpp"

using namespace std;

heuristic_control::heuristic_control(const vector<double>& weights)
  : weights(weights)
{
}

static vector<vector<tetris::action>> sequence[NB_KINDS] =
  {
    { // O_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // I_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP},

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // S_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP},

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // Z_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP},

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // L_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // J_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
    { // T_KIND
      { tetris::DROP },

      { tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN, tetris::ROTATE_RIGHT, tetris::MOVE_DOWN,tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_LEFT, tetris::DROP },

      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
      { tetris::MOVE_DOWN, tetris::ROTATE_LEFT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::MOVE_DOWN, tetris::MOVE_RIGHT, tetris::DROP },
    },
  };

bool heuristic_control::apply_seq(tetris& t, const vector<tetris::action>& seq, int& nb_lines)
{
  nb_lines = 0;
  for (uint8_t a : seq)
    {
      if (!t.do_action(tetris::action(a))) return false;
    }
  t.drop_current_piece();
  nb_lines = t.count_possible_lines();
  return true;
}

vector<tetris::action> heuristic_control::operator()(const tetris& t)
{
  vector<tetris::action> res = { tetris::DROP };
  double best_value = numeric_limits<double>::lowest();
  int current_piece_kind = t.get_current_piece().get_kind();
  int next_piece_kind = t.get_next_piece().get_kind();
  for (const auto& seq1 : sequence[current_piece_kind])
    {
      int nb_lines_seq1 = 0;
      tetris t1 = t;
      if (!apply_seq(t1, seq1, nb_lines_seq1)) continue;
      if (t1.is_game_over())
        {
          double v = evaluate_position(t1, nb_lines_seq1);
          if (v > best_value)
            {
              best_value = v;
              res = seq1;
            }
          continue;
        }
      t1.set_current_piece(t1.get_next_piece());
      for (const auto& seq2 : sequence[next_piece_kind])
        {
          tetris t2 = t1;
          int nb_lines_seq2;
          if (apply_seq(t2, seq2, nb_lines_seq2))
            {
              double v = evaluate_position(t2, nb_lines_seq1 + nb_lines_seq2);
              if (v > best_value)
                {
                  best_value = v;
                  res = seq1;
                }
            }
        }
    }
  return res;
}

double heuristic_control::evaluate_position(tetris& t, int nb_lines)
{
  double res = 0;

  int max_height = 0;
  // Height of columns
  for (int i = 0; i < BOARD_WIDTH; i++) {
      int height = get_height(t, i);
      res += height * weights[0+i];
      if (height > max_height) max_height = height;
  }

  // Max height of columns
  res += max_height * weights[BOARD_WIDTH];

  int max_diff = 0;
  // Difference of height between columns
  int last_height = get_height(t, 0);
  for (int i = 1; i < BOARD_WIDTH; i++) {
      int height = get_height(t, i);
      res += abs(last_height - height) * weights[BOARD_WIDTH + i];
      last_height = height;
      if (abs(last_height - height) > max_diff) max_diff = abs(last_height - height);
  }

  // Number of holes in each column
  for (int i = 0; i < BOARD_WIDTH; i++) {
      int holes = get_holes(t, i);
      res += holes * weights[2*BOARD_WIDTH + i];
  }

  // Max difference of height between columns
  res += max_diff * weights[3*BOARD_WIDTH];

  // Number of lines destroyed by the last move
  res += nb_lines * nb_lines * weights[3*BOARD_WIDTH + 1];

  // Number of covered holes
  res += covered_hole_count(t) * weights[3*BOARD_WIDTH + 2];

  // Height of the highest covered hole
  res += get_highest_covered_hole(t) * weights[3*BOARD_WIDTH + 3];

  return res;
}

int heuristic_control::get_height(const tetris& t, int x)
{
  for (int i = 0; i < BOARD_HEIGHT; i++)
    {
      if (t.get(i,x) != FREE)
        {
          return BOARD_HEIGHT - i;
        }
    }
  return 0;
}

int heuristic_control::get_holes(const tetris& t, int x)
{
  // If the position has blocks to the left and right, it is a hole.
  // If it is completely covered, it is a worse hole

  int res = 0;
  bool block_above = false;
  for (int i = 0; i < BOARD_HEIGHT; i++)
    {
      if (x > 0 && t.get(i,x) == FREE && t.get(i,x-1) != FREE) {
        res++;
      }
      // For now we don't consider the right side to get the ai to build a tetris on the right column
      /*
      if (x < BOARD_WIDTH - 1 && t.get(i,x) == FREE && t.get(i,x+1) != FREE) {
        res++;
      }
      */
      if (t.get(i,x) != FREE) {
        block_above = true;
      }
      else if (block_above) {
        res++;
      }
    }

  return res;
}

int heuristic_control::covered_hole_count(const tetris& t)
{
  int res = 0;
  for (int x = 0; x < BOARD_WIDTH; x++)
    {
      bool block_above = false;
      for (int i = 0; i < BOARD_HEIGHT; i++)
        {
          if (t.get(i,x) != FREE)
            {
              block_above = true;
            }
          else if (block_above)
            {
              res++;
            }
        }
    }
  return res;
}

int heuristic_control::get_highest_covered_hole(const tetris& t)
{
  int res = 0;
  for (int x = 0; x < BOARD_WIDTH; x++)
    {
      bool block_above = false;
      for (int i = 0; i < BOARD_HEIGHT; i++)
        {
          if (t.get(i,x) != FREE)
            {
              block_above = true;
            }
          else if (block_above)
            {
              res = i;
            }
        }
    }
  return res;
}


int evaluate_control(const vector<double>& weights)
{
  const int NB_GAMES = 2;
  const int MAX_NB_ITERATIONS = 10000;
  const int SEED = 27;
  using namespace std;
  mt19937 engine(SEED);
  heuristic_control control(weights);
  int res = 0;
  for (int i = 0; i < NB_GAMES; ++i)
    {
      tetris t(engine);
      int nb_iterations = 0;
      while (nb_iterations < MAX_NB_ITERATIONS)
        {
          ++nb_iterations;
          for (auto a : control(t))
            {
              t.do_action(a);
            }
          if (t.is_current_piece_fallen())
            {
              int nb_lines = t.delete_possible_lines();
              t.add_to_lines(nb_lines);
              t.add_to_score(nb_lines * nb_lines);
              if (t.is_game_over()) break;
              t.set_current_piece(t.get_next_piece());
              t.set_next_piece_at_random(engine);
            }
        }
      res += t.get_score() + nb_iterations*0.1;
    }
  return res;
}
