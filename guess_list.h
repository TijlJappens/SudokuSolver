#ifndef GUESS_LIST_H
#define GUESS_LIST_H

using namespace std;

class Sudoku;

class GuessList{
    public:
    class Triple{
        public:
            Triple(int i, int j, int k, unordered_set<int> l){
                firstX=i; firstY=j; second=k; third=l;
        }
        bool IsContained(){
            if(third.count(second)==1){return true;}
            else{return false;}
        }
        int GetLowest(){
            for(int i=0;i<10;i++){
                if(third.count(i)==1){return i;}
            }
            return -1;
        }
        int firstX;
        int firstY;
        int second;
        unordered_set<int> third;
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
                os << t.second << " |";
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