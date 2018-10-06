#ifndef __MONTE_CARLO_TREE_SEARCH_H__
#define __MONTE_CARLO_TREE_SEARCH_H__

#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include "board.h"
using namespace std;

// Namespace for save constants of monte carlo tree search
namespace mct_const {
  const double TIME = 5;
  const double TERMINATE_TIME_PADDING = 0.01;
  const int NUMBER_OF_THREADS = 4;
  const int NUMBER_OF_MAX_CHILD_NODES = 12;
  const int NUMBER_OF_ROUNDS = 16;
  const char BLACK = 1;
  const char WHITE = 2;
};

// Class to save position of stone in board
class Position {
  public:
    Position() : pos_(0) {}
    Position(int pos) {
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
    int GetPosition() { return pos_; }
  
  private:
    int pos_;
};

// Class for represent MCT_search's state
class State {
  public:
    State() : parent_(NULL), uct_value_(123456789), number_of_wins_(0), number_of_visiting_(0), change_idx_1_(-1), change_idx_2_(-1) {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        child_list_[i] = NULL;
      }
      memset(board_, 0, sizeof(board_));
    }
    State(const State& ref)
      : parent_(NULL), uct_value_(123456789), number_of_wins_(0), number_of_visiting_(0), change_idx_1_(-1), change_idx_2_(-1) {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        child_list_[i] = NULL;
      }
      for (int i = 0; i <361; i++) {
        board_[i] = ref.board_[i];
      }
    }
    State(const char* board) : parent_(NULL), uct_value_(123456789), number_of_wins_(0), number_of_visiting_(0), change_idx_1_(-1), change_idx_2_(-1) {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        child_list_[i] = NULL;
      }
      for (int i = 0; i < 361; i++) {
        board_[i] = board[i];
      }
    }
        
    // Construction using board
    State(const board& board_ref) 
      : parent_(NULL), uct_value_(123456789), number_of_wins_(0), number_of_visiting_(0), change_idx_1_(-1), change_idx_2_(-1) {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        child_list_[i] = NULL;
      }
      for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
          board_[19 * i + j] = board_ref.stones[i][j]->state;
        }
      }
    }
    
    // If it is needed, then implement constructor using stones' chage index
    State(const int idx1, const int idx2);

    ~State() {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        if (child_list_[i]) {
          delete child_list_[i];
        }
      }
    }
    State& SelectionAndExpansion();
    void MakeChildState(const int child_idx, const int idx_1, const int idx_2, const char color);
    int Evaluation();
    void VirtualPlay(int& win_count);
    void Update(int result);
    void BestChoice();

  private:
    State* parent_;
    State* child_list_[mct_const::NUMBER_OF_MAX_CHILD_NODES];
    double uct_value_;
    int number_of_wins_;
    int number_of_visiting_;
    int change_idx_1_;
    int change_idx_2_;
    char board_[361];
};

void MonteCarloTreeSearch();
int IsWin(const char* board);
#endif
