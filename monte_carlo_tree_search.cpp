#include "monte_carlo_tree_search.h"
using namespace std;

//
// TO DO List
//
// 1. SelectionAndExpansion Implementation V \ MakeChildState Implementation
// 2. Evaluation random index Selection
// 3. VirtualPlay Implementation
// 4. Calculate uct_value
// 5. Determine Bestchoice strategy
// 6. Recursive to Iterative
//

// Main MonteCarloTreeSearch Implemetation
Position MonteCarloTreeSearch(State& current, double recv_time) {
  // Calculate start time
  chrono::system_clock::time_point start = chrono::system_clock::now();

  // Modify received time to return safely
  recv_time -= mct_const::TERMINIATE_TIME_PADDING;
  Position best_pos;

  // Play game virtually during received time
  while ((chrono::duration<double>(chrono::system_clock::now() - start).count() < recv_time) {
    State& best_child = current.SelectionAndExpansion();
    best_child.Update(best_child.Evaluation());
    best_pos = current.BestChoice();
  }

  return best_pos;
}

State& State::SelectionAndExpansion() {
  // If there is no child state, then expand and return its child.
  // If all children are visited, then go to best child.
  int max_idx;
  double max_uct_value = 0;
  for (int i = 0; i < mct_const::NUMBER_OF_MAX_CHILD_NODES; i++) {
    if (child_list_[i] == NULL) {
      for ( int j = 0; j < mct_const::NUMBER_OF_MAX_CHILD_NODES; j++) {
        //
        // Make child state with our strategy MakeChildState();
        // Save change index
        //
      }
      return *child_list_[i];
    }
    // For non visited node
    if (child_list_[i]->number_of_visiting_ == 0) return *child_list_[i];

    // Compare uct_values to choose best child
    if (child_list_[i]->uct_value_ > max_uct_value) {
      max_idx = i;
      max_uct_value = child_list_[i]->uct_value_;
    }
  }

  return child_list_[max_idx]->SelectionAndExpansion();
}

int State::Evaluation() {
  int win_count = 0;
  thread** thread_list = new thread*[mct_const::NUMBER_OF_THREADS];

  //
  // 2. Select random index of child child_list_
  //
  for (int i = 0; i < mct_const::NUMBER_OF_THREADS; i++) {
    thread_list[i] = new thread(VirtualPlay, win_count);
  }

  for (int i = 0; i < mct_const::NUMBER_OF_THREADS; i++) {
    thread_list[i]->join();
  }
  delete[] thread_list;

  return win_count;
}

void State::VirtualPlay(int& win_count) {
  // Do Connect6 randomly according to expected strategy
  int number_of_rounds;
  for (int i = 0; i < number_of_rounds; i++) {
    //
    // 3. strategy
    //
  }
  // Check whether win this game
  if () {

  }
}

void State::Update(int result) {
  number_of_wins_ += result;
  number_of_visiting_ += mct_const::NUMBER_OF_THREADS;
  //
  // 4. Change uct_value_
  //
  parent_->Update();
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
