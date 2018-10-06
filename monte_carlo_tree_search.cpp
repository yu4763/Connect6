#include "monte_carlo_tree_search.h"
using namespace std;

//
extern board window;

//
// TO DO List
//
// 1. SelectionAndExpansion Implementation V \ MakeChildState Implementation
// 2. Evaluation random index Selection
// 3. VirtualPlay Implementation
// 4. Calculate uct_value
// 5. Determine Bestchoice strategy
// 7. Get Color when start
//

// Main MonteCarloTreeSearch Implemetation
Position MonteCarloTreeSearch(State& current) {
  // Calculate start time
  chrono::system_clock::time_point start = chrono::system_clock::now();

  // Modify received time to return safely
  double time = mct_const::TIME - mct_const::TERIMINATE_TIME_PADDING;
  Position best_pos;

  // Play game virtually during received time
  while ((chrono::duration<double>(chrono::system_clock::now() - start).count() < time) {
    State& best_child = current.SelectionAndExpansion();
    best_child.Update(best_child.Evaluation());
    best_pos = current.BestChoice();
  }

  return best_pos.GetPostion();
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
      if (best_chlid->child_list[child_idx] == NULL) {
        // ================================
        // For arbitrary child construction
        //
        int empty_list[mct_const::NUMBER_OF_MAX_CHILD_NODES * 2];
        int empty_count = 0;
        int empty_idx = 0;
        while (1) {
          if (empty_idx == 361) {
            cout << "There is no space to play" << endl;
            exit(1);
          }
          if (board_[idx] == 0) {
            empty_list[empty_count++] = empty_idx;
            if (empty_count == mct_const::NUMBER_OF_MAX_CHILD_NODES * 2) break;
          }
          empty_idx++;
        }
        // ================================
        for (int born = 0; born < mct_const::NUMBER_OF_MAX_CHILD_NODES; born++) {
          //
          // Make child state with our strategy MakeChildState();
          // Save change index
          //

          // Make with black
          best_child->MakeChildState(born, empty_list[2 * born], empty_list[2 * born + 1], 1);
        }
        return *(best_child->child_list_[child_idx]);

      // Case2) There is unvisited node
      } else if (best_child->child_list_[child_idx]->number_of_visiting_ == 0) {
        return *(best_child->child_list_[child_idx]);

      // Case3) There is node who has larger uct_value_ than max
      } else if (best_child->child_list_[child_idx]->uct_valuee_ > max_uct_value) {
        max_idx = child_idx;
        max_uct_value = best_child->child_list_[child_idx]->uct_value_;
      }
    }

    // Update Best child
    best_child = best_child->child_list_[child_idx];
  }
}

void State::MakeChildState(const int child_idx, const int idx_1, const int idx_2, const char color) {
  child_list_[child_idx] = new State(*this);
  child_list_[child_idx]->parent = this;

  // Set with my_color
  child_list_[child_idx]->board_[idx_1] = color;
  child_list_[child_idx]->board_[idx_2] = color;

  child_list_[chlid_idx]->change_idx_1 = idx_1;
  child_list_[child_idx]->change_idx_2 = idx_2;
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
  // Copty board to virtual board
  for (int i = 0; i < 361; i++) {
    virutual_board[i] = board_[i];
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
    int empty_list[4];
    int empty_count = 0;
    int empty_idx = 0;
    while (1) {
      if (empty_idx == 361) {
        cout << "There is no space to play" << endl;
        exit(1);
      }
      if (virtual_board[empty_idx] == 0) {
        empty_list[empty_count++] = empty_idx;
        if (empty_count == 4) break;
      }
      empty_idx++;
     }
    // ==========================
    for (int i = 0; i < 2; i++) {
      virutal_board[empty_list[i]] = mct_const::BLACK;
      virtual_board[empty_list[i+2]] = mct_const::WHITE;
    }
  }
  // Check whether win this game
  win_count += IsWin(virtual_board);
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
    current_state->number_of_wins += result;
    current_state->numver_of_visiting++;

    // Base state do not need to update uct value
    if (parent_ != NULL) {
      current_state->uct_value_ += 2 * log(parent_->number_of_visiting_ + 1);
      current_state->uct_value_ /= current_state->number_of_visiting_;
      current_state->uct_value_ = sqrt(current_state->uct_value_);
      current_state->uct_value_ += current_state->number_of_wins_ / current_state->number_of_visiting_;
    }
    current_state = current_state->parent;
  }
}

// 5. Select Best
// Option1. The most visited
// Option2. The highest uct_value
// etc.
Position State::BestChoice() {
  int max_idx = 0;
  int max_visited = child_list_[0]->number_of_visiting_;
  for (int i = 1; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
    if (child_list_[i]->number_of_visiting_ > max_visited) {
      max_visited = child_list_[i]->number_of_visiting_;
      max_idx = i;
    }
  }

  Position ret_pos = Position(child_list_[max_idx]->change_idx_);
  return ret_pos;
}

int IsWin(const char* board) {

  // ===========================
  // For arbitrary win decision
  //
  if (rand() & 1) {
    return 0;
  } else {
    return 1;
  }
}
