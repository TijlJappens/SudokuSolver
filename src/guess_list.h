#ifndef GUESS_LIST_H
#define GUESS_LIST_H

using namespace std;

class Sudoku;

class GuessList{
    public:
    class Triple{
        public:
            Triple(int i, int j, int k, BoolArray l){
                firstX=i; firstY=j; second=k; third=l;
        }
        bool IsContained(){
            if(third.GetBool(second)==true){return true;}
            else{return false;}
        }
        int GetLowest(){
            return third.FirstTrue();
        }
        int firstX;
        int firstY;
        uint8_t second;
        BoolArray third;
    };

    public:
        GuessList(const Sudoku& s);

        int GetFirstZeroIndex(){
            for(int i=0;i<guesses.size();i++){
                if(guesses[i].second == 0){return i;}
            }
            cout << "No zeros" << endl;
            return -1;
        }

        void SpecifyGuess(){
            int to_be_specified=GetFirstZeroIndex();
            guesses[to_be_specified].second=guesses[to_be_specified].GetLowest();}

        void ChangeWrongGuess(){
            int i=GetFirstZeroIndex()-1;
            //cout << "Index of change wrong guess: " << i << endl;
            guesses[i].second++;
            while(guesses[i].second<=9){
                if(guesses[i].IsContained()){return;}
                guesses[i].second++;
            }
            guesses[i].second=0;
            ChangeWrongGuess();
        }

        ostream& Print(ostream& os) const{
            os << "| ";
            for(Triple t:guesses){
                os << unsigned(t.second) << " |";
            }
            return os;
        }

        vector<Triple> guesses;

};

// Operator overload that allows stream output syntax
ostream& operator<<(ostream& os, const GuessList& l) {
    return l.Print(os);
}

#endif