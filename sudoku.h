#ifndef SUDOKU_H
#define SUDOKU_H

#include "guess_list.h"

static const unordered_set<int> all_numbers={1,2,3,4,5,6,7,8,9};

class Sudoku{
    private:
        array<int,9*9> table;
        array<unordered_set<int>,9*9> possibilities;
        

    public:
        Sudoku(array<int,9*9> t){
            table=t;
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(table[i+9*j]!=0){
                        possibilities[i+9*j]={};
                    }else{
                        possibilities[i+9*j]=all_numbers;
                    }
                }
            }
        }


        array<int,9*9> GetTable(){return table;}

        // The first element is called column, the second one the row and the third one is the variable by which to change it.
        void ChangeElement(const int& i,const int& j,const int& k){table[i+9*j]=k;}
        int GetElement(const int& i,const int& j) const {return table[i+9*j];}
        unordered_set<int> GetPossibilities(const int& i,const int& j) const {return possibilities[i+9*j];}


        bool CheckConsistent(){
            //For all columns, for all numbers,
            // check if a number isn't included twice
            for(int i=0;i<9;i++){
                for(int n=1;n<10;n++){
                    int n_count = 0;
                    for(int j=0;j<9;j++){
                        if(GetElement(i,j) == n){n_count++;}
                    }
                    if(n_count>1){return false;}
                }
            }
            //Same thing for columns
            for(int i=0;i<9;i++){
                for(int n=1;n<10;n++){
                    int n_count = 0;
                    for(int j=0;j<9;j++){
                        if(GetElement(j,i) == n){n_count++;}
                    }
                    if(n_count>1){return false;}
                }
            }
            //Same thing for the blocks
            for(int i1=0;i1<9;i1+=3){
                for(int j1=0;j1<9;j1+=3){
                    for(int n=1;n<10;n++){
                        int n_count = 0;
                        for(int i2=0;i2<3;i2++){
                            for(int j2=0;j2<3;j2++){
                                if(GetElement(i1+i2,j1+j2)==n){n_count++;}
                            }
                        }
                        if(n_count>1){return false;}
                    }
                }
            }
            return true;
        }
        bool CheckFullySolved(){
            for(int i=0;i<9*9;i++){
                if(table[i] == 0){return false;}
            }
            return true;
        }

        void UpdatePossibilies(){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(GetElement(i,j)!=0){
                        possibilities[i+9*j].clear();
                    }else{
                        for(int n:possibilities[i+9*j]){
                            ChangeElement(i,j,n);
                            if(!CheckConsistent()){possibilities[i+9*j].erase(n);}
                            ChangeElement(i,j,0);
                        }
                    }
                }
            }
        }

        bool CheckPossible(){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(GetElement(i,j)==0 && possibilities[i+9*j].empty()){return false;}
                }
            }
            return true;
        }



        //This method fills in a number if it is the only possibility. The bool checks if something has changed using this method.
        bool FillSinglePossibility(){
            bool changed=false;
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(possibilities[i+9*j].size()==1){ChangeElement(i,j,*possibilities[i+9*j].begin());changed=true;}
                }
            }
            UpdatePossibilies();
            return changed;
        }

        ostream& Print(ostream& os) const{
            for(int i=0;i<9;i++){
                if(i%3==0){
                    for(int j=0;j<8*3+1;j++){os << "-";}
                    os << endl;
                }
                for(int j=0;j<9;j++){
                    if(j%3==0){os << "| ";}
                    os << table[9*i+j] << " ";
                }
                os << "|" << endl;
            }
            for(int j=0;j<8*3+1;j++){os << "-";}
                    os << endl;
            return os;
        }    

        void Sudoku::InsertGuesslist( GuessList& g){
            for(GuessList::Triple& t:g.guesses){
                Sudoku::ChangeElement(t.firstX,t.firstY,t.second);
            }
        }

    
};

// Operator overload that allows stream output syntax
ostream& operator<<(ostream& os, const Sudoku& sud) {
    return sud.Print(os);
}


GuessList::GuessList(const Sudoku& s){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(s.GetElement(i,j)==0){
                unordered_set<int> a = s.GetPossibilities(i,j);
                Triple t=Triple(i,j,0,a);
                guesses.push_back(t);
            }
        }
    }
}

#endif