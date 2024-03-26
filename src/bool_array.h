array<bool,9> allSameGenerator(bool b){
    if(b){
        return {true,true,true,true,true,true,true,true,true};
    }else{
        return {false,false,false,false,false,false,false,false,false};
    }
}

class BoolArray{
    public:
    array<bool,9> bool_array;
    int amount_true;
    
    BoolArray(bool b) : bool_array(allSameGenerator(b)), amount_true(b*9){}
    BoolArray() : BoolArray(false){}

    int FirstTrue() const{
        for(int i=0;i<9;i++){
            if(bool_array[i]==true){return i+1;}
        }
        throw runtime_error("Accessing first true element of an array that is all false.");
    }

    bool NoneTrue() const {
        if(amount_true==0){return true;}
        else{return false;}
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

    void SetAllFalse(){
        if(amount_true>0){bool_array.fill(false);amount_true=0;}
    }
};