#include "monte_carlo_tree_search_deep.h"
using namespace std;

const char my_color = -1;
const char userColor = 1;
int best_pos1, best_pos2;

extern int board[19][19];

// ====================================================================
// ************************** < Deep Learning > ***********************
//                               * Change *
//                             state => bstate
//                               UCT => score
//                        CalDeepUCT => score_board
//
int input_size = 11;
int hidden_size = 10;
int output_size = 1;
int batch = 4;

// char bstate[361];
// float score[361] = {0};

Network score_board(batch, input_size, hidden_size, output_size);
float W1[110] = {0.4861,0.7128,-0.2039,-0.2234,0.5542,0.4818,1.1016,0.5619,0.1123,-0.2307,0.6434,1.0494,-0.3328,-0.7888,0.7962,0.7770,1.6928,-0.1019,-0.4037,-0.5478,0.6584,12.9292,-0.0486,-1.1102,1.5440,0.4683,-3.6548,0.6664,-0.7352,-1.1583,19.0214,-6.4903,2.0214,-0.8613,3.0790,0.5439,\
  -4.2066,0.7813,-1.4816,-1.4619,-13.6157,-2.0031,1.6530,-0.2923,3.3260,0.7729,4.6276,1.5423,-1.9160,-2.0843,0.5079,-1.1777,0.1840,-0.4112,1.3126,0.3635,-0.8112,-0.4158,0.4746,-1.1001,2.8636,1.7081,-11.3290,1.1701,18.5662,-2.0117,0.5348,-1.6124,-0.1008,-2.4072,2.5549,1.1915,-1.4798,1.3488,\
  -15.6656,3.0098,-0.4666,-2.3336,0.3723,-1.7225,1.6522,0.6421,11.4877,0.8859,1.9072,-0.1442,-1.7397,4.7702,0.5818,-1.1990,1.3050,0.0884,3.3730,0.5116,1.2800,0.4814,0.4004,0.9201,0.5790,-0.6684,0.4874,0.7251,0.1224,-0.0620,0.7607,0.2064,1.9619,0.5380,0.2100,-0.2411};
float b1[40] = {10.6849,6.0581,-3.7608,-2.8541,9.4082,0.9127,0.9896,2.0188,2.9215,-3.2534,10.6849,6.0581,-3.7608,-2.8541,9.4082,0.9127,0.9896,2.0188,2.9215,-3.2534,10.6849,6.0581,-3.7608,-2.8541,9.4082,0.9127,0.9896,2.0188,2.9215,-3.2534,10.6849,6.0581,-3.7608,-2.8541,9.4082,0.9127,0.9896,\
  2.0188,2.9215,-3.2534};
float W2[10] = {10.6010,8.9444,-3.2706,37.4749,9.7365,14.5639,6.8975,9.1706,-33.1780,42.8233};
float b2[4] = {-3.9451,-3.9451,-3.9451,-3.9451};
// ====================================================================

// Main MonteCarloTreeSearch Implemetation
void MonteCarloTreeSearch() {
  // Calculate start time
  chrono::system_clock::time_point start = chrono::system_clock::now();

  char window[361];
  // Copy board
  for (int i = 0; i < 19; i++) {
    int row = 19 * i;
    for (int j = 0; j < 19; j++) {
      if (board[i][j] == 2) {
        window[row + j] = -1;
      } else {
        window[row + j] = board[i][j];
      }
    }
  }
  // Get Board and set current state
  State current = State(window);

  // For random functions below
  srand((unsigned int)time(NULL));

  // Modify received time to return safely
  double rcv_time = mct_const::TIME - mct_const::TERMINATE_TIME_PADDING;

  // int search_chance = 0;
  State* best_choice;

  // Play game virtually during received time
  while ((chrono::duration<double>(chrono::system_clock::now() - start)).count() < rcv_time) {
    State& best_child = current.SelectionAndExpansion();
    best_child.Update(best_child.Evaluation());
    best_choice = current.BestChoice();
  }
}

