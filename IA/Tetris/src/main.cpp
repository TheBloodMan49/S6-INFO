#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include <list>
#include <bitset>
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <climits>
#include <cmath>
#include <stack>
#include <queue>
#include <cfloat>
#include <initializer_list>
#include <iomanip>
#include <functional>
#include <unordered_map>
#include <regex>
#include "tetris.hpp"
#include "view.hpp"
#include "control.hpp"
#include "cem.hpp"
#include <chrono>
#include <fstream>

using namespace std;
using namespace sf;

void play(const vector<double>& weights)
{
  mt19937 engine(chrono::system_clock::now().time_since_epoch().count());
  tetris game(engine);
  //ascii_view view;
  graphical_view view;
  heuristic_control control(weights);
  play_tetris(game, view, control, engine, 0u);
}

void learn(const vector<double>& weights)
{
  cem cem(weights, 30, 75, 0.3, evaluate_control);
  cem.run();
}

int main(int argc, char *argv[])
{
  vector<double> weights;
  double weight;
  ifstream file("weights.txt");
  while (file >> weight)
  {
    weights.push_back(weight);
  }

  if (argc > 1 && strcmp(argv[1], "learnr") == 0)
  {
    for (double & w : weights)
    {
      w = 0.0;
    }
    learn(weights);
  }
  else if (argc > 1 && strcmp(argv[1], "learn") == 0)
  {
    learn(weights);
  }
  else if (argc > 1 && strcmp(argv[1], "play") == 0)
  {
    play(weights);
  }
  else
  {
    cout << "Usage: " << argv[0] << " [learnr|learn|play]" << endl;
  }

  file.close();

  return 0;
}
