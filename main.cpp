#include <iostream>
#include <array>
#include <vector>
#include <chrono>

#include <fstream>
#include <string>
#include <queue>
#include <thread>
#include "thread_pool.h"

using namespace std;

#include "sudoku.h"




static int moves=0;
static const short int amount_of_threads=10;

void ItterativeSolverSingleThread(GuessList* g,Sudoku* s,bool* solved,bool* consistent,bool* possible){
    s->InsertGuesslist(*g);
    s->UpdatePossibilies();
    while(s->FillSinglePossibility()==true);

    *solved = s->CheckFullySolved();
    *consistent = s->CheckConsistent();
    *possible = s->CheckPossible();

}

pair<GuessList,Sudoku> ItterativeSolver(Sudoku&s ,GuessList& l){

    
    queue<GuessList> ToBeChecked;
    ToBeChecked.push(l);

    array<thread*,amount_of_threads> t={nullptr};
    array<GuessList*,amount_of_threads> results_guesses={nullptr};
    array<Sudoku*,amount_of_threads> results_sudokus={nullptr};
    array<bool*,amount_of_threads> solved={nullptr};
    array<bool*,amount_of_threads> consistent={nullptr};
    array<bool*,amount_of_threads> possible={nullptr};

    while(true){
        for(int i=0;i<amount_of_threads;i++){
            if(ToBeChecked.size()>=1){
                results_guesses[i] = new GuessList(ToBeChecked.front());
                ToBeChecked.pop();
                results_sudokus[i] = new Sudoku(s);
                solved[i]=new bool(false);
                consistent[i]= new bool(false);
                possible[i]=new bool(false);
                t[i] = new thread(ItterativeSolverSingleThread,results_guesses[i],results_sudokus[i],solved[i],consistent[i],possible[i]);
            }else{
                t[i]=nullptr;
                results_guesses[i]=nullptr;
                results_sudokus[i]=nullptr;
            }
        }
        for(int i=0;i<amount_of_threads;i++){
            if(t[i]!=nullptr){
                t[i]->join();
                delete t[i];
                t[i]=nullptr;
            }
        }

        for(int i=0;i<amount_of_threads;i++){
            if(results_guesses[i]!=nullptr&&results_sudokus[i]!=nullptr){
                if(*solved[i]&&*consistent[i]){
                    pair<GuessList,Sudoku> p(*results_guesses[i],*results_sudokus[i]);
                    return p;
                }else if(!(*consistent[i] && *possible[i])){
                    moves++;
                }else if(!*solved[i]){
                    moves++;
                    //cout << "Specifying guess" << endl;
                    //cout << *results_guesses[i] << endl;
                    results_guesses[i]->SpecifyGuessQueue(ToBeChecked);
                    
                }
            }
            
        }
        
        for(int i=0;i<amount_of_threads;i++){
            if(results_guesses[i]!=nullptr){delete results_guesses[i]; results_guesses[i]=nullptr;}
            if(results_sudokus[i]!=nullptr){delete results_sudokus[i]; results_sudokus[i]=nullptr;}
            if(solved[i]!=nullptr){delete solved[i]; solved[i]=nullptr;}
            if(consistent[i]!=nullptr){delete consistent[i]; consistent[i]=nullptr;}
            if(possible[i]!=nullptr){delete possible[i]; possible[i]=nullptr;}
        }
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
    pair<GuessList,Sudoku> p = ItterativeSolver(s,l);

    auto end = chrono::system_clock::now();
    auto elapsed = end - start;
    std::cout << "It took " << elapsed.count() << " micro seconds." << endl;
    std::cout << "It took " << moves << " guesses." << endl;

    std::cout << p.first << endl;
    std::cout << p.second << endl;
    
    return 0;
}