// Iterative version of Selection and Expansion
State& State::SelectionAndExpansion() {
  State* best_child = this;
  State* child_state;
  while (1) {
    int max_idx = 0;
    double max_uct_value = 0;
    // Case1) No child
    if (best_child->child_list_[0] == NULL) {
      // =================================================================================
      // < Deep Learning >
      // For get one pair of indexes at once
      int index1[mct_const::NUMBER_OF_BEST_POS];
      int index2[mct_const::NUMBER_OF_BEST_POS];
      int sq_pos = pow(mct_const::NUMBER_OF_BEST_POS, 2);
      int counter = 0;
      // Set my stones
      GetBestIndex(best_child->board_, index1, W1, b1, W2, b2, score_board, mct_const::NUMBER_OF_BEST_POS);
      for (int i = 0; i < mct_const::NUMBER_OF_BEST_POS; i++) {
        for (int j = 0; j < mct_const::NUMBER_OF_BEST_POS; j++) {
          best_child->MakeChildState(mct_const::NUMBER_OF_BEST_POS * i + j, index1[i], index1[i], my_color);
        }
      }
      for (int i = 0; i < mct_const::NUMBER_OF_BEST_POS; i++) {
        GetBestIndex(best_child->child_list_[mct_const::NUMBER_OF_BEST_POS * i]->board_, index2, W1, b1, W2, b2, score_board, mct_const::NUMBER_OF_BEST_POS);
        for (int j = 0; j < mct_const::NUMBER_OF_BEST_POS; j++) {
          best_child->child_list_[mct_const::NUMBER_OF_BEST_POS * i + j]->board_[index2[j]] = my_color;
          best_child->child_list_[mct_const::NUMBER_OF_BEST_POS * i + j]->change_idx_2_ = index2[j];
        }
      }
      // Set opponent's stones
      for (int i = 0; i < sq_pos; i ++) {
        GetBestIndex(best_child->child_list_[i]->board_, index1, W1, b1, W2, b2, score_board, 1);
        best_child->child_list_[i]->board_[index1[0]] = userColor;
        GetBestIndex(best_child->child_list_[i]->board_, index2, W1, b1, W2, b2, score_board, 1);
        best_child->child_list_[i]->board_[index2[0]] = userColor;
      }

      // =================================================================================
      return *(best_child->child_list_[0]);
    } else {
      // There are child nodes
      for (int child_idx = 0; child_idx < mct_const::NUMBER_OF_MAX_CHILD_NODES; child_idx++) {
        child_state = best_child->child_list_[child_idx];
        // Case2) There is unvisited node
        if (child_state->number_of_visiting_ == 0) {
          return *(child_state);
        }
        // Update UCT value
        child_state->uct_value_ = 0;
        child_state->uct_value_ += 2 * log(best_child->number_of_visiting_);
        child_state->uct_value_ /= child_state->number_of_visiting_;
        child_state->uct_value_ = 1 * sqrt(child_state->uct_value_);
        child_state->uct_value_ += (double)child_state->number_of_wins_ / child_state->number_of_visiting_;
        // Case3) There is node who has larger uct_value_ than max
        if (child_state->uct_value_ > max_uct_value) {
          max_idx = child_idx;
          max_uct_value = child_state->uct_value_;
        }
      }
      }
    // Update Best child
    best_child = best_child->child_list_[max_idx];
  }
}

void State::MakeChildState(const int child_idx, const int idx_1, const int idx_2, const char color) {
  child_list_[child_idx] = new State(*this);
  child_list_[child_idx]->parent_ = this;

  // Set with my_color
  child_list_[child_idx]->board_[idx_1] = color;
  child_list_[child_idx]->board_[idx_2] = color;

  child_list_[child_idx]->change_idx_1_ = idx_1;
  child_list_[child_idx]->change_idx_2_ = idx_2;
}

// Without Thread
int State::Evaluation() {
  int win_count = 0;

  VirtualPlay(win_count);

  return win_count;
}

