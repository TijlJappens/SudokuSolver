#include <iostream>
#include <array>
#include <vector>
#include <chrono>

using namespace std;

#include "sudoku.h"



static int moves=0;

pair<GuessList,Sudoku> ItterativeSolver(Sudoku&s ,GuessList& l){

    pair<GuessList,Sudoku> p(l,s);
    while(true){
        p.second=s;
        p.second.InsertGuesslist(p.first);
        p.second.UpdatePossibilies();
        while(p.second.FillSinglePossibility()==true);

        bool solved = p.second.CheckFullySolved();
        bool consistent = p.second.CheckConsistent();
        bool possible = p.second.CheckPossible();

        if(solved && consistent){
            //cout << "Solved and consistent." << endl;
            return p;
        }else if(!consistent || !possible){
            //cout << "Changing guess" << endl;
            p.first.ChangeWrongGuess();
        }else if(!solved){
            //cout << "Specifying guess" << endl;
            p.first.SpecifyGuess();
        }
    }
    throw runtime_error("Cannot solve sudoku.");
    return p;
}


int main()
{
    array<int, 9*9> starter_sudoku_array = 
    {4,5,0,0,0,0,0,0,0,
     0,0,2,0,7,0,6,3,0,
     0,0,0,0,0,0,0,2,8,
     0,0,0,9,5,0,0,0,0,
     0,8,6,0,0,0,2,0,0,
     0,2,0,6,0,0,7,5,0,
     0,0,0,0,0,0,4,7,6,
     0,7,0,0,4,5,0,0,0,
     0,0,8,0,0,9,0,0,0};

     array<int, 9*9> solved_sudoku_array = 
    {4,5,3,8,2,6,1,9,7,
     8,9,2,5,7,1,6,3,4,
     1,6,7,4,9,3,5,2,8,
     7,1,4,9,5,2,8,6,3,
     5,8,6,1,3,7,2,4,9,
     3,2,9,6,8,4,7,5,1,
     9,3,5,2,1,8,4,7,6,
     6,7,1,3,4,5,9,8,2,
     2,4,8,7,6,9,3,1,5};

     array<int, 9*9> starter_sudoku_array_one_removed = 
    {0,5,0,0,0,0,0,0,0,
     0,0,2,0,7,0,6,3,0,
     0,0,0,0,0,0,0,2,8,
     0,0,0,9,5,0,0,0,0,
     0,8,6,0,0,0,2,0,0,
     0,2,0,6,0,0,7,5,0,
     0,0,0,0,0,0,4,7,6,
     0,7,0,0,4,5,0,0,0,
     0,0,8,0,0,9,0,0,0};

     array<int, 9*9> testing_solver_horizontal_line_array = 
    {1,2,3,4,5,6,7,0,0,
     0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,9,8,
     0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,0};

     array<int, 9*9> testing_solver_vertical_line_array = 
    {1,0,0,0,0,0,0,0,0,
     2,0,0,0,0,0,0,0,0,
     3,0,0,0,0,0,0,0,0,
     4,0,0,0,0,0,0,0,0,
     5,0,0,0,0,0,0,0,0,
     6,0,0,0,0,0,0,0,0,
     7,0,0,0,0,0,0,0,0,
     0,0,0,8,0,0,0,0,0,
     0,0,0,9,0,0,0,0,0};
    
    
    cout << "Before filling in single possibilities: " << endl;
    Sudoku s = Sudoku(starter_sudoku_array);
    cout << s << endl;
    cout << "Consistent: " << s.CheckConsistent() << endl;
    cout << "Fully solved: " << s.CheckFullySolved() << endl;
    cout << "Possible: " << s.CheckPossible() << endl;

    auto start = chrono::system_clock::now();
    s.UpdatePossibilies();
    GuessList l = GuessList(s);
    pair<GuessList,Sudoku> p = ItterativeSolver(s,l);

    auto end = chrono::system_clock::now();
    auto elapsed = end - start;
    cout << elapsed.count() << '\n';

    cout << p.first << endl;
    cout << p.second << endl;

    return 0;
}