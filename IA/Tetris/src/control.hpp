#pragma once

#include "tetris.hpp"
#include "view.hpp"
#include <chrono>
#include <functional>
#include <vector>

template <typename View, typename Control>
void play_tetris(tetris& t, View& view, Control& control, std::mt19937& engine, unsigned int milliseconds)
{
  using namespace std;
  auto wait = [&](unsigned int ms)
    {
      const chrono::system_clock::time_point start = chrono::system_clock::now();
      while (chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - start).count() < ms) view.draw(t);
    };
  view.draw(t);
  wait(milliseconds);
  while (true)
    {
      for (auto a : control(t))
        {
          t.do_action(a);
          view.draw(t);
          wait(milliseconds);
        }
      if (t.is_current_piece_fallen())
        {
          int nb_lines = t.delete_possible_lines();
          t.add_to_lines(nb_lines);
          t.add_to_score(nb_lines * nb_lines);// * (t.get_level() + 1) * 10);
          //          t.set_level(t.get_lines() / 10);
          if (t.is_game_over())
            {
              view.draw(t);
              cout << "Score: " << t.get_score() << endl;
              cout << "Lines: " << t.get_lines() << endl;
              cout << "Level: " << t.get_level() << endl;
              break;
            }
          t.set_current_piece(t.get_next_piece());
          t.set_next_piece_at_random(engine);
        }
      view.draw(t);
      wait(milliseconds);
    }
}

struct heuristic_control
{
  const std::vector<double>& weights;
  heuristic_control(const std::vector<double>& weights);
  std::vector<tetris::action> operator()(const tetris& t);
  double evaluate_position(tetris& t, int nb_lines);
  static bool apply_seq(tetris& t, const std::vector<tetris::action>& seq, int& nb_lines);

    static int get_height(const tetris &t, int x);

  static int get_holes(const tetris &t, int x);

  static int covered_hole_count(const tetris &t);

  int get_highest_covered_hole(const tetris &t);
};

int evaluate_control(const std::vector<double>& weights);
