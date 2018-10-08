#include "monte_carlo_tree_search.h"
using namespace std;

// For Debug
// CS: search chances
// BD: Board draw
// CV: Check value
//

char my_color = (userColor & 1) + 1;
int best_pos1, best_pos2;

// Main MonteCarloTreeSearch Implemetation
void MonteCarloTreeSearch() {
  // Get Board and set current state
  State current = State(*window);

  // Calculate start time
  chrono::system_clock::time_point start = chrono::system_clock::now();

  // For random functions below
  srand((unsigned int)time(NULL));

  // Modify received time to return safely
  double rcv_time = mct_const::TIME - mct_const::TERMINATE_TIME_PADDING;

  // CS: Check how many search chances
  int search_chance = 0;

  // Play game virtually during received time
  while ((chrono::duration<double>(chrono::system_clock::now() - start)).count() < rcv_time) {
    State& best_child = current.SelectionAndExpansion();
    best_child.Update(best_child.Evaluation());
    current.BestChoice();

    // CS
    search_chance++;
  }

  // CS
  cout << "We have " << search_chance << " times search. \n";
}

// Recursive version of Selection and Expansion
// State& State::SelectionAndExpansion() {
//   // If there is no child state, then expand and return its child.
//   // If all children are visited, then go to best child.
//   int max_idx;
//   double max_uct_value = 0;
//   for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
//     if (child_list_[i] == NULL) {
//       // For Arbitrary child construction
//       // ==========================================
//       int cidx_list[10];
//       int cidx_num = 0;
//       int idx = 0;
//       while (1) {
//         if (cidx_num == 10) break;
//         if (idx == 361) {
//           cout << "There is no space to play" << endl;
//           exit(1);
//         }
//         if (board_[idx] == 0) {
//           cidx_list[cidx_num++] = idx;
//         }
//         idx++;
//       }
//       // =========================================
//       for (int j = 0; j < mct_const::NUMBER_OF_MAX_CHILD_NODES; j++) {
//         //
//         // Make child state with our strategy MakeChildState();
//         // Save change index
//         //
//
//         // Make with black
//         MakeChildState(j, cidx_list[2 * j], cidx2[2 * j + 1], 1);
//       }
//       return *child_list_[i];
//     }
//     // For non visited node
//     if (child_list_[i]->number_of_visiting_ == 0) return *child_list_[i];
//
//     // Compare uct_values to choose best child
//     if (child_list_[i]->uct_value_ > max_uct_value) {
//       max_idx = i;
//       max_uct_value = child_list_[i]->uct_value_;
//     }
//   }
//
//   return child_list_[max_idx]->SelectionAndExpansion();
// }

