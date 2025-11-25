#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <syncstream> 
#include <latch>
#include <map>
#include <stdexcept>

//Compiler: Microsoft Visual Studio 2022 (MSVC), Standard: C++20
//Berezhna Olga, K-27
//Variant 13

using namespace std;

const map<char, int> action_count = {
    {'a', 7}, {'b', 4}, {'c', 5}, {'d', 8}, {'e', 7},
    {'f', 4}, {'g', 7}, {'h', 9}, {'i', 5}
};

latch level1_done(1);
latch level2_done(3);

void print(char set_name, int action_index) {
   this_thread::sleep_for(chrono::milliseconds(20)); 
   osyncstream(cout) << "From set " << set_name << " action was performed " << action_index << endl;
}

void execute_set(char set_name, int count) {
    for (int i = 1; i <= count; ++i) {
        print(set_name, i);
    }
}


void thread_worker(int id) {

    if (id == 1) {
        execute_set('a', action_count.at('a'));
        level1_done.count_down(); 
    }

    level1_done.wait();

    if (id == 1) {
        execute_set('d', action_count.at('d'));
        level2_done.count_down(); 
    }
    else if (id == 2) {
        execute_set('b', action_count.at('b'));
        level2_done.count_down(); 
    }
    else if (id == 3) {
        execute_set('c', action_count.at('c'));
        level2_done.count_down(); 
    }

    level2_done.wait();

    if (id == 1) {
         execute_set('h', action_count.at('h'));
    }
    else if (id == 2) {
         execute_set('e', action_count.at('e'));
    }
    else if (id == 3) {
         execute_set('g', action_count.at('g'));
    }
    else if (id == 4) {
        execute_set('i', action_count.at('i'));
        execute_set('f', action_count.at('f'));
    }
}

int main() {
    int nt = 4;
    vector<thread> threads;
   
    osyncstream(cout) << "Calculation started!\n";

    for (int i = 1; i <= nt; ++i) {
        threads.emplace_back(thread_worker, i);
    }
    for (auto& t : threads) {
        if (t.joinable())
            t.join(); 
    }

    osyncstream(cout) << "\nCalculation finished!\n" << std::endl;

    return 0;
}