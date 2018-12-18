#include "typelist.cpp"
#include <iostream>
#include <fstream>

//template <typename T> class debug;

template <typename TL> class Reader{

    static const size_t memsize = size<TL>::res;		
    static const size_t num_elements = length<TL>::res;
    std::ifstream input_file;

public:

    Reader(){
	    input_file.open("input.txt");
  	    if (!input_file.is_open()){
            std::cout << "Unable to open file" << std::endl;
  	    }
    }

    ~Reader(){
        input_file.close();
    }

    void* readNextLine(){
        char* ptr = static_cast<char*>(malloc(memsize));
        print<TL>(input_file, ptr);
        
        if(input_file.peek() == EOF){
            return nullptr;
        }
 
        return reinterpret_cast<void*>(ptr);
    }
};
