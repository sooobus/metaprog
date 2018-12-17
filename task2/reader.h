#include "typelist.cpp"
#include <iostream>
#include <fstream>
template <typename T> class debug;

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
        void* ptr = static_cast<char*>(malloc(memsize));
	std::cout << static_cast<void*>(ptr) << " " << memsize <<  std::endl;
	readNextElement<typename get_nth<TL, 0>::res, 0>(reinterpret_cast<typename get_nth<TL, 0>::res*>(ptr));
	return reinterpret_cast<void*>(ptr);
    }

private:

    template <typename T, const size_t N>
    void readNextElement(T* ptr){
	auto conv_ptr = reinterpret_cast<typename get_nth<TL, N>::res*>(ptr);
        input_file >> *conv_ptr;
        std::cout << *conv_ptr;

	if(N < num_elements - 1){
	    readNextElement<typename get_nth<TL, N + 1>::res, N + 1>(reinterpret_cast<typename get_nth<TL, N + 1>::res*>(conv_ptr + 1));
	}
    }
};
