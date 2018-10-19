#ifndef __MONTE_CARLO_TREE_SEARCH_H__
#define __MONTE_CARLO_TREE_SEARCH_H__

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <random>
using namespace std;

// Namespace for save constants of monte carlo tree search
namespace mct_const {
  const double TIME = 5;
  const double TERMINATE_TIME_PADDING = 1;
  const int NUMBER_OF_MAX_CHILD_NODES = 20;
  const int NUMBER_OF_ROUNDS = 40;
  const int NEAR_CENTER1[8] = {
    160, 161, 162,
    179,      181,
    198, 199, 200
  };
  const int NEAR_CENTER2[16] = {
    140, 141, 142, 143, 144,
    159,                163,
    178,                182,
    197,                201,
    216, 217, 218, 219, 220
  };
  const int NEAR_CENTER3[24] = {
    120, 121, 122, 123, 124, 125, 126,
    139,                          145,
    158,                          164,
    177,                          183,
    196,                          202,
    215,                          221,
    234, 235, 236, 237, 238, 239, 240
  };
  const int NEAR_CENTER4[32] = {
    100, 101, 102, 103, 104, 105, 106, 107, 108,
    119,                                    127,
    138,                                    146,
    157,                                    165,
    176,                                    184,
    195,                                    203,
    214,                                    222,
    233,                                    241,
    252, 253, 254, 255, 256, 257, 258, 259, 260
  };
}

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
    ~State() {
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        if (child_list_[i]) {
          delete child_list_[i];
        }
      }
    }
    State& SelectionAndExpansion();
    void MakeChildState(const int child_idx, const int idx_1, const int idx_2, const int opp_idx1, const int opp_idx2, const char color);
    int Evaluation();
    void VirtualPlay(int& win_count);
    void Update(int result);
    State* BestChoice();

  private:
    State* parent_;
    State* child_list_[mct_const::NUMBER_OF_MAX_CHILD_NODES];
    double uct_value_;
    int number_of_wins_;
    int number_of_visiting_;
    int change_idx_1_;
    int change_idx_2_;
    char board_[361];
    // For Debug
    friend void MonteCarloTreeSearch();
};

void MonteCarloTreeSearch();
int IsEnd(const char* _board, const int pos, const char color);

#endif