void State::VirtualPlay(int& win_count) {
  char virtual_board[361];
  // Copy board to virtual board
  for (int i = 0; i < 361; i++) {
    virtual_board[i] = board_[i];
  }

  // Do Connect6 randomly according to expected strategy
  for (int round = 0; round < mct_const::NUMBER_OF_ROUNDS; round++) {
    // < Deep Learning >
    int index[mct_const::NUMBER_OF_BEST_POS];
    int select_idx;
    int is_end;
    // Set my stones
    // Set my first stone
    GetBestIndex(virtual_board, index, W1, b1, W2, b2, score_board, mct_const::NUMBER_OF_BEST_POS);
    select_idx = rand() % mct_const::NUMBER_OF_BEST_POS;
    virtual_board[index[select_idx]] = my_color;
    is_end = IsEnd(virtual_board, index[select_idx], my_color);
    if (is_end == 1) {
      win_count++;
      return;
    } else if (is_end == -1) {
      return;
    }
    GetBestIndex(virtual_board, index, W1, b1, W2, b2, score_board, mct_const::NUMBER_OF_BEST_POS);
    select_idx = rand() % mct_const::NUMBER_OF_BEST_POS;
    virtual_board[index[select_idx]] = my_color;
    is_end = IsEnd(virtual_board, index[select_idx], my_color);
    if (is_end == 1) {
      win_count++;
      return;
    } else if (is_end == -1) {
      return;
    }
    GetBestIndex(virtual_board, index, W1, b1, W2, b2, score_board, mct_const::NUMBER_OF_BEST_POS);
    select_idx = rand() % mct_const::NUMBER_OF_BEST_POS;
    virtual_board[index[select_idx]] = userColor;
    is_end = IsEnd(virtual_board, index[select_idx], userColor);
    if (is_end == 1) {
      return;
    } else if (is_end == -1) {
      win_count++;
      return;
    }
    select_idx = rand() % mct_const::NUMBER_OF_BEST_POS;
    virtual_board[index[select_idx]] = userColor;
    is_end = IsEnd(virtual_board, index[select_idx], userColor);
    if (is_end == 1) {
      return;
    } else if (is_end == -1) {
      win_count++;
      return;
    }
  }
}

// Iterative Version of Update without threads
void State::Update(int result) {
  State* current_state = this;
  while (1) {
    current_state->number_of_wins_ += result;
    current_state->number_of_visiting_++;

    if (current_state->parent_ == NULL) break;
    current_state = current_state->parent_;
  }
}

// Select Best
// The most visited
State* State::BestChoice() {
  int max_idx = 0;
  int max_visited = child_list_[0]->number_of_visiting_;
  for (int i = 1; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
    if (child_list_[i]->number_of_visiting_ > max_visited) {
      max_visited = child_list_[i]->number_of_visiting_;
      max_idx = i;
    }
  }

  best_pos1 = child_list_[max_idx]->change_idx_1_;
  best_pos2 = child_list_[max_idx]->change_idx_2_;
  return child_list_[max_idx];
}

