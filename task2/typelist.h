// TypeList implementation. Functionality:

// Add type
// Remove type
// Get N-th type
// Get length
// Get sizeof

#include <cstddef>

// Null type terminates list.
struct NullType{};

// Core structure for the type list.
template <typename H, typename T>
struct my_typelist{
	using head = H;
	using tail = T;
};

// Accessor for safe removal.
template <typename TL>
struct list_accessor;

template <typename H, typename T>
struct list_accessor<my_typelist<H, T>>{
    using head = H;
    using tail = T;
};

template<>
struct list_accessor<NullType>{
    using head = NullType;
    using tail = NullType;
};

// List creation in variadic template style.
template <typename ... Ts> struct create_next;

template <typename T, typename ... L>
struct create_next<T, L...> { 
    using type = my_typelist<T, typename create_next<L...>::type>;
};

template <>
struct create_next<> { using type = NullType; };

template <typename ... Ts>
using create_list = typename create_next<Ts...>::type;

// Add op.
template <typename L, typename T>
using add = my_typelist<T, L>;

// Remove op.
template <typename TL>
using remove_el = typename list_accessor<TL>::tail;

// Get Nth op.
template <typename TL, size_t N>
struct get_nth{
    using res = typename get_nth<typename list_accessor<TL>::tail, N - 1>::res;
};

template <typename TL>
struct get_nth<TL, 0>{ 
    using res = typename list_accessor<TL>::head;
};

// Length op.
template <typename TL> struct length;

template <>
struct length<NullType>
{
    static const size_t res = 0;
};

template <typename T, typename L>
struct length<my_typelist<T, L>>
{
    static const size_t res = 1 + length<L>::res;
};

// Sizeof op.
template <typename TL> struct size;

template <>
struct size<NullType>
{
    static const size_t res = 0;
};

template <typename T, typename L>
struct size<my_typelist<T, L>>
{
    static const size_t res = sizeof(T) + size<L>::res;
};



