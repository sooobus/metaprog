#include "reader.h"
#include <iostream>
#include <cassert>

Reader<create_list<int, int, float, int>> r;

bool test_ReadNextLine(){
    void* ptr = r.readNextLine();
    if(ptr == nullptr)
        return false;
    char* work_ptr = reinterpret_cast<char*>(ptr);
    int* x1_ptr = reinterpret_cast<int*>(work_ptr);
    work_ptr += sizeof(int);
    int* x2_ptr = reinterpret_cast<int*>(work_ptr);
    work_ptr += sizeof(int);;
    float* x3_ptr = reinterpret_cast<float*>(work_ptr);
    work_ptr += sizeof(float);
    int* x4_ptr = reinterpret_cast<int*>(work_ptr);
    work_ptr += sizeof(int);
    std::cout << *x1_ptr << " " << *x2_ptr << " " << *x3_ptr << " " << *x4_ptr << std::endl;
    return true;
}

int main(){
    assert(test_ReadNextLine());
    assert(test_ReadNextLine());
    assert(test_ReadNextLine());
    assert(!test_ReadNextLine());
}