// Iterative version of Selection and Expansion
State& State::SelectionAndExpansion() {
  State* best_child = this;
  while (1) {
    int max_idx = 0;
    double max_uct_value = 0;
    for (int child_idx = 0; child_idx < mct_const::NUMBER_OF_MAX_CHILD_NODES; child_idx++) {
      // Case1) No child
      if (best_child->child_list_[child_idx] == NULL) {
        // ================================
        // For arbitrary child construction
        //
        int empty_list[mct_const::NUMBER_OF_MAX_CHILD_NODES * 2];
        int empty_count = 0;
        int empty_tmp;
        int empty_idx1, empty_idx2, empty_idx3, empty_idx4;
        // while (1) {
        //   empty_idx = rand() % 361;
        //   if (board_[empty_idx] == 0) {
        //     empty_list[empty_count++] = empty_idx;
        //     if (empty_count == mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) break;
        //   }
        // }
        // ================================
        // Strong random
        //
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
          //
          // Make child state with our strategy MakeChildState();
          // Save change index
          //
          empty_idx1 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
          empty_idx2 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
          empty_idx3 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
          empty_idx4 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);

          while ((empty_idx1 == empty_idx2) || (empty_idx1 == empty_idx3) || (empty_idx1 == empty_idx4) || (empty_idx2 == empty_idx3) || (empty_idx2 == empty_idx4) || (empty_idx3 == empty_idx4)) {
            empty_idx1 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
            empty_idx2 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
            empty_idx3 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
            empty_idx4 = rand() % (mct_const::NUMBER_OF_MAX_CHILD_NODES * 2);
          }
          // Make with my color
          best_child->MakeChildState(iter, empty_list[empty_idx1], empty_list[empty_idx2], empty_list[empty_idx3], empty_list[empty_idx4], my_color);
        }
        return *(best_child->child_list_[child_idx]);

      // Case2) There is unvisited node
      } else if (best_child->child_list_[child_idx]->number_of_visiting_ == 0) {
        return *(best_child->child_list_[child_idx]);

      // Case3) There is node who has larger uct_value_ than max
      } else if (best_child->child_list_[child_idx]->uct_value_ > max_uct_value) {
        max_idx = child_idx;
        max_uct_value = best_child->child_list_[child_idx]->uct_value_;
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

// With Thread
// int State::Evaluation() {
//   int win_count = 0;
//   thread** thread_list = new thread*[mct_const::NUMBER_OF_THREADS];
//
//   for (int i = 0; i < mct_const::NUMBER_OF_THREADS; i++) {
//     thread_list[i] = new thread(VirtualPlay, win_count);
//   }
//
//   for (int i = 0; i < mct_const::NUMBER_OF_THREADS; i++) {
//     thread_list[i]->join();
//   }
//   delete[] thread_list;
//
//   return win_count;
// }

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
    //
    // 3. strategy
    //

    // ==========================
    // For arbitrary game playing
    // Arbitrary game playing is implemented by passing both user's play in each round
    //
    // while (1) {
    //   empty_idx = rand() % 361;
    //   if (virtual_board[empty_idx] == 0) {
    //   empty_list[empty_count++] = empty_idx;
    //     if (empty_count == 4) break;
    //   }
    // }
    // ==========================
    // Strong random
    //
    for (int turn = 0; turn < 2; turn++) {
      int empty_list[mct_const::NUMBER_OF_MAX_CHILD_NODES * 2];
      int empty_count = 0;
      int empty_idx1, empty_idx2;
      int empty_tmp;
      bool is_end;
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
        // BD 16
        // for (int i = 0; i < 19; i++) {
        //  for (int j = 0; j < 19; j++) {
        //    if (virtual_board[19 * i + j] == 0) {
        //      cout << "|   ";
        //    } else if (virtual_board[19 * i + j] == 1) {
        //      cout << "| O ";
        //    } else if (virtual_board[19 * i + j] == 2) {
        //      cout << "| X ";
        //    }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << empty_list[empty_idx1] << " " << empty_list[empty_idx2] << endl;
        // cout << "\n\n" << endl;
        // =================================

        // Get Result
        is_end = IsEnd(virtual_board, empty_list[empty_idx1], my_color);
        if (is_end) {
          return;
        }

        virtual_board[empty_list[empty_idx2]] = my_color;

        is_end = IsEnd(virtual_board, empty_list[empty_idx2], my_color);
        if (is_end) {
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
        if (is_end) {
          win_count++;
          return;
        }

        virtual_board[empty_list[empty_idx2]] = userColor;

        is_end = IsEnd(virtual_board, empty_list[empty_idx2], userColor);
        if (is_end) {
          win_count++;
          return;
        }
      }
    }
  }
  // Check whether win this game
  // win_count += IsWin(virtual_board);

}

// With thread
// void State::Update(int result) {
//   number_of_wins_ += result;
//   number_of_visiting_ += mct_const::NUMBER_OF_THREADS;
//
//   // Base state do not need to update uct value
//   if (parent_ != NULL) {
//     uct_value_ += 2 * log(parent_->number_of_visiting_ + mct_const::NUMBER_OF_THREADS);
//     uct_value_ /= number_of_visiting_;
//     uct_value_ = sqrt(uct_value_);
//     uct_value_ += number_of_wins_ / number_of_visiting_;
//   }
//   parent_->Update();
// }

