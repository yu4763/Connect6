#include "monte_carlo_tree_search.h"
using namespace std;

Position MonteCarloTreeSearch(State& current, double recv_time) {
  // Calculate start time
  chrono::system_clock::time_point start = chrono::system_clock::now();

  // Modify received time to return safe
  recv_time -= TERMINIATE_TIME_PADDING;
  Position best_pos;

  // Play game virtually during received time
  while((chrono::duration<double>(chrono::system_clock::now() - start).count() < recv_time) {
    State& best_child = current.SelectionAndExpansion();
    best_child.Evaluation();
    best_child.Update();
    best_pos = current.BestChoice();
  }
  return best_pos;
}

State& State::SelectionAndExpansion() {
  // If all children are visited, then go to best child.
  // If there are children to check, then select it and expand children.
  while(1) {
    if () {


    } else {

      return
    }
  }
}

void State::Evaluation() {
  thread** thread_list = new thread*[mct_const::NUMBER_OF_THREADS];
  for (int i = 0; i < mct_const::NUMBER_OF_THREADS; i++) {
    thread_list[i] =
  }

  for (int i = 0; i < mct_const::NUMBER_OF_THREADS; i++) {
    thread_list[i]->join();
  }

  delete[] thread_list;
}

void State::Update() {

}
