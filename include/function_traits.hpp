#ifndef __FUNCTION_TRAITS_HPP__
#define __FUNCTION_TRAITS_HPP__


#include <functional>
#include <tuple>
#include <iostream>



//declare
template<typename T>
struct function_traits;


//normal function
template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
{
public:
	enum { arity = sizeof...(Args) };
	typedef Ret function_type(Args...);
	typedef Ret return_type;
	using stl_function_type = std::function<function_type>;
	typedef Ret(*pointer)(Args...);

	template<size_t I>
	struct args
	{
		static_assert(I < arity, "index is out of range, index must less than sizeof Args");
		using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
	};

	typedef std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...> tuple_type;
	typedef std::tuple<std::remove_const_t<std::remove_reference_t<Args>>...> bare_tuple_type;
};


//function pointer
template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> : function_traits<Ret(Args...)> {};

//std::function
template<typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>> : function_traits<Ret(Args...)> {};


//member function
#define FUNCTION_TRAITS(...)\
template<typename Ret, typename ClassType, typename... Args>\
struct function_traits<Ret(ClassType::*)(Args...) __VA_ARGS__> : function_traits<Ret(Args...)> {};
//normal member function
FUNCTION_TRAITS()
//const member function
FUNCTION_TRAITS(const)
//volatile member function
FUNCTION_TRAITS(volatile)
//const volatile member function
FUNCTION_TRAITS(const volatile)

//callable object
template<typename Callable>
struct function_traits : function_traits<decltype(&Callable::operator())> {};
template<typename Callable>
struct function_traits<Callable&> : function_traits<Callable> {};
template<typename Callable>
struct function_traits<const Callable&> : function_traits<Callable> {};
template<typename Callable>
struct function_traits<Callable&&> : function_traits<Callable> {};
template<typename Callable>
struct function_traits<Callable*> : function_traits<Callable> {};
template<typename Callable>
struct function_traits<const Callable*> : function_traits<Callable> {};



template <typename Function>
typename function_traits<Function>::stl_function_type to_function(const Function& lambda)
{
	return static_cast<function_traits<Function>::stl_function_type>(lambda);
}


template <typename Function>
typename function_traits<Function>::stl_function_type to_function(Function&& lambda)
{
	return static_cast<function_traits<Function>::stl_function_type>(std::forward<Function>(lambda));
}

template <typename Function>
typename function_traits<Function>::pointer to_function_pointer(const Function& lambda)
{
	return static_cast<function_traits<Function>::pointer>(std::forward<Function>(lambda));
}


#endif //__FUNCTION_TRAITS_HPP__