int IsEnd(const char* _board, const int pos, const char color) {
  // Detect end condition
  // Detection operates through finding successive color
  char vboard[19][19];
  int xpos = pos % 19;
  int ypos = pos / 19;
  int test_x, test_y;
  int row;
  // Copy board_
  for (int i = 0; i < 19; i++) {
      row = 19 * i;
    for (int j = 0; j < 19; j++) {
      vboard[i][j] = _board[row + j];
    }
  }

  // Horizontal
  // Horizontal Left
  if (((test_x = xpos - 3) >= 0) && (vboard[ypos][test_x] == color)) {
    if ((vboard[ypos][test_x + 1] == color) && (vboard[ypos][test_x + 2] == color)) {
      if (((test_x - 2) >= 0) && (vboard[ypos][test_x - 2] == color) && (vboard[ypos][test_x - 1] == color)) {
        // Detect more than 6
        if ((((test_x - 3) >= 0) && (vboard[ypos][test_x - 3] == color)) || (((xpos + 1) < 19) && (vboard[ypos][xpos + 1] == color))) {
          return -1;
        }
        return 1;
      } else if (((xpos + 2) < 19) && (vboard[ypos][xpos + 2] == color) && (vboard[ypos][xpos + 1] == color)) {
        // Detect more than 6
        if ((((xpos + 3) < 19) && (vboard[ypos][xpos + 3] == color)) || (((test_x - 1) >= 0) && (vboard[ypos][test_x - 1] == color))) {
          return -1;
        }
        return 1;
      }
    }
  // Horizontal Right
  } else if (((test_x = xpos + 3) < 19) && (vboard[ypos][test_x] == color)) {
    if ((vboard[ypos][test_x - 1] == color) && (vboard[ypos][test_x - 2] == color)) {
      if (((test_x + 2) < 19) && (vboard[ypos][test_x + 2] == color) && (vboard[ypos][test_x + 1] == color)) {
        // Detect more than 6
        if ((((test_x + 3) < 19) && (vboard[ypos][test_x + 3] == color)) || (((xpos - 1) >= 0) && (vboard[ypos][xpos - 1] == color))) {
          return -1;
        }
        return 1;
      } else if (((xpos - 2) >= 0) && (vboard[ypos][xpos - 2] == color) && (vboard[ypos][xpos - 1] == color)) {
        // Detect more than 6
        if ((((xpos - 3) >= 0) && (vboard[ypos][xpos - 3] == color)) || (((test_x + 1) < 19) && (vboard[ypos][test_x + 1] == color))) {
          return -1;
        }
        return 1;
      }
    }
  }

  // Vertical
  // Vertical Up
  if (((test_y = ypos - 3) >= 0) && (vboard[test_y][xpos] == color)) {
    if ((vboard[test_y + 1][xpos] == color) && (vboard[test_y + 2][xpos] == color)) {
      if (((test_y - 2) >= 0) && (vboard[test_y - 2][xpos] == color) && (vboard[test_y - 1][xpos] == color)) {
        // Detect more than 6
        if ((((test_y - 3) >= 0) && (vboard[test_y - 3][xpos] == color)) || (((ypos + 1) < 19) && (vboard[ypos + 1][xpos] == color))) {
          return -1;
        }
        return 1;
      } else if (((ypos + 2) < 19) && (vboard[ypos + 2][xpos] == color) && (vboard[ypos + 1][xpos] == color)) {
        // Detect more than 6
        if ((((ypos + 3) < 19) && (vboard[ypos + 3][xpos] == color)) || (((test_y - 1) >= 0) && (vboard[test_y - 1][xpos] == color))) {
          return -1;
        }
        return 1;
      }
    }
  // Vertical Down
  } else if (((test_y = ypos + 3) < 19) && (vboard[test_y][xpos] == color)) {
    if ((vboard[test_y - 1][xpos] == color) && (vboard[test_y - 2][xpos] == color)) {
      if (((test_y + 2) < 19) && (vboard[test_y + 2][xpos] == color) && (vboard[test_y + 1][xpos] == color)) {
        // Detect more than 6
        if ((((test_y + 3) < 19) && (vboard[test_y + 3][xpos] == color)) || (((ypos - 1) >= 0) && (vboard[ypos - 1][xpos] == color))) {
          return -1;
        }
        return 1;
      } else if (((ypos - 2) >= 0) && (vboard[ypos - 2][xpos] == color) && (vboard[ypos - 1][xpos] == color)) {
        // Detect more than 6
        if ((((ypos - 3) >= 0) && (vboard[ypos - 3][xpos] == color)) || (((test_y + 1) < 19) && (vboard[test_y + 1][xpos] == color))) {
          return -1;
        }
        return 1;
      }
    }
  }

  // Diagonal Left UP to Right Down
  // Diagonal LU
  if (((test_x = xpos - 3) >= 0) && ((test_y = ypos - 3) >= 0) && (vboard[test_y][test_x] == color)) {
    if ((vboard[test_y + 1][test_x + 1] == color) && (vboard[test_y + 2][test_x + 2] == color)) {
      if (((test_x - 2) >= 0) && ((test_y - 2) >= 0) && (vboard[test_y - 2][test_x - 2] == color) && (vboard[test_y - 1][test_x - 1] == color)) {
        // Detect more than 6
        if ((((test_x - 3) >= 0) && ((test_y - 3) >= 0) && (vboard[test_y - 3][test_x - 3] == color)) || (((ypos + 1) < 19) && ((xpos + 1) < 19) && (vboard[ypos + 1][xpos + 1] == color))) {
          return -1;
        }
        return 1;
      } else if (((xpos + 2) < 19) && ((ypos + 2) < 19) && (vboard[ypos + 2][xpos + 2] == color) && (vboard[ypos + 1][xpos + 1] == color)) {
        // Detect more than 6
        if ((((xpos + 3) < 19) && ((ypos + 3) < 19) && (vboard[ypos + 3][xpos + 3] == color)) || (((test_x - 1) >= 0) && ((test_y - 1) >= 0) && (vboard[test_y - 1][test_x - 1] == color))) {
          return -1;
        }
        return 1;
      }
    }
  // Diagonal RD
  } else if (((test_x = xpos + 3) < 19) && ((test_y = ypos + 3) < 19) && (vboard[test_y][test_x] == color)) {
    if ((vboard[test_y - 1][test_x - 1] == color) && (vboard[test_y - 2][test_x - 2] == color)) {
      if (((test_x + 2) < 19) && ((test_y + 2) < 19) && (vboard[test_y + 2][test_x + 2] == color) && (vboard[test_y + 1][test_x + 1] == color)) {
        // Detect more than 6
        if ((((test_x + 3) < 19) && ((test_y + 3) < 19) && (vboard[test_y + 3][test_x + 3] == color)) || (((xpos - 1) >= 0) && ((ypos - 1) >= 0) && (vboard[ypos - 1][xpos - 1] == color))) {
          return -1;
        }
        return 1;
      } else if (((xpos - 2) >= 0) && ((ypos - 2) >= 0) && (vboard[ypos - 2][xpos - 2] == color) && (vboard[ypos - 1][xpos - 1] == color)) {
        // Detect more than 6
        if ((((xpos - 3) >= 0) && ((ypos - 3) >= 0) && (vboard[ypos - 3][xpos - 3] == color)) || (((test_x + 1) < 19) && ((test_y + 1) < 19) && (vboard[test_y + 1][test_x + 1] == color))) {
          return -1;
        }
        return 1;
      }
    }
  }

  // Diagonal Left Down to Right Up
  // Diagonal LD
  if (((test_x = xpos - 3) >= 0) && ((test_y = ypos + 3) < 19) && (vboard[test_y][test_x] == color)) {
    if ((vboard[test_y - 1][test_x + 1] == color) && (vboard[test_y - 2][test_x + 2] == color)) {
      if (((test_x - 2) >= 0) && ((test_y + 2) < 19) && (vboard[test_y + 2][test_x - 2] == color) && (vboard[test_y + 1][test_x - 1] == color)) {
        // Detect more than 6
        if ((((test_x - 3) >= 0) && ((test_y + 3) < 19) && (vboard[test_y + 3][test_x - 3] == color)) || (((xpos + 1) < 19) && ((ypos - 1) >= 0) && (vboard[ypos - 1][xpos + 1] == color))) {
          return -1;
        }
        return 1;
      } else if (((xpos + 2) < 19) && ((ypos - 2) >= 0) && (vboard[ypos - 2][xpos + 2] == color) && (vboard[ypos - 1][xpos + 1] == color)) {
        // Detect more than 6
        if ((((xpos + 3) < 19) && ((ypos - 3) >= 0) && (vboard[ypos - 3][xpos + 3] == color)) || (((test_x - 1) >= 0) && ((test_y + 1) < 19) && (vboard[test_y + 1][test_x - 1] == color))) {
          return -1;
        }
        return 1;
      }
    }
  // Diagonal RU
  } else if (((test_x = xpos + 3) < 19) && ((test_y = ypos - 3) >= 0) && (vboard[test_y][test_x] == color)) {
    if ((vboard[test_y + 1][test_x - 1] == color) && (vboard[test_y + 2][test_x - 2] == color)) {
      if (((test_x + 2) < 19) && ((test_y - 2) >= 0) && (vboard[test_y - 2][test_x + 2] == color) && (vboard[test_y - 1][test_x + 1] == color)) {
        // Detect more than 6
        if ((((test_x + 3) < 19) && ((test_y - 3) >= 0) && (vboard[test_y - 3][test_x + 3] == color)) || (((xpos - 1) >= 0) && ((ypos + 1) < 19) && (vboard[ypos + 1][xpos - 1] == color))) {
          return -1;
        }
        return 1;
      } else if (((xpos - 2) >= 0) && ((ypos + 2) < 19) && (vboard[ypos + 2][xpos - 2] == color) && (vboard[ypos + 1][xpos - 1] == color)) {
        // Detect more than 6
        if ((((xpos - 3) >= 0) && ((ypos + 3) < 19) && (vboard[ypos + 3][xpos - 3] == color)) || (((test_x + 1) < 19) && ((test_y - 1) >= 0) && (vboard[test_y - 1][test_x + 1] == color))) {
          return -1;
        }
        return 1;
      }
    }
  }
  return 0;
}
