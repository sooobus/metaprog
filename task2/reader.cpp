#include "reader.h"
#include <iostream>

int main(){

    Reader<create_list<int, int, float, int>> r;
    void* ptr = r.readNextLine();
    std::cout << ptr << std::endl;
    char* work_ptr = reinterpret_cast<char*>(ptr);
    int* x1_ptr = reinterpret_cast<int*>(work_ptr);
    work_ptr += 1;
    int* x2_ptr = reinterpret_cast<int*>(work_ptr);
    work_ptr += 1;
    float* x3_ptr = reinterpret_cast<float*>(work_ptr);
    work_ptr += 1;
    int* x4_ptr = reinterpret_cast<int*>(work_ptr);
    work_ptr += 1;
   
    std::cout << *x1_ptr << " " << *x2_ptr << " " << *x3_ptr << " " << *x4_ptr << std::endl;


}
