

template <class Iterator, typename T>
void _sift_fix(Iterator first, int top, int hole, const T& value){
    int parent=(hole-1)/2; 
    while(parent>top && value>*(first+hole)){
        *(first+hole)=*(first+parent);
        hole=parent;
        parent=(hole-1)/2;
    }
    *(first+hole)=value;
}

template <class Iterator, typename T>
void _sink_fix(Iterator first, int len, int hole, const T& value){
    int bigChild=2*hole+1;
    while(bigChild<len)
    if(*(first+bigChild+1)>*(first+bigChild)){
        bigChild++;
    }
    if(*(first+bigChild)>*(first+hole)){
        *(first+hole)=*(first+bigChild);
        hole=bigChild;
        bigChild=2*hole+1;
    }
}
