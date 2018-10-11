#include "monte_carlo_tree_search.h"
using namespace std;

const char my_color = 2;
const char userColor = 1;
int best_pos1, best_pos2;

extern int board[19][19];

// Main MonteCarloTreeSearch Implemetation
void MonteCarloTreeSearch() {
  // Calculate start time
  chrono::system_clock::time_point start = chrono::system_clock::now();

  char window[361];
  // Copy board
  for (int i = 0; i < 19; i++) {
    int row = 19 * i;
    for (int j = 0; j < 19; j++) {
          window[row + j] = board[i][j];
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
      int empty_list[mct_const::NUMBER_OF_MAX_CHILD_NODES * 2];
      int empty_count = 0;
      int empty_tmp;
      int empty_idx1, empty_idx2, empty_idx3, empty_idx4;
      for (int near1 = 0; near1 < 8; near1++) {
        if(board_[mct_const::NEAR_CENTER1[near1]] == 0) {
          empty_list[empty_count++] = mct_const::NEAR_CENTER1[near1];
        }
      }
      for (int near2 = 0; near2 < 16; near2++) {
        if(board_[mct_const::NEAR_CENTER2[near2]] == 0) {
          empty_list[empty_count++] = mct_const::NEAR_CENTER2[near2];
          if (empty_count == mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) break;
        }
      }
      if (empty_count != mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) {
        for (int near3 = 0; near3 < 24; near3++) {
          if(board_[mct_const::NEAR_CENTER3[near3]] == 0) {
            empty_list[empty_count++] = mct_const::NEAR_CENTER3[near3];
            if (empty_count == mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) break;
          }
        }
      }
      if (empty_count != mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) {
        for (int near4 = 0; near4 < 32; near4++) {
           if(board_[mct_const::NEAR_CENTER4[near4]] == 0) {
             empty_list[empty_count++] = mct_const::NEAR_CENTER4[near4];
             if (empty_count == mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) break;
          }
        }
      }
      while (empty_count < mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) {
        empty_tmp = rand() % 361;
        if (board_[empty_tmp] == 0) {
          empty_list[empty_count++] = empty_tmp;
        }
      }
      for (int iter = 0; iter < mct_const::NUMBER_OF_MAX_CHILD_NODES; iter++) {
        empty_idx1 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        empty_idx2 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        empty_idx3 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        empty_idx4 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);

        while ((empty_list[empty_idx1] == empty_list[empty_idx2]) || (empty_list[empty_idx1] == empty_list[empty_idx3]) || (empty_list[empty_idx1] == empty_list[empty_idx4]) || \
               (empty_list[empty_idx2] == empty_list[empty_idx3]) || (empty_list[empty_idx2] == empty_list[empty_idx4]) || (empty_list[empty_idx3] == empty_list[empty_idx4])) {
          empty_idx1 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
          empty_idx2 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
          empty_idx3 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
          empty_idx4 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        }
        // Make with my color
        best_child->MakeChildState(iter, empty_list[empty_idx1], empty_list[empty_idx2], empty_list[empty_idx3], empty_list[empty_idx4], my_color);
        }
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

void State::MakeChildState(const int child_idx, const int idx_1, const int idx_2, const int opp_idx1, const int opp_idx2, const char color) {
  child_list_[child_idx] = new State(*this);
  child_list_[child_idx]->parent_ = this;

  // Set with my_color
  child_list_[child_idx]->board_[idx_1] = color;
  child_list_[child_idx]->board_[idx_2] = color;
  child_list_[child_idx]->board_[opp_idx1] = (color & 1) + 1;
  child_list_[child_idx]->board_[opp_idx2] = (color & 1) + 1;

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
    for (int turn = 0; turn < 2; turn++) {
      int empty_list[mct_const::NUMBER_OF_MAX_CHILD_NODES * 2];
      int empty_count = 0;
      int empty_idx1, empty_idx2;
      int empty_tmp;
      int is_end;
      for (int near1 = 0; near1 < 8; near1++) {
        if(virtual_board[mct_const::NEAR_CENTER1[near1]] == 0) {
          empty_list[empty_count++] = mct_const::NEAR_CENTER1[near1];
        }
      }
      for (int near2 = 0; near2 < 16; near2++) {
        if(virtual_board[mct_const::NEAR_CENTER2[near2]] == 0) {
          empty_list[empty_count++] = mct_const::NEAR_CENTER2[near2];
          if (empty_count == mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) break;
        }
      }
      if (empty_count != mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) {
        for (int near3 = 0; near3 < 24; near3++) {
          if(virtual_board[mct_const::NEAR_CENTER3[near3]] == 0) {
            empty_list[empty_count++] = mct_const::NEAR_CENTER3[near3];
            if (empty_count == mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) break;
          }
        }
      }
      if (empty_count != mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) {
        for (int near4 = 0; near4 < 32; near4++) {
          if(virtual_board[mct_const::NEAR_CENTER4[near4]] == 0) {
            empty_list[empty_count++] = mct_const::NEAR_CENTER4[near4];
            if (empty_count == mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) break;
          }
        }
      }
      while (empty_count < mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) {
        empty_tmp = rand() % 361;
        if (virtual_board[empty_tmp] == 0) {
          empty_list[empty_count++] = empty_tmp;
        }
      }

      if (turn == 0) {
        // Set my stone
        empty_idx1 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        empty_idx2 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        while (empty_list[empty_idx1] == empty_list[empty_idx2]) {
          empty_idx2 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        }

        virtual_board[empty_list[empty_idx1]] = my_color;
        // Get Result
        is_end = IsEnd(virtual_board, empty_list[empty_idx1], my_color);
        if (is_end == 1) {
          win_count++;
          return;
        } else if (is_end == -1) {
          return;
        }

        virtual_board[empty_list[empty_idx2]] = my_color;
        is_end = IsEnd(virtual_board, empty_list[empty_idx2], my_color);
        if (is_end == 1) {
          win_count++;
          return;
        } else if (is_end == -1) {
          return;
        }
      } else if (turn == 1) {
        // Set opponent stone
        empty_idx1 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        empty_idx2 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        while (empty_list[empty_idx1] == empty_list[empty_idx2]) {
          empty_idx2 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
        }

        virtual_board[empty_list[empty_idx1]] = userColor;
        // Get Result
        is_end = IsEnd(virtual_board, empty_list[empty_idx1], userColor);
        if (is_end == 1) {
          return;
        } else if (is_end == -1) {
          win_count++;
          return;
        }

        virtual_board[empty_list[empty_idx2]] = userColor;

        is_end = IsEnd(virtual_board, empty_list[empty_idx2], userColor);
        if (is_end == 1) {
          return;
        } else if (is_end == -1) {
          win_count++;
          return;
        }
      }
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
