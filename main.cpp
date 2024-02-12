#include <iostream>
#include <array>
#include <vector>
#include "unordered_set"
#include "sudoku.h"


using namespace std;
static int moves=0;

GuessList ItterativeSolver(Sudoku&s ,GuessList l){
    Sudoku k = Sudoku(s);
    while(true){
        k=s;
        k.InsertGuesslist(l);
        k.UpdatePossibilies();
        while(k.FillSinglePossibility()==true);

        bool solved = k.CheckFullySolved();
        bool consistent = k.CheckConsistent();
        bool possible = k.CheckPossible();

        if(solved && consistent){
            cout << "Solved and consistent." << endl;
            return l;
        }else if(!consistent || !possible){
            cout << "Changing guess" << endl;
            l.ChangeWrongGuess();
        }else if(!solved){
            cout << "Specifying guess" << endl;
            l.SpecifyGuess();
        }
    }
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
    
    /*Sudoku starter_sudoku = Sudoku(starter_sudoku_array);
    cout << starter_sudoku << endl;
    cout << "Consistent: " << starter_sudoku.CheckConsistent() << endl;
    cout << "Fully solved: " << starter_sudoku.CheckFullySolved() << endl;

    Sudoku solved_sudoku = Sudoku(solved_sudoku_array);
    cout << solved_sudoku << endl;
    cout << "Consistent: " << solved_sudoku.CheckConsistent() << endl;
    cout << "Fully solved: " << solved_sudoku.CheckFullySolved() << endl;
    */
    cout << "Before filling in single possibilities: " << endl;
    Sudoku s = Sudoku(starter_sudoku_array_one_removed);
    cout << s << endl;
    cout << "Consistent: " << s.CheckConsistent() << endl;
    cout << "Fully solved: " << s.CheckFullySolved() << endl;
    cout << "Possible: " << s.CheckPossible() << endl;
    s.UpdatePossibilies();
    GuessList l = GuessList(s);
    l.SpecifyGuess();
    GuessList l1 = ItterativeSolver(s,l);

    s.InsertGuesslist(l1);
    s.UpdatePossibilies();
    while(s.FillSinglePossibility()==true);
    cout << s << endl;

    /*
    cout << "Consistent: " << k.CheckConsistent() << endl;
    cout << "Fully solved: " << k.CheckFullySolved() << endl;
    cout << "Possible: " << k.CheckPossible() << endl;
    */
    return 0;
}