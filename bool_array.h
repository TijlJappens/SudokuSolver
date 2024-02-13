static const array<bool,9> all_true={true};
static const array<bool,9> all_false={false};

class BoolArray{
    public:
    array<bool,9> bool_array;
    int amount_true;
    BoolArray(bool b){
        if(b==true){bool_array=all_true;amount_true=9;}
        else{bool_array=all_false;amount_true=0;}
    }

    int FirstTrue() const{
        for(int i=0;i<9;i++){
            if(bool_array[i]==true){return i+1;}
        }
    }

    bool OnlyOneTrue() const{
        if(amount_true==1){return true;}
        else{return false;}
    }

    bool GetBool(const int& i){
        return bool_array[i-1];
    }

    void SetFalse(const int& i){
        if(bool_array[i-1]==true){bool_array[i-1]=false;amount_true--;}
    }
};