// Iterative Version of Update without threads
void State::Update(int result) {
  State* current_state = this;
  while (1) {
    if (current_state == NULL) break;
    current_state->number_of_wins_ += result;
    current_state->number_of_visiting_++;

    // Base state do not need to update uct value
    if (parent_ != NULL) {
      current_state->uct_value_ = 0;
      current_state->uct_value_ += 2 * log(parent_->number_of_visiting_ + 1);
      current_state->uct_value_ /= current_state->number_of_visiting_;
      current_state->uct_value_ = 1 * sqrt(current_state->uct_value_);
      current_state->uct_value_ += current_state->number_of_wins_ / current_state->number_of_visiting_;
    }
    current_state = current_state->parent_;
  }
}

// 5. Select Best
// Option1. The most visited
// Option2. The highest uct_value
// etc.
void State::BestChoice() {
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
  // CV
  // cout << best_pos1 << ", " << best_pos2 << ": " << child_list_[max_idx]->number_of_visiting_ << " " << child_list_[max_idx]->number_of_wins_ << " " << child_list_[max_idx]->uct_value_ << endl;
}

bool IsEnd(const char* _board, const int pos, const char color) {

  // ===========================
  // For arbitrary win decision
  //
  // if (rand() & 1) {
  //   return 0;
  // } else {
  //   return 1;
  // }
  // ===========================

  // ===========================
  // Detect end condition
  // Detection operates through finding successive color

  char board[19][19];
  int xpos = pos % 19;
  int ypos = pos / 19;
  int test_x, test_y;
  int row;
  // Copy board_
  for (int i = 0; i < 19; i++) {
      row = 19 * i;
    for (int j = 0; j < 19; j++) {
      board[i][j] = _board[row + j];
    }
  }

  // Horizontal
  // Horizontal Left
  if (((test_x = xpos - 3) >= 0) && (board[ypos][test_x] == color)) {
    if ((board[ypos][test_x + 1] == color) && (board[ypos][test_x + 2] == color)) {
      if (((test_x - 2) >= 0) && (board[ypos][test_x - 2] == color) && (board[ypos][test_x - 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "horizontal1 " << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      } else if (((xpos + 2) < 19) && (board[ypos][xpos + 2] == color) && (board[ypos][xpos + 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "horizontal2 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      }
    }
  // Horizontal Right
  } else if (((test_x = xpos + 3) < 19) && (board[ypos][test_x] == color)) {
    if ((board[ypos][test_x - 1] == color) && (board[ypos][test_x - 2] == color)) {
      if (((test_x + 2) < 19) && (board[ypos][test_x + 2] == color) && (board[ypos][test_x + 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "horizontal3 " << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      } else if (((xpos - 2) >= 0) && (board[ypos][xpos - 2] == color) && (board[ypos][xpos - 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "horizontal4 " << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      }
    }
  }

  // Vertical
  // Vertical Up
  if (((test_y = ypos - 3) >= 0) && (board[test_y][xpos] == color)) {
    if ((board[test_y + 1][xpos] == color) && (board[test_y + 2][xpos] == color)) {
      if (((test_y - 2) >= 0) && (board[test_y - 2][xpos] == color) && (board[test_y - 1][xpos] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "vertical1 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      } else if (((ypos + 2) < 19) && (board[ypos + 2][xpos] == color) && (board[ypos + 1][xpos] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "vertical2 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      }
    }
  // Vertical Down
  } else if (((test_y = ypos + 3) < 19) && (board[test_y][xpos] == color)) {
    if ((board[test_y - 1][xpos] == color) && (board[test_y - 2][xpos] == color)) {
      if (((test_y + 2) < 19) && (board[test_y + 2][xpos] == color) && (board[test_y + 1][xpos] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "vertical3 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      } else if (((ypos - 2) >= 0) && (board[ypos - 2][xpos] == color) && (board[ypos - 1][xpos] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "vertical4 "  << ypos << ", " << xpos << "  " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      }
    }
  }

  // Diagonal Left UP to Right Down
  // Diagonal LU
  if (((test_x = xpos - 3) >= 0) && ((test_y = ypos - 3) >= 0) && (board[test_y][test_x] == color)) {
    if ((board[test_y + 1][test_x + 1] == color) && (board[test_y + 2][test_x + 2] == color)) {
      if (((test_x - 2) >= 0) && ((test_y - 2) >= 0) && (board[test_y - 2][test_x - 2] == color) && (board[test_y - 1][test_x - 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "Diagonal LU2RD 1 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      } else if (((xpos + 2) < 19) && ((ypos + 2) < 19) && (board[ypos + 2][xpos + 2] == color) && (board[ypos + 1][xpos + 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "Diagonal LU2RD 2 "  << ypos << ", " << xpos << "  " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      }
    }
  // Diagonal RD
  } else if (((test_x = xpos + 3) < 19) && ((test_y = ypos + 3) < 19) && (board[test_y][test_x] == color)) {
    if ((board[test_y - 1][test_x - 1] == color) && (board[test_y - 2][test_x - 2] == color)) {
      if (((test_x + 2) < 19) && ((test_y + 2) < 19) && (board[test_y + 2][test_x + 2] == color) && (board[test_y + 1][test_x + 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "Diagonal LU2RD 3 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      } else if (((xpos - 2) >= 0) && ((ypos - 2) >= 0) && (board[ypos - 2][xpos - 2] == color) && (board[ypos - 1][xpos - 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "Diagonal LU2RD 4"  << xpos << " " << ypos << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      }
    }
  }

  // Diagonal Left Down to Right Up
  // Diagonal LD
  if (((test_x = xpos - 3) >= 0) && ((test_y = ypos + 3) < 19) && (board[test_y][test_x] == color)) {
    if ((board[test_y - 1][test_x + 1] == color) && (board[test_y - 2][test_x + 2] == color)) {
      if (((test_x - 2) >= 0) && ((test_y + 2) < 19) && (board[test_y + 2][test_x - 2] == color) && (board[test_y + 1][test_x - 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "Diagonal LD2RU 1 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      } else if (((xpos + 2) < 19) && ((ypos - 2) >=0) && (board[ypos - 2][xpos + 2] == color) && (board[ypos - 1][xpos + 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "Diagonal LD2RU 2 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      }
    }
  // Diagonal RU
  } else if (((test_x = xpos + 3) < 19) && ((test_y = ypos - 3) >= 0) && (board[test_y][test_x] == color)) {
    if ((board[test_y + 1][test_x - 1] == color) && (board[test_y + 2][test_x - 2] == color)) {
      if (((test_x + 2) < 19) && ((test_y - 2) >= 0) && (board[test_y - 2][test_x + 2] == color) && (board[test_y - 1][test_x + 1] == color)) {
        // // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "Diagonal LD2RU 3 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      } else if (((xpos - 2) >= 0) && ((ypos + 2) < 19) && (board[ypos + 2][xpos - 2] == color) && (board[ypos + 1][xpos - 1] == color)) {
        // BD 16
        // for (int i = 0; i < 19; i++) {
        //   for (int j = 0; j < 19; j++) {
        //     if (board[i][j] == 0) {
        //       cout << "|   ";
        //     } else if (board[i][j] == 1) {
        //       cout << "| O ";
        //     } else if (board[i][j] == 2) {
        //       cout << "| X ";
        //     }
        //   }
        //   cout << "|\n" << endl;
        // }
        // cout << "Diagonal LD2RU 4 "  << ypos << ", " << xpos << " " << color << endl;
        // cout << "\n\n" << endl;
        // // =================================
        return true;
      }
    }
  }
  return false;
}
