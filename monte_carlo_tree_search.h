#ifndef __MONTE_CARLO_TREE_SEARCH_H__
#define __MONTE_CARLO_TREE_SEARCH_H__

#include <cstdlib>
#include <cmath>
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
    State() : parent_(NULL), uct_value_(123456789), number_of_wins_(0), number_of_visiting_(0), change_idx_(-1) {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        child_list_[i] = NULL;
      }
      memset(board_, 0, sizeof(board_));
    }
    State(const int* board) : parent_(NULL), uct_value_(123456789), number_of_wins_(0), number_of_visiting_(0), change_idx_(-1){
      for (int i = 0; i < 361; i++) {
        board_[i] = board[i];
      }
    }
    State& SelectionAndExpansion();
    int Evaluation();
    void VirtualPlay(int& win_count);
    void Update(int result);
    Position BestChoice();

  private:
    State* parent_;
    State* child_list_[mct_const::NUMBER_OF_MAX_CHILD_NODES];
    double uct_value_;
    int number_of_wins_;
    int number_of_visiting_;
    int change_idx_;
    int board_[361];
};

Position MonteCarloTreeSearch(State& current, double recv_time);

#endif
