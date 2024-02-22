#ifndef SUDOKU_H
#define SUDOKU_H

#include "bool_array.h"
#include "guess_list.h"


class Sudoku{
    private:
        array<short int,9*9> table;
        array<BoolArray,9*9> possibilities;
        

    public:
        Sudoku(array<short int,9*9> t){
            table=t;
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(table[i+9*j]!=0){
                        BoolArray b = BoolArray(false);
                        possibilities[i+9*j]=b;
                    }else{
                        BoolArray b = BoolArray(true);
                        possibilities[i+9*j]=b;
                    }
                }
            }
        }


        array<short int,9*9> GetTable(){return table;}

        // The first element is called column, the second one the row and the third one is the variable by which to change it.
        void ChangeElement(const int& i,const int& j,const short int& k){table[i+9*j]=k;}
        short int GetElement(const int& i,const int& j) const {return table[i+9*j];}
        BoolArray GetPossibilities(const int& i,const int& j) const {return possibilities[i+9*j];}

        //Checks if the table is consistent.
        bool CheckConsistent(){
            //For all columns, rows and squares, for all numbers,
            // check if a number isn't included twice
            for(int i=0;i<9;i++){
                for(short int n=1;n<10;n++){
                    int n_count_row = 0;
                    int n_count_col = 0;
                    int n_count_square = 0;
                    for(int j=0;j<9;j++){
                        //This checks columns.
                        if(GetElement(i,j) == n){n_count_row++;}
                        //This checks rows.
                        if(GetElement(j,i) == n){n_count_col++;}
                        //This checks squares.
                        if(GetElement((i*3)%9+j%3,3*((i*3)/9)+j/3) == n){n_count_square++;}
                    }
                    if(n_count_row>1 || n_count_col>1 || n_count_square>1){return false;}
                }
            }
            return true;
        }

        //Checks if inserting a (single) new element doesn't create additional inconsistencies.
        bool CheckConsistentSinglePosition(int x,int y){
            int n = GetElement(x,y);
            int n_count_row = 0;
            int n_count_col = 0;
            int n_count_square = 0;
            for(int j=0;j<9;j++){
                if(GetElement(x,j) == n){n_count_row++;}
                if(GetElement(j,y) == n){n_count_col++;}
                int i = (x/3+3*(y/3))*3;
                if(GetElement(i%9+j%3,3*(i/9)+j/3) == n){n_count_square++;}
            }
            if(n_count_row>1 || n_count_col>1 || n_count_square>1){return false;}
            return true;
        }

        bool CheckFullySolved(){
            for(int i=0;i<9*9;i++){
                if(table[i] == 0){return false;}
            }
            return true;
        }

        void UpdateSinglePossibility(const int& i,const int& j){
            if(GetElement(i,j)!=0){
                possibilities[i+9*j].SetAllFalse();
            }else{
                for(short int n=1;n<=9;n++){
                    if(possibilities[i+9*j].GetBool(n)==true){
                        ChangeElement(i,j,n);
                        if(!CheckConsistentSinglePosition(i,j)){possibilities[i+9*j].SetFalse(n);}
                        ChangeElement(i,j,0); 
                    }
                }
            }
        }

        void UpdatePossibilies(){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    UpdateSinglePossibility(i,j);
                }
            }
        }

        bool CheckPossible(){
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(GetElement(i,j)==0 && possibilities[i+9*j].NoneTrue()){return false;}
                }
            }
            return true;
        }
        /*
        bool CheckIfOnlyInsertionPlaceInColumnRowOrSquare(const int& i,const int& j, const int& n){
            for(int k=0;k<9;k++){
                if(i+9*j!=k+9*j && possibilities[k+9*j].GetBool(n)==true){
                    return false;
                }
                if(i+9*j!=i+9*k && possibilities[i+9*k].GetBool(n)==true){
                    return false;
                }
                int l = (i/3)*3+(j/3)*3*9;
                if(l+k%3+9*(k/3)!=i+9*j && possibilities[l+k%3+9*(k/3)].GetBool(n)==true){
                    return false;
                }
            }
            return true;
        }*/

        //This method fills in a number if it is the only possibility and then inserts it.
        //
        //The bool checks if something has changed using this method.
        bool FillSinglePossibility(){
            bool changed=false;
            for(int i=0;i<9;i++){
                for(int j=0;j<9;j++){
                    if(GetElement(i,j)==0){
                        if(possibilities[i+9*j].OnlyOneTrue()==true){ChangeElement(i,j,possibilities[i+9*j].FirstTrue());changed=true;}
                        else if(possibilities[i+9*j].NoneTrue()==true){return false;}
                        /*else{
                            for(int n=1;n<=9;n++){
                                if(possibilities[i+9*j].GetBool(n)==true){
                                    if(CheckIfOnlyInsertionPlaceInColumnRowOrSquare(i,j,n)){ChangeElement(i,j,n);changed=true;}
                                }
                            }
                        }*/
                    }
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
    for(short int i=0;i<9;i++){
        for(short int j=0;j<9;j++){
            if(s.GetElement(i,j)==0){
                BoolArray a = s.GetPossibilities(i,j);
                Triple t=Triple(i,j,0,a);
                guesses.push_back(t);
            }
        }
    }
}

#endif