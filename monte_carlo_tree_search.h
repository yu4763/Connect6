#ifndef __MONTE_CARLO_TREE_SEARCH_H__
#define __MONTE_CARLO_TREE_SEARCH_H__

#include <cstdlib>
#include <thread>
#include <chrono>
#include <random>
using namespace std;

// Namespace for save constants of monte carlo tree search
namespace mct_const {
  const double TERMINIATE_TIME_PADDING = 0.01;
  const int NUMBER_OF_THREADS = 4;
  const int NUMBER_OF_MAX_CHILD_NODES = 12;
};

class Position {
  public:
    Position() : xpos_(0), ypos_(0) {}
    Position(int x, int y) : xpos_(x), ypos_(y) {}
    Position(const Position& ref) : xpos_(ref.xpos_), ypos_(ref.ypos_) {}
    Position& operator=(const Position& ref) {
      xpos_ = ref.xpos_;
      ypos_ = ref.ypos_;
      return *this;
    }

  private:
    int xpos_;
    int ypos_;
};

class State {
  public:
    State() : parent_(NULL), uct_value_(123456789), avg_wins_(0), number_of_visiting_(0) {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        child_list_[i] = NULL;
      }
    }
    State& SelectionAndExpansion();
    void Evaluation();
    void Update();
    Position BestChoice();

  private:
    State* parent_;
    State* child_list_[mct_const::NUMBER_OF_MAX_CHILD_NODES];
    double uct_value_;
    double avg_wins_;
    int number_of_visiting_;
};

Position MonteCarloTreeSearch(State& current, double recv_time);

#endif
