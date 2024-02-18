#include <iostream>
#include <array>
#include <vector>
#include <chrono>

#include <fstream>
#include <string>
#include <queue>
#include <thread>
#include "thread_pool.h"
#include <stdlib.h> 

using namespace std;

#include "sudoku.h"




static int moves=0;
static const short int amount_of_threads=10;


class SudokuSolver{
    public:
        SudokuSolver(GuessList g1, bool solved1, bool consistent1, bool possible1) : g(g1),solved(solved1),consistent(consistent1),possible(possible1){}
        SudokuSolver(){

        }
        void solve(Sudoku& s){
            s.InsertGuesslist(g);
            s.UpdatePossibilies();
            while(s.FillSinglePossibility()==true);

            solved = s.CheckFullySolved();
            consistent = s.CheckConsistent();
            possible = s.CheckPossible();
        }
        GuessList g;
        bool solved;
        bool consistent;
        bool possible;
};

static SudokuSolver static_solver;
static Sudoku static_sudoku;

void ItterativeSolverSingleThread(Sudoku s, SudokuSolver solve, ThreadPool* pool){
    Sudoku sud = s;
    solve.solve(sud);
    if(solve.solved&&solve.consistent){
        static_solver=solve;
        static_sudoku=sud;
        pool->SetStop();
        return;
    }else if(!(solve.consistent&&solve.possible)){
        moves++;
    }else if(!solve.solved){
        moves++;
        queue<GuessList> ToBeChecked;
        solve.g.SpecifyGuessQueue(ToBeChecked);
        while(ToBeChecked.empty()==false){
            SudokuSolver newSolver(ToBeChecked.front(),false,false,false);
            ToBeChecked.pop();
            pool->enqueue(ItterativeSolverSingleThread,s,newSolver,pool);
        }
    }
}

void ItterativeSolver(Sudoku&s ,GuessList& l){
    ThreadPool pool(10);
    SudokuSolver newSolver(l,false,false,false);
    ItterativeSolverSingleThread(s,newSolver,&pool);
    while(!pool.allTasksCompleted()){
        std::this_thread::sleep_for(std::chrono::microseconds{10});
    }
}


int main(int argc, char **argv)
{
    if(argc>2){throw runtime_error("Too many arguments.");}
    else if(argc==1){throw runtime_error("No path to sudoku given.");}
    
    string path = argv[1];

    array<short int, 9*9> a;
    ifstream myfile;
    
    myfile.open(path);
    if(myfile.fail()){
        cerr << "Couldn't find file.";
        exit(1);
    }
    

    if ( myfile.is_open() ) {
        int i=0;
        char mychar;
        while ( myfile ) {
            mychar = myfile.get();
            if(isdigit(mychar)){
                if(i>a.size()-1){throw runtime_error("Too many numbers in file.");}
                a[i] = (short int) (mychar-'0');
                i++;
            }
        }
    }
    myfile.close();
    
    
    std::cout << "Before filling in single possibilities: " << endl;
    Sudoku s = Sudoku(a);
    std::cout << s << endl;
    std::cout << "Consistent: " << s.CheckConsistent() << endl;
    std::cout << "Fully solved: " << s.CheckFullySolved() << endl;
    std::cout << "Possible: " << s.CheckPossible() << endl;

    auto start = chrono::system_clock::now();
    s.UpdatePossibilies();
    GuessList l = GuessList(s);
    ItterativeSolver(s,l);

    auto end = chrono::system_clock::now();
    auto elapsed = end - start;
    std::cout << "It took " << elapsed.count() << " micro seconds." << endl;
    std::cout << "It took " << moves << " guesses." << endl;

    std::cout << static_solver.g << endl;
    std::cout << static_sudoku << endl;
    
    return 0;
}