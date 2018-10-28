#include "monte_carlo_tree_search_strong.h"
using namespace std;

// For Black
const char my_color = 1;
const char userColor = 2;

// For white
// const char my_color = 2;
// const char userColor = 1;
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
  // best_child: pointer to point best child of state
  State* best_child = this;
  State* child_state;
  while (1) {
    int max_idx = 0;
    double max_uct_value = 0;
    // Case1) No child
    if (best_child->child_list_[0] == NULL) {
      // Arrays to save best picks
      int indexes1[mct_const::FIRST_PICK];
      int indexes2[mct_const::SECOND_PICK];
      int index[1];

      // Get First Position
      GetBestPositions(best_child->board_, indexes1, mct_const::FIRST_PICK, my_color, 1);
      // Set child state using first positions
      // There are (mct_const::SECOND_PICK) child states for each first position
      for (int i = 0; i < mct_const::FIRST_PICK; i++) {
        int iter = mct_const::SECOND_PICK * i;
        for (int j = 0; j < mct_const::SECOND_PICK; j++) {
          best_child->child_list_[iter + j] = new State(*best_child);
          best_child->child_list_[iter + j]->parent_ = best_child;
          best_child->child_list_[iter + j]->board_[indexes1[i]] = my_color;
          best_child->child_list_[iter + j]->change_idx_1_ = indexes1[i];
        }
      }
      for (int i = 0; i < mct_const::FIRST_PICK; i++) {
        int iter = mct_const::SECOND_PICK * i;
        GetBestPositions(best_child->child_list_[iter]->board_, indexes2, mct_const::SECOND_PICK, my_color, 2);
        for (int j = 0; j < mct_const::SECOND_PICK; j++) {
          best_child->child_list_[iter + j]->board_[indexes2[j]] = my_color;
          best_child->child_list_[iter + j]->change_idx_2_ = indexes2[j];
        }
      }
      for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
        GetBestPositions(best_child->child_list_[i]->board_, index, 1, userColor, 1);
        best_child->child_list_[i]->board_[index[0]] = userColor;
        GetBestPositions(best_child->child_list_[i]->board_, index, 1, userColor, 2);
        best_child->child_list_[i]->board_[index[0]] = userColor;
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
        child_state->uct_value_ = mct_const::UCT_CONSTANT * sqrt(child_state->uct_value_);
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

void GetBestPositions(char* board, int* indexes, int num, char color, int turn) {
  // score: Save the value of positions
  // *_connect: Save the number of connected stones before
  // *_block: Save the distance from blocking factors(opponent's stone / wall)
  char opp_color = (color & 1) + 1;
  int score[361] = {0};
  int score_opp[361] = {0};
  int vert_connect[19] = {0};
  int diagLU_connect[27] = {0};
  int diagRU_connect[27] = {0};
  int vert_block[19] = {0};
  int diagLU_block[27] = {0};
  int diagRU_block[27] = {0};

  // Test Score
  // ofstream out1("out1.txt", ios::app);
  // ofstream out2("out2.txt", ios::app);
  // ofstream total("total.txt", ios::app);

  // Get our score
  for (int i = 0; i < 19; i++) {
    int row = 19 * i;
    int hori_connect = 0;
    int hori_block = 0;
    for (int j = 0; j < 19; j++) {
      // Update distance from blockings
      hori_block++;
      vert_block[j]++;
      if ((i - j >= -13) && (i - j <= 13)) {
        diagLU_block[i-j+13]++;
      }
      if ((i + j >= 5) && (i + j <= 31)) {
        diagRU_block[i+j-5]++;
      }
      if (board[row + j] == 0) {
        // tmp_score: Save the number of my stones after Position
        // succ_score: Save the number of succesive my stones after Position
        int tmp_score = 0;
        int succ_score = 0;
        int tmp_block = 7;
        bool is_succ = true;
        bool is_c7 = false;
        bool is_block = false;
        // Get Horizontal score
        // Can make right c6
        if (j <= (19 - (6 - hori_connect))) {
          for (int k = 1; k < 6 - hori_connect; k++) {
            if (board[row + j + k*1] == color) {
              tmp_score++;
            } else if (board[row + j + k*1] == opp_color) {
              tmp_score = 0;
              tmp_block = k;
              break;
            } else {
              if (is_succ) {
                succ_score = tmp_score;
                is_succ = false;
              }
            }
          }
          // All closed
          if (tmp_block + hori_block < 7) {
            is_block = true;
          }
          // Check whether stones pattern can be c7
          // This position makes c7
          if (j <= (19 - (7 - hori_connect))) {
            if (board[row + j + (6 - hori_connect)] == color) {
              is_c7 = true;
            }
          }
          // Rearrange score
          if (!is_c7 && !is_block) {
            if (turn == 1) {
              if (tmp_score + hori_connect == 5) {
                score[row + j] += 4096;
              } else if (tmp_score + hori_connect == 4) {
                score[row + j] += 512;
              } else if (tmp_score + hori_connect == 3) {
                score[row + j] += 40;
              } else if (tmp_score + hori_connect == 2) {
                score[row + j] += 32;
              } else if (tmp_score + hori_connect == 1) {
                score[row + j] += 20;
              } else if (tmp_score + hori_connect == 0) {
                score[row + j] += 2;
              }
            } else {
              if (tmp_score + hori_connect == 5) {
                score[row + j] += 4096;
              } else if (tmp_score + hori_connect == 4) {
                score[row + j] += 0;
              } else if (tmp_score + hori_connect == 3) {
                score[row + j] += 40;
              } else if (tmp_score + hori_connect == 2) {
                score[row + j] += 32;
              } else if (tmp_score + hori_connect == 1) {
                score[row + j] += 20;
              } else if (tmp_score + hori_connect == 0) {
                score[row + j] += 2;
              }
            }
          }

          tmp_score = 0;
          // Can make left c6
          if (hori_block >= (6 - succ_score)) {
            for (int k = 1; k < 6 - succ_score; k++) {
              if (board[row + j - k*1] == color) {
                tmp_score++;
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (hori_block >= (7 - succ_score)) {
              if (board[row + j - (6 - succ_score)] == color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (turn == 1) {
                if (tmp_score + succ_score == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 4) {
                  score[row + j] += 512;
                } else if (tmp_score + succ_score == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + succ_score == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + succ_score == 0) {
                  score[row + j] += 2;
                }
              } else {
                if (tmp_score + succ_score == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 4) {
                  score[row + j] += 0;
                } else if (tmp_score + succ_score == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + succ_score == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + succ_score == 0) {
                  score[row + j] += 2;
                }
              }
            }
          }
        // Can't make right c6
        } else {
          // Can make left c6
          for (int k = 1; k < 19 - j; k++) {
            if (board[row + j + k*1] == color) {
              tmp_score++;
            } else if (board[row + j + k*1] == opp_color) {
              tmp_score = 0;
              break;
            } else {
              if (is_succ) {
                succ_score = tmp_score;
                is_succ = false;
              }
            }
          }
          if (is_succ) {
            succ_score = tmp_score;
          }
          if (hori_block >= (6 - succ_score)) {
            for (int k = 1; k < 6 - succ_score; k++) {
              if (board[row + j - k*1] == color) {
                tmp_score++;
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (hori_block >= (7 - succ_score)) {
              if (board[row + j - (6 - succ_score)] == color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (turn == 1) {
                if (tmp_score + succ_score == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 4) {
                  score[row + j] += 512;
                } else if (tmp_score + succ_score == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + succ_score == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + succ_score == 0) {
                  score[row + j] += 2;
                }
              } else {
                if (tmp_score + succ_score == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 4) {
                  score[row + j] += 0;
                } else if (tmp_score + succ_score == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + succ_score == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + succ_score == 0) {
                  score[row + j] += 2;
                }
              }
            }
          }
        }
        hori_connect = 0;


        tmp_score = 0;
        tmp_block = 7;
        succ_score = 0;
        is_succ = true;
        // Get Vertical score
        // Can make down c6
        if (i <= (19 - (6 - vert_connect[j]))) {
          for (int k = 1; k < 6 - vert_connect[j]; k++) {
            if (board[row + j + k*(19)] == color) {
              tmp_score++;
            } else if (board[row + j + k*(19)] == opp_color) {
              tmp_score = 0;
              tmp_block = k;
              break;
            } else {
              if (is_succ) {
                succ_score = tmp_score;
                is_succ = false;
              }
            }
          }
          // All closed
          is_block = false;
          if (tmp_block + vert_block[j] < 7) {
            is_block = true;
          }
          // Check whether stones pattern can be c7
          // This position makes c7
          is_c7 = false;
          if (i <= (19 - (7 - vert_connect[j]))) {
            if (board[row + j + (6 - vert_connect[j])*(19)] == color) {
              is_c7 = true;
            }
          }
          // Rearrange score
          if (!is_c7 && !is_block) {
            if (turn == 1) {
              if (tmp_score + vert_connect[j] == 5) {
                score[row + j] += 4096;
              } else if (tmp_score + vert_connect[j] == 4) {
                score[row + j] += 512;
              } else if (tmp_score + vert_connect[j] == 3) {
                score[row + j] += 40;
              } else if (tmp_score + vert_connect[j] == 2) {
                score[row + j] += 32;
              } else if (tmp_score + vert_connect[j] == 1) {
                score[row + j] += 20;
              } else if (tmp_score + vert_connect[j] == 0) {
                score[row + j] += 2;
              }
            } else {
              if (tmp_score + vert_connect[j] == 5) {
                score[row + j] += 4096;
              } else if (tmp_score + vert_connect[j] == 4) {
                score[row + j] += 0;
              } else if (tmp_score + vert_connect[j] == 3) {
                score[row + j] += 40;
              } else if (tmp_score + vert_connect[j] == 2) {
                score[row + j] += 32;
              } else if (tmp_score + vert_connect[j] == 1) {
                score[row + j] += 20;
              } else if (tmp_score + vert_connect[j] == 0) {
                score[row + j] += 2;
              }
            }
          }

          tmp_score = 0;
          // Can make Up c6
          if (vert_block[j] >= (6 - succ_score)) {
            for (int k = 1; k < 6 - succ_score; k++) {
              if (board[row + j - k*(19)] == color) {
                tmp_score++;
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (vert_block[j] >= (7 - succ_score)) {
              if (board[row + j - (6 - succ_score)*(19)] == color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (turn == 1) {
                if (tmp_score + succ_score == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 4) {
                  score[row + j] += 512;
                } else if (tmp_score + succ_score == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + succ_score == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + succ_score == 0) {
                  score[row + j] += 2;
                }
              } else {
                if (tmp_score + succ_score == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 4) {
                  score[row + j] += 0;
                } else if (tmp_score + succ_score == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + succ_score == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + succ_score == 0) {
                  score[row + j] += 2;
                }
              }
            }
          }
        // Can't make down c6
        } else {
          // Can make up c6
          for (int k = 1; k < 19 - i; k++) {
            if (board[row + j + k*(19)] == color) {
              tmp_score++;
            } else if (board[row + j + k*(19)] == opp_color) {
              tmp_score = 0;
              break;
            } else {
              if (is_succ) {
                succ_score = tmp_score;
                is_succ = false;
              }
            }
          }
          if (is_succ) {
            succ_score = tmp_score;
          }
          if (vert_block[j] >= (6 - succ_score)) {
            for (int k = 1; k < 6 - succ_score; k++) {
              if (board[row + j - k*(19)] == color) {
                tmp_score++;
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (vert_block[j] >= (7 - succ_score)) {
              if (board[row + j - (6 - succ_score)*(19)] == color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (turn == 1) {
                if (tmp_score + succ_score == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 4) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + succ_score == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + succ_score == 0) {
                  score[row + j] += 2;
                }
              } else {
                if (tmp_score + succ_score == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + succ_score == 4) {
                  score[row + j] += 0;
                } else if (tmp_score + succ_score == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + succ_score == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + succ_score == 0) {
                  score[row + j] += 2;
                }
              }
            }
          }
        }
        vert_connect[j] = 0;


        tmp_score = 0;
        tmp_block = 7;
        succ_score = 0;
        is_succ = true;
        // Get DiagonalLU score
        // Can make RD c6
        if ((i - j >= -13) && (i - j <= 13)) {
          if ((j <= (19 - (6 - diagLU_connect[i-j+13]))) && (i <= (19 - (6 - diagLU_connect[i-j+13])))) {
            for (int k = 1; k < 6 - diagLU_connect[i-j+13]; k++) {
              if (board[row + j + k*(19 + 1)] == color) {
                tmp_score++;
              } else if (board[row + j + k*(19 + 1)] == opp_color) {
                tmp_score = 0;
                tmp_block = k;
                break;
              } else {
                if (is_succ) {
                  succ_score = tmp_score;
                  is_succ = false;
                }
              }
            }
            // All closed
            is_block = false;
            if (tmp_block + diagLU_block[i-j+13] < 7) {
              is_block = true;
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if ((j <= (19 - (7 - diagLU_connect[i-j+13]))) && (i <= (19 - (7 - diagLU_connect[i-j+13])))) {
              if (board[row + j + (6 - diagLU_connect[i-j+13])*(19 + 1)] == color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7 && !is_block) {
              if (turn == 1) {
                if (tmp_score + diagLU_connect[i-j+13] == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + diagLU_connect[i-j+13] == 4) {
                  score[row + j] += 4096;
                } else if (tmp_score + diagLU_connect[i-j+13] == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + diagLU_connect[i-j+13] == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + diagLU_connect[i-j+13] == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + diagLU_connect[i-j+13] == 0) {
                  score[row + j] += 2;
                }
              } else {
                if (tmp_score + diagLU_connect[i-j+13] == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + diagLU_connect[i-j+13] == 4) {
                  score[row + j] += 0;
                } else if (tmp_score + diagLU_connect[i-j+13] == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + diagLU_connect[i-j+13] == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + diagLU_connect[i-j+13] == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + diagLU_connect[i-j+13] == 0) {
                  score[row + j] += 2;
                }
              }
            }

            tmp_score = 0;
            // Can make LU c6
            if (diagLU_block[i-j+13] >= (6 - succ_score)) {
              for (int k = 1; k < 6 - succ_score; k++) {
                if (board[row + j - k*(19 + 1)] == color) {
                  tmp_score++;
                }
              }
              // Check whether stones pattern can be c7
              // This position makes c7
              is_c7 = false;
              if (diagLU_block[i-j+13] >= (7 - succ_score)) {
                if (board[row + j - (6 - succ_score)*(19 + 1)] == color) {
                  is_c7 = true;
                }
              }
              if (!is_c7) {
                if (turn == 1) {
                  if (tmp_score + succ_score == 5) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 4) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 3) {
                    score[row + j] += 40;
                  } else if (tmp_score + succ_score == 2) {
                    score[row + j] += 32;
                  } else if (tmp_score + succ_score == 1) {
                    score[row + j] += 20;
                  } else if (tmp_score + succ_score == 0) {
                    score[row + j] += 2;
                  }
                } else {
                  if (tmp_score + succ_score == 5) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 4) {
                    score[row + j] += 0;
                  } else if (tmp_score + succ_score == 3) {
                    score[row + j] += 40;
                  } else if (tmp_score + succ_score == 2) {
                    score[row + j] += 32;
                  } else if (tmp_score + succ_score == 1) {
                    score[row + j] += 20;
                  } else if (tmp_score + succ_score == 0) {
                    score[row + j] += 2;
                  }
                }
              }
            }
          // Can't make RD c6
          } else {
            // Can make LU c6
            for (int k = 1; k < 19; k++) {
              if ((i + k == 19) || (j + k == 19)) break;
              if (board[row + j + k*(19 + 1)] == color) {
                tmp_score++;
              } else if (board[row + j + k*(19 + 1)] == opp_color) {
                tmp_score = 0;
                break;
              } else {
                if (is_succ) {
                  succ_score = tmp_score;
                  is_succ = false;
                }
              }
            }
            if (is_succ) {
              succ_score = tmp_score;
            }
            if (diagLU_block[i-j+13] >= (6 - succ_score)) {
              for (int k = 1; k < 6 - succ_score; k++) {
                if (board[row + j - k*(19 + 1)] == color) {
                  tmp_score++;
                }
              }
              // Check whether stones pattern can be c7
              // This position makes c7
              is_c7 = false;
              if (diagLU_block[i-j+13] >= (7 - succ_score)) {
                if (board[row + j - (6 - succ_score)*(19 + 1)] == color) {
                  is_c7 = true;
                }
              }
              if (!is_c7) {
                if (turn == 1) {
                  if (tmp_score + succ_score == 5) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 4) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 3) {
                    score[row + j] += 40;
                  } else if (tmp_score + succ_score == 2) {
                    score[row + j] += 32;
                  } else if (tmp_score + succ_score == 1) {
                    score[row + j] += 20;
                  } else if (tmp_score + succ_score == 0) {
                    score[row + j] += 2;
                  }
                } else {
                  if (tmp_score + succ_score == 5) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 4) {
                    score[row + j] += 0;
                  } else if (tmp_score + succ_score == 3) {
                    score[row + j] += 40;
                  } else if (tmp_score + succ_score == 2) {
                    score[row + j] += 32;
                  } else if (tmp_score + succ_score == 1) {
                    score[row + j] += 20;
                  } else if (tmp_score + succ_score == 0) {
                    score[row + j] += 2;
                  }
                }
              }
            }
          }
          diagLU_connect[i-j+13] = 0;
        }


        tmp_score = 0;
        tmp_block = 7;
        succ_score = 0;
        is_succ = true;
        // Get DiagonalRU score
        // Can make LD c6
        if ((i + j >= 5) && (i + j <= 31)) {
          if ((i <= (19 - (6 - diagRU_connect[i+j-5]))) && (j >= 5 - diagRU_connect[i+j-5])) {
            for (int k = 1; k < 6 - diagRU_connect[i+j-5]; k++) {
              if (board[row + j + k*(19 - 1)] == color) {
                tmp_score++;
              } else if (board[row + j + k*(19 - 1)] == opp_color) {
                tmp_score = 0;
                tmp_block = k;
                break;
              } else {
                if (is_succ) {
                  succ_score = tmp_score;
                  is_succ = false;
                }
              }
            }
            // All closed
            is_block = false;
            if (tmp_block + diagRU_block[i+j-5] < 7) {
              is_block = true;
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (i <= (19 - (7 - diagRU_connect[i+j-5])) && (j >= 6 - diagRU_connect[i+j-5])) {
              if (board[row + j + (6 - diagRU_connect[i+j-5])*(19 - 1)] == color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7 && !is_block) {
              if (turn == 1) {
                if (tmp_score + diagRU_connect[i+j-5] == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + diagRU_connect[i+j-5] == 4) {
                  score[row + j] += 4096;
                } else if (tmp_score + diagRU_connect[i+j-5] == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + diagRU_connect[i+j-5] == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + diagRU_connect[i+j-5] == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + diagRU_connect[i+j-5] == 0) {
                  score[row + j] += 2;
                }
              } else {
                if (tmp_score + diagRU_connect[i+j-5] == 5) {
                  score[row + j] += 4096;
                } else if (tmp_score + diagRU_connect[i+j-5] == 4) {
                  score[row + j] += 0;
                } else if (tmp_score + diagRU_connect[i+j-5] == 3) {
                  score[row + j] += 40;
                } else if (tmp_score + diagRU_connect[i+j-5] == 2) {
                  score[row + j] += 32;
                } else if (tmp_score + diagRU_connect[i+j-5] == 1) {
                  score[row + j] += 20;
                } else if (tmp_score + diagRU_connect[i+j-5] == 0) {
                  score[row + j] += 2;
                }
              }
            }

            tmp_score = 0;
            // Can make RU c6
            if (diagRU_block[i+j-5] >= (6 - succ_score)) {
              for (int k = 1; k < 6 - succ_score; k++) {
                if (board[row + j - k*(19 - 1)] == color) {
                  tmp_score++;
                }
              }
              // Check whether stones pattern can be c7
              // This position makes c7
              is_c7 = false;
              if (diagRU_block[i+j-5] >= (7 - succ_score)) {
                if (board[row + j - (6 - succ_score)*(19 - 1)] == color) {
                  is_c7 = true;
                }
              }
              if (!is_c7) {
                if (turn == 1) {
                  if (tmp_score + succ_score == 5) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 4) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 3) {
                    score[row + j] += 40;
                  } else if (tmp_score + succ_score == 2) {
                    score[row + j] += 32;
                  } else if (tmp_score + succ_score == 1) {
                    score[row + j] += 20;
                  } else if (tmp_score + succ_score == 0) {
                    score[row + j] += 2;
                  }
                } else {
                  if (tmp_score + succ_score == 5) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 4) {
                    score[row + j] += 0;
                  } else if (tmp_score + succ_score == 3) {
                    score[row + j] += 40;
                  } else if (tmp_score + succ_score == 2) {
                    score[row + j] += 32;
                  } else if (tmp_score + succ_score == 1) {
                    score[row + j] += 20;
                  } else if (tmp_score + succ_score == 0) {
                    score[row + j] += 2;
                  }
                }
              }
            }
          // Can't make LD c6
          } else {
            // Can make RU c6
            for (int k = 1; k < 19; k++) {
              if ((i + k == 19) || (j - k == -1)) break;
              if (board[row + j + k*(19 - 1)] == color) {
                tmp_score++;
              } else if (board[row + j + k*(19 - 1)] == opp_color) {
                tmp_score = 0;
                break;
              } else {
                if (is_succ) {
                  succ_score = tmp_score;
                  is_succ = false;
                }
              }
            }
            if (is_succ) {
              succ_score = tmp_score;
            }
            if (diagRU_block[i+j-5] >= (6 - succ_score)) {
              for (int k = 1; k < 6 - succ_score; k++) {
                if (board[row + j - k*(19 - 1)] == color) {
                  tmp_score++;
                }
              }
              // Check whether stones pattern can be c7
              // This position makes c7
              is_c7 = false;
              if (diagRU_block[i+j-5] >= (7 - succ_score)) {
                if (board[row + j - (6 - succ_score)*(19 - 1)] == color) {
                  is_c7 = true;
                }
              }
              if (!is_c7) {
                if (turn == 1) {
                  if (tmp_score + succ_score == 5) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 4) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 3) {
                    score[row + j] += 40;
                  } else if (tmp_score + succ_score == 2) {
                    score[row + j] += 32;
                  } else if (tmp_score + succ_score == 1) {
                    score[row + j] += 20;
                  } else if (tmp_score + succ_score == 0) {
                    score[row + j] += 2;
                  }
                } else {
                  if (tmp_score + succ_score == 5) {
                    score[row + j] += 4096;
                  } else if (tmp_score + succ_score == 4) {
                    score[row + j] += 0;
                  } else if (tmp_score + succ_score == 3) {
                    score[row + j] += 40;
                  } else if (tmp_score + succ_score == 2) {
                    score[row + j] += 32;
                  } else if (tmp_score + succ_score == 1) {
                    score[row + j] += 20;
                  } else if (tmp_score + succ_score == 0) {
                    score[row + j] += 2;
                  }
                }
              }
            }
          }
          diagRU_connect[i+j-5] = 0;
        }

      } else if (board[row + j] == color) {
        hori_connect++;
        vert_connect[j]++;
        diagLU_connect[i-j+13]++;
        diagRU_connect[i+j-5]++;
      } else {
        hori_connect = 0;
        vert_connect[j] = 0;
        diagLU_connect[i-j+13] = 0;
        diagRU_connect[i+j-5] = 0;
        hori_block = 0;
        vert_block[j] = 0;
        diagLU_block[i-j+13] = 0;
        diagRU_block[i+j-5] = 0;
      }
    }
  }

  // Test Score
  // out1 << endl;
  // for (int i = 0; i < 19; i++) {
  //   int iter = 19 * i;
  //   for (int j = 0; j < 19; j++) {
  //     out1 << score[iter + j] << "  ";
  //   }
  //   out1 << endl;
  // }
  // out1.close();

  // Get opponent's score
  for (int i = 0; i < 19; i++) {
    vert_connect[i] = 0;
    vert_block[i] = 0;
  }
  for (int i = 0; i < 27; i++) {
    diagLU_connect[i] = 0;
    diagRU_connect[i] = 0;
    diagLU_block[i] = 0;
    diagRU_block[i] = 0;
  }
  for (int i = 0; i < 19; i++) {
    int row = 19 * i;
    int hori_connect = 0;
    int hori_block = 0;
    for (int j = 0; j < 19; j++) {
      // Update distance from blockings
      hori_block++;
      vert_block[j]++;
      if ((i - j >= -13) && (i - j <= 13)) {
        diagLU_block[i-j+13]++;
      }
      if ((i + j >= 5) && (i + j <= 31)) {
        diagRU_block[i+j-5]++;
      }
      if (board[row + j] == 0) {
        // tmp_score: Save the number of my stones after Position
        // succ_score: Save the number of succesive my stones after Position
        int tmp_score = 0;
        int succ_score = 0;
        bool is_succ = true;
        bool is_c7 = false;
        // Get Horizontal score
        // Can make right c6
        if (j <= (19 - (6 - hori_connect))) {
          for (int k = 1; k < 6 - hori_connect; k++) {
            if (board[row + j + k*1] == opp_color) {
              tmp_score++;
            } else if (board[row + j + k*1] == color) {
              tmp_score = 0;
              break;
            } else {
              if (is_succ) {
                succ_score = tmp_score;
                is_succ = false;
              }
            }
          }
          // Check whether stones pattern can be c7
          // This position makes c7
          is_c7 = false;
          if (j <= (19 - (7 - hori_connect))) {
            if (board[row + j + (6 - hori_connect)] == opp_color) {
              is_c7 = true;
            }
          }
          // Rearrange score
          if (!is_c7) {
            if (tmp_score + hori_connect == 5) {
              score_opp[row + j] += 512;
            } else if (tmp_score + hori_connect == 4) {
              score_opp[row + j] += 512;
            } else if (tmp_score + hori_connect == 3) {
              score_opp[row + j] += 64;
            } else if (tmp_score + hori_connect == 2) {
              score_opp[row + j] += 32;
            } else if (tmp_score + hori_connect == 1) {
              score_opp[row + j] += 4;
            } else if (tmp_score + hori_connect == 0) {
              score_opp[row + j] += 2;
            }
          }

          tmp_score = 0;
          // Can make left c6
          if (hori_block >= (6 - succ_score)) {
            for (int k = 1; k < 6 - succ_score; k++) {
              if (board[row + j - k*1] == opp_color) {
                tmp_score++;
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (hori_block >= (7 - succ_score)) {
              if (board[row + j - (6 - succ_score)] == opp_color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (tmp_score + succ_score == 5) {
                score_opp[row + j] += 512;
              } else if (tmp_score + succ_score == 4) {
                score_opp[row + j] += 512;
              } else if (tmp_score + succ_score == 3) {
                score_opp[row + j] += 64;
              } else if (tmp_score + succ_score == 2) {
                score_opp[row + j] += 32;
              } else if (tmp_score + succ_score == 1) {
                score_opp[row + j] += 4;
              } else if (tmp_score + succ_score == 0) {
                score_opp[row + j] += 2;
              }
            }
          }
        // Can't make right c6
        } else {
          // Can make left c6
          for (int k = 1; k < 19 - j; k++) {
            if (board[row + j + k*1] == opp_color) {
              tmp_score++;
            } else if (board[row + j + k*1] == color) {
              tmp_score = 0;
              break;
            } else {
              if (is_succ) {
                succ_score = tmp_score;
                is_succ = false;
              }
            }
          }
          if (is_succ) {
            succ_score = tmp_score;
          }
          if (hori_block >= (6 - succ_score)) {
            for (int k = 1; k < 6 - succ_score; k++) {
              if (board[row + j - k*1] == opp_color) {
                tmp_score++;
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (hori_block >= (7 - succ_score)) {
              if (board[row + j - (6 - succ_score)] == opp_color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (tmp_score + succ_score == 5) {
                score_opp[row + j] += 512;
              } else if (tmp_score + succ_score == 4) {
                score_opp[row + j] += 512;
              } else if (tmp_score + succ_score == 3) {
                score_opp[row + j] += 64;
              } else if (tmp_score + succ_score == 2) {
                score_opp[row + j] += 32;
              } else if (tmp_score + succ_score == 1) {
                score_opp[row + j] += 4;
              } else if (tmp_score + succ_score == 0) {
                score_opp[row + j] += 2;
              }
            }
          }
        }
        hori_connect = 0;


        tmp_score = 0;
        succ_score = 0;
        is_succ = true;
        // Get Vertical score
        // Can make down c6
        if (i <= (19 - (6 - vert_connect[j]))) {
          for (int k = 1; k < 6 - vert_connect[j]; k++) {
            if (board[row + j + k*(19)] == opp_color) {
              tmp_score++;
            } else if (board[row + j + k*(19)] == color) {
              tmp_score = 0;
              break;
            } else {
              if (is_succ) {
                succ_score = tmp_score;
                is_succ = false;
              }
            }
          }
          // Check whether stones pattern can be c7
          // This position makes c7
          is_c7 = false;
          if (i <= (19 - (7 - vert_connect[j]))) {
            if (board[row + j + (6 - vert_connect[j])*(19)] == opp_color) {
              is_c7 = true;
            }
          }
          // Rearrange score
          if (!is_c7) {
            if (tmp_score + vert_connect[j] == 5) {
              score_opp[row + j] += 512;
            } else if (tmp_score + vert_connect[j] == 4) {
              score_opp[row + j] += 512;
            } else if (tmp_score + vert_connect[j] == 3) {
              score_opp[row + j] += 64;
            } else if (tmp_score + vert_connect[j] == 2) {
              score_opp[row + j] += 32;
            } else if (tmp_score + vert_connect[j] == 1) {
              score_opp[row + j] += 4;
            } else if (tmp_score + vert_connect[j] == 0) {
              score_opp[row + j] += 2;
            }
          }

          tmp_score = 0;
          // Can make Up c6
          if (vert_block[j] >= (6 - succ_score)) {
            for (int k = 1; k < 6 - succ_score; k++) {
              if (board[row + j - k*(19)] == opp_color) {
                tmp_score++;
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (vert_block[j] >= (7 - succ_score)) {
              if (board[row + j - (6 - succ_score)*(19)] == opp_color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (tmp_score + succ_score == 5) {
                score_opp[row + j] += 512;
              } else if (tmp_score + succ_score == 4) {
                score_opp[row + j] += 512;
              } else if (tmp_score + succ_score == 3) {
                score_opp[row + j] += 64;
              } else if (tmp_score + succ_score == 2) {
                score_opp[row + j] += 32;
              } else if (tmp_score + succ_score == 1) {
                score_opp[row + j] += 4;
              } else if (tmp_score + succ_score == 0) {
                score_opp[row + j] += 2;
              }
            }
          }
        // Can't make down c6
        } else {
          // Can make up c6
          for (int k = 1; k < 19 - i; k++) {
            if (board[row + j + k*(19)] == opp_color) {
              tmp_score++;
            } else if (board[row + j + k*(19)] == color) {
              tmp_score = 0;
              break;
            } else {
              if (is_succ) {
                succ_score = tmp_score;
                is_succ = false;
              }
            }
          }
          if (is_succ) {
            succ_score = tmp_score;
          }
          if (vert_block[j] >= (6 - succ_score)) {
            for (int k = 1; k < 6 - succ_score; k++) {
              if (board[row + j - k*(19)] == opp_color) {
                tmp_score++;
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (vert_block[j] >= (7 - succ_score)) {
              if (board[row + j - (6 - succ_score)*(19)] == opp_color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (tmp_score + succ_score == 5) {
                score_opp[row + j] += 512;
              } else if (tmp_score + succ_score == 4) {
                score_opp[row + j] += 512;
              } else if (tmp_score + succ_score == 3) {
                score_opp[row + j] += 64;
              } else if (tmp_score + succ_score == 2) {
                score_opp[row + j] += 32;
              } else if (tmp_score + succ_score == 1) {
                score_opp[row + j] += 4;
              } else if (tmp_score + succ_score == 0) {
                score_opp[row + j] += 2;
              }
            }
          }
        }
        vert_connect[j] = 0;


        tmp_score = 0;
        succ_score = 0;
        is_succ = true;
        // Get DiagonalLU score
        // Can make RD c6
        if ((i - j >= -13) && (i - j <= 13)) {
          if ((j <= (19 - (6 - diagLU_connect[i-j+13]))) && (i <= (19 - (6 - diagLU_connect[i-j+13])))) {
            for (int k = 1; k < 6 - diagLU_connect[i-j+13]; k++) {
              if (board[row + j + k*(19 + 1)] == opp_color) {
                tmp_score++;
              } else if (board[row + j + k*(19 + 1)] == color) {
                tmp_score = 0;
                break;
              } else {
                if (is_succ) {
                  succ_score = tmp_score;
                  is_succ = false;
                }
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if ((j <= (19 - (7 - diagLU_connect[i-j+13]))) && (i <= (19 - (7 - diagLU_connect[i-j+13])))) {
              if (board[row + j + (6 - diagLU_connect[i-j+13])*(19 + 1)] == opp_color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (tmp_score + diagLU_connect[i-j+13] == 5) {
                score_opp[row + j] += 512;
              } else if (tmp_score + diagLU_connect[i-j+13] == 4) {
                score_opp[row + j] += 512;
              } else if (tmp_score + diagLU_connect[i-j+13] == 3) {
                score_opp[row + j] += 64;
              } else if (tmp_score + diagLU_connect[i-j+13] == 2) {
                score_opp[row + j] += 32;
              } else if (tmp_score + diagLU_connect[i-j+13] == 1) {
                score_opp[row + j] += 4;
              } else if (tmp_score + diagLU_connect[i-j+13] == 0) {
                score_opp[row + j] += 2;
              }
            }

            tmp_score = 0;
            // Can make LU c6
            if (diagLU_block[i-j+13] >= (6 - succ_score)) {
              for (int k = 1; k < 6 - succ_score; k++) {
                if (board[row + j - k*(19 + 1)] == opp_color) {
                  tmp_score++;
                }
              }
              // Check whether stones pattern can be c7
              // This position makes c7
              is_c7 = false;
              if (diagLU_block[i-j+13] >= (7 - succ_score)) {
                if (board[row + j - (6 - succ_score)*(19 + 1)] == opp_color) {
                  is_c7 = true;
                }
              }
              // Rearrange score
              if (!is_c7) {
                if (tmp_score + succ_score == 5) {
                  score_opp[row + j] += 512;
                } else if (tmp_score + succ_score == 4) {
                  score_opp[row + j] += 512;
                } else if (tmp_score + succ_score == 3) {
                  score_opp[row + j] += 64;
                } else if (tmp_score + succ_score == 2) {
                  score_opp[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score_opp[row + j] += 4;
                } else if (tmp_score + succ_score == 0) {
                  score_opp[row + j] += 2;
                }
              }
            }
          // Can't make RD c6
          } else {
            // Can make LU c6
            for (int k = 1; k < 19; k++) {
              if ((i + k == 19) || (j + k == 19)) break;
              if (board[row + j + k*(19 + 1)] == opp_color) {
                tmp_score++;
              } else if (board[row + j + k*(19 + 1)] == color) {
                tmp_score = 0;
                break;
              } else {
                if (is_succ) {
                  succ_score = tmp_score;
                  is_succ = false;
                }
              }
            }
            if (is_succ) {
              succ_score = tmp_score;
            }
            if (diagLU_block[i-j+13] >= (6 - succ_score)) {
              for (int k = 1; k < 6 - succ_score; k++) {
                if (board[row + j - k*(19 + 1)] == opp_color) {
                  tmp_score++;
                }
              }
              // Check whether stones pattern can be c7
              // This position makes c7
              is_c7 = false;
              if (diagLU_block[i-j+13] >= (7 - succ_score)) {
                if (board[row + j - (6 - succ_score)*(19 + 1)] == opp_color) {
                  is_c7 = true;
                }
              }
              // Rearrange score
              if (!is_c7) {
                if (tmp_score + succ_score == 5) {
                  score_opp[row + j] += 512;
                } else if (tmp_score + succ_score == 4) {
                  score_opp[row + j] += 512;
                } else if (tmp_score + succ_score == 3) {
                  score_opp[row + j] += 64;
                } else if (tmp_score + succ_score == 2) {
                  score_opp[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score_opp[row + j] += 4;
                } else if (tmp_score + succ_score == 0) {
                  score_opp[row + j] += 2;
                }
              }
            }
          }
          diagLU_connect[i-j+13] = 0;
        }


        tmp_score = 0;
        succ_score = 0;
        is_succ = true;
        // Get DiagonalRU score
        // Can make LD c6
        if ((i + j >= 5) && (i + j <= 31)) {
          if ((i <= (19 - (6 - diagRU_connect[i+j-5]))) && (j >= 5 - diagRU_connect[i+j-5])) {
            for (int k = 1; k < 6 - diagRU_connect[i+j-5]; k++) {
              if (board[row + j + k*(19 - 1)] == opp_color) {
                tmp_score++;
              } else if (board[row + j + k*(19 - 1)] == color) {
                tmp_score = 0;
                break;
              } else {
                if (is_succ) {
                  succ_score = tmp_score;
                  is_succ = false;
                }
              }
            }
            // Check whether stones pattern can be c7
            // This position makes c7
            is_c7 = false;
            if (i <= (19 - (7 - diagRU_connect[i+j-5])) && (j >= 6 - diagRU_connect[i+j-5])) {
              if (board[row + j + (6 - diagRU_connect[i+j-5])*(19 - 1)] == opp_color) {
                is_c7 = true;
              }
            }
            // Rearrange score
            if (!is_c7) {
              if (tmp_score + diagRU_connect[i+j-5] == 5) {
                score_opp[row + j] += 512;
              } else if (tmp_score + diagRU_connect[i+j-5] == 4) {
                score_opp[row + j] += 512;
              } else if (tmp_score + diagRU_connect[i+j-5] == 3) {
                score_opp[row + j] += 64;
              } else if (tmp_score + diagRU_connect[i+j-5] == 2) {
                score_opp[row + j] += 32;
              } else if (tmp_score + diagRU_connect[i+j-5] == 1) {
                score_opp[row + j] += 4;
              } else if (tmp_score + diagRU_connect[i+j-5] == 0) {
                score_opp[row + j] += 2;
              }
            }

            tmp_score = 0;
            // Can make RU c6
            if (diagRU_block[i+j-5] >= (6 - succ_score)) {
              for (int k = 1; k < 6 - succ_score; k++) {
                if (board[row + j - k*(19 - 1)] == opp_color) {
                  tmp_score++;
                }
              }
              // Check whether stones pattern can be c7
              // This position makes c7
              is_c7 = false;
              if (diagRU_block[i+j-5] >= (7 - succ_score)) {
                if (board[row + j - (6 - succ_score)*(19 - 1)] == opp_color) {
                  is_c7 = true;
                }
              }
              // Rearrange score
              if (!is_c7) {
                if (tmp_score + succ_score == 5) {
                  score_opp[row + j] += 512;
                } else if (tmp_score + succ_score == 4) {
                  score_opp[row + j] += 512;
                } else if (tmp_score + succ_score == 3) {
                  score_opp[row + j] += 64;
                } else if (tmp_score + succ_score == 2) {
                  score_opp[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score_opp[row + j] += 4;
                } else if (tmp_score + succ_score == 0) {
                  score_opp[row + j] += 2;
                }
              }
            }
          // Can't make LD c6
          } else {
            // Can make RU c6
            for (int k = 1; k < 19; k++) {
              if ((i + k == 19) || (j - k == -1)) break;
              if (board[row + j + k*(19 - 1)] == opp_color) {
                tmp_score++;
              } else if (board[row + j + k*(19 - 1)] == color) {
                tmp_score = 0;
                break;
              } else {
                if (is_succ) {
                  succ_score = tmp_score;
                  is_succ = false;
                }
              }
            }
            if (is_succ) {
              succ_score = tmp_score;
            }
            if (diagRU_block[i+j-5] >= (6 - succ_score)) {
              for (int k = 1; k < 6 - succ_score; k++) {
                if (board[row + j - k*(19 - 1)] == opp_color) {
                  tmp_score++;
                }
              }
              // Check whether stones pattern can be c7
              // This position makes c7
              is_c7 = false;
              if (diagRU_block[i+j-5] >= (7 - succ_score)) {
                if (board[row + j - (7 - succ_score)*(19 - 1)] == opp_color) {
                  is_c7 = true;
                }
              }
              // Rearrange score
              if (!is_c7) {
                if (tmp_score + succ_score == 5) {
                  score_opp[row + j] += 512;
                } else if (tmp_score + succ_score == 4) {
                  score_opp[row + j] += 512;
                } else if (tmp_score + succ_score == 3) {
                  score_opp[row + j] += 64;
                } else if (tmp_score + succ_score == 2) {
                  score_opp[row + j] += 32;
                } else if (tmp_score + succ_score == 1) {
                  score_opp[row + j] += 4;
                } else if (tmp_score + succ_score == 0) {
                  score_opp[row + j] += 2;
                }
              }
            }
          }
          diagRU_connect[i+j-5] = 0;
        }

      } else if (board[row + j] == opp_color) {
        hori_connect++;
        vert_connect[j]++;
        diagLU_connect[i-j+13]++;
        diagRU_connect[i+j-5]++;
      } else {
        hori_connect = 0;
        vert_connect[j] = 0;
        diagLU_connect[i-j+13] = 0;
        diagRU_connect[i+j-5] = 0;
        hori_block = 0;
        vert_block[j] = 0;
        diagLU_block[i-j+13] = 0;
        diagRU_block[i+j-5] = 0;
      }
    }
  }

  // // Test Score
  // out2 << endl;
  // for (int i = 0; i < 19; i++) {
  //   int iter = 19 * i;
  //   for (int j = 0; j < 19; j++) {
  //     out2 << score_opp[iter + j] << "  ";
  //   }
  //   out2 << endl;
  // }
  // out2.close();

  // Add score Arrays
  for (int i = 0; i < 361; i++) {
    score[i] += (score_opp[i] + score[i]);
  }

  // // Test Score
  // total << endl;
  // for (int i = 0; i < 19; i++) {
  //   int iter = 19 * i;
  //   for (int j = 0; j < 19; j++) {
  //     total << score[iter + j] << "  ";
  //   }
  //   total << endl;
  // }
  // total.close();

  // Select (num) Best picks
  int min_idx = 0;
  int min = score[0];
  indexes[0] = 0;
  for (int i = 1; i < num; i++) {
    indexes[i] = i;
    if (score[i] < min) {
      min = score[i];
      min_idx = i;
    }
  }
  for (int i = num; i < 361; i++) {
    if (score[i] > min) {
      indexes[min_idx] = i;
      min_idx = 0;
      min = score[indexes[0]];
      for (int j = 1; j < num; j++) {
        if (score[indexes[j]] < min) {
          min = score[indexes[j]];
          min_idx = j;
        }
      }
    }
  }
}

// Without Thread
int State::Evaluation() {
  int win_count = 0;

  VirtualPlay(win_count);

  return win_count;
}

// Vitually play connect6
void State::VirtualPlay(int& win_count) {
  char virtual_board[361];
  int index[1];
  int is_end;
  int empty = 0;
  // Copy board to virtual board
  for (int i = 0; i < 361; i++) {
    virtual_board[i] = board_[i];
    if (board_[i] == 0){
      empty++;
    }
  }
  // Do Connect6 using best pick derived from our strategy
  for (int round = 0; round < mct_const::NUMBER_OF_ROUNDS; round++) {
    // If there is no space to place stone
    if (empty < 4) break;

    GetBestPositions(virtual_board, index, 1, my_color, 1);
    virtual_board[index[0]] = my_color;
    is_end = IsEnd(virtual_board, index[0], my_color);
    if (is_end == 1) {
      win_count++;
      return;
    } else if (is_end == -1) {
      return;
    }
    GetBestPositions(virtual_board, index, 1, my_color, 2);
    virtual_board[index[0]] = my_color;
    is_end = IsEnd(virtual_board, index[0], my_color);
    if (is_end == 1) {
      win_count++;
      return;
    } else if (is_end == -1) {
      return;
    }
    GetBestPositions(virtual_board, index, 1, userColor, 1);
    virtual_board[index[0]] = userColor;
    is_end = IsEnd(virtual_board, index[0], userColor);
    if (is_end == 1) {
      win_count++;
      return;
    } else if (is_end == -1) {
      return;
    }
    GetBestPositions(virtual_board, index, 1, userColor, 2);
    virtual_board[index[0]] = userColor;
    is_end = IsEnd(virtual_board, index[0], userColor);
    if (is_end == 1) {
      win_count++;
      return;
    } else if (is_end == -1) {
      return;
    }
    empty -= 4;
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
// The biggest win rate
State* State::BestChoice() {
  int max_idx = 0;
  // double max_uct = child_list_[0]->uct_value_;
  // for (int i = 1; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
  //   if (child_list_[i]->uct_value_ > max_uct) {
  //     max_uct = child_list_[i]->uct_value_;
  //     max_idx = i;
  //   }
  // }
  double max_winrate = (double)child_list_[0]->number_of_wins_ / child_list_[0]->number_of_visiting_;
  for (int i =1; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
    if (((double)child_list_[i]->number_of_wins_ / child_list_[i]->number_of_visiting_) > max_winrate) {
      max_winrate = (double)child_list_[i]->number_of_wins_ / child_list_[i]->number_of_visiting_;
      max_idx = i;
    }
  }

  best_pos1 = child_list_[max_idx]->change_idx_1_;
  best_pos2 = child_list_[max_idx]->change_idx_2_;

  // Test values
  // ofstream out("out.txt", ios::app);
  // out << best_pos1/19 << ", " << best_pos1%19 << " | ";
  // out << best_pos2/19 << ", " << best_pos2%19 << endl;
  // for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
  //   out << "UCT: " << child_list_[i]->uct_value_ << " Visit: " << child_list_[i]->number_of_visiting_;
  //   out << " Win Rate: " << (double) child_list_[i]->number_of_wins_/child_list_[i]->number_of_visiting_ << endl;
  // }
  // out << endl;
  // out.close();
  //

  return child_list_[max_idx];
}

// Detect end condition
int IsEnd(const char* _board, const int pos, const char color) {
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
