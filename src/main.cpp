#include <iostream>
#include <array>
#include <vector>
#include <chrono>

#include <fstream>
#include <string>

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
            moves++;
            //cout << "Changing guess" << endl;
            p.first.ChangeWrongGuess();
        }else if(!solved){
            moves++;
            //cout << "Specifying guess" << endl;
            p.first.SpecifyGuess();
        }
    }
    throw runtime_error("Cannot solve sudoku.");
    return p;
}

Sudoku sudoku_reader(int argc, char** argv){
        if(argc==1){throw runtime_error("No path to sudoku given.");}
    
    string path = argv[1];

    array<uint8_t, 9*9> a;
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
                a[i] = (int) (mychar-'0');
                i++;
            }
        }
    }
    myfile.close();
    
    
    std::cout << "Before filling in single possibilities: " << endl;
    return Sudoku(a);
}

int main(int argc, char **argv)
{
    Sudoku s = sudoku_reader(argc,argv);
    std::cout << s << endl;
    std::cout << "Consistent: " << s.CheckConsistent() << endl;
    std::cout << "Fully solved: " << s.CheckFullySolved() << endl;
    std::cout << "Possible: " << s.CheckPossible() << endl;

    auto start = chrono::system_clock::now();
    s.UpdatePossibilies();
    GuessList l = GuessList(s);
    pair<GuessList,Sudoku> p = ItterativeSolver(s,l);

    auto end = chrono::system_clock::now();
    auto elapsed = end - start;
    std::cout << "It took " << elapsed.count() << " micro seconds." << endl;
    std::cout << "It took " << moves << " guesses." << endl;

    std::cout << p.first << endl;
    std::cout << p.second << endl;
    
    return 0;
}