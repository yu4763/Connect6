#ifndef __MONTE_CARLO_TREE_SEARCH_H__
#define __MONTE_CARLO_TREE_SEARCH_H__

#include <cstdlib>
#include <cmath>
#include <iostream>
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

// Class to save position of stone in board
class Position {
  public:
    Position() : pos_(0) {}
    Position(int pos) : {
      try {
        if (pos < 0 || pos >= 361) throw pos;
        pos_ = pos;
      } catch(int pos) {
        cout << pos << " is unvalid Position" << endl;
        exit(1);
      }
    }
    Position(const Position& ref) : pos_(ref.pos_) {}
    Position& operator=(const Position& ref) {
      pos_ = ref.pos_;
      return *this;
    }

  private:
    int pos_;
};

// Class for represent MCT_search's state
class State {
  public:
    State() : parent_(NULL), uct_value_(123456789), number_of_wins_(0), number_of_visiting_(0), change_idx_(-1) {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        child_list_[i] = NULL;
      }
      memset(board_, 0, sizeof(board_));
    }
    State(const int* board) : parent_(NULL), uct_value_(123456789), number_of_wins_(0), number_of_visiting_(0), change_idx_(-1){
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        child_list_[i] = NULL;
      }
      for (int i = 0; i < 361; i++) {
        board_[i] = board[i];
      }
    }
    State& SelectionAndExpansion();
    void MakeChildState();
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
    int change_idx_; // Discuss to use
    int board_[361];
};

Position MonteCarloTreeSearch(State& current, double recv_time);

#endif
