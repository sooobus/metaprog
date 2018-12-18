#include <iostream>
#include "typelist.cpp"

template<typename T> class debug;

// Test list creation.
using f_l = create_list<size_t, char, int>;
using my_add_l = add<f_l, float>;
using my_pre_l = remove_el<my_add_l>;
using my_l  = typename get_nth<my_pre_l,2 >::res;

int main(){

	const int l = size<my_pre_l>::res;
	std::cout << sizeof(size_t) << " + " << sizeof(char) << " + " << sizeof(int) << " + " << l;

	//debug<my_l> t;
}
