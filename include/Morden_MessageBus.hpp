#ifndef __MORDEN_MESSAGEBUS_HPP__
#define __MORDEN_MESSAGEBUS_HPP__
#include <assert.h>
#include <string>
#include <map>
#include <functional>
#include "function_traits.hpp"

class MordernMessageBus
{
public:
	template<typename Function>
	void register_handler(std::string const & name, Function const & f)
	{
		this->invokers_[name] = std::bind(&invoker<Function>::apply, f, std::placeholders::_1, std::placeholders::_2);
	}

	template<typename T, typename ... Args>
	T call(const std::string& name, Args&& ... args)
	{
		const auto it = invokers_.find(name);
		if (invokers_.end() == it) return{};
		
		//断言返回值
		//using rt_type = typename function_traits<decltype(it->second)>::return_type;
		//bool rslt = std::is_same<rt_type, T>::value;
		//assert(rslt);
		

		auto args_tuple = std::make_tuple(std::forward<Args>(args)...);
		T t;
		it->second(&args_tuple, &t);
		return t;
	}
	template<typename ... Args>
	void call_void(const std::string& name, Args&& ... args)
	{
		const auto it = invokers_.find(name);
		if (invokers_.end() == it) return;
		
		auto args_tuple = std::make_tuple(std::forward<Args>(args)...);
		it->second(&args_tuple, nullptr);
	}

private:
	template<typename Function>
	struct invoker
	{
		static inline void apply(const Function  & func, void* bl, void* result)
		{
			using tuple_type = function_traits<Function>::tuple_type;
			const tuple_type* tp = static_cast<tuple_type*>(bl);
			call(func, *tp, result);
		}

		template<typename F, typename... Args>
		static typename std::enable_if<std::is_void<typename std::result_of<F(Args...)>::type>::value>::type call(const F  & f, const std::tuple<Args...>&  tp, void*)
		{
			call_helper(f, std::make_index_sequence<sizeof...(Args)>(), tp);
		}

		template<typename F, typename... Args>
		static typename std::enable_if<!std::is_void<typename std::result_of<F(Args...)>::type>::value>::type call(F const & f, std::tuple<Args...> const &  tp, void* result)
		{
			auto rslt = call_helper(f, std::make_index_sequence<sizeof...(Args)>(), tp);
			*(decltype(rslt)*)result = rslt;
		}


		template<typename F, size_t... I, typename... Args>
		static auto call_helper(const F  & f, const std::index_sequence<I...>  &, const std::tuple<Args...>  & tup)
		{
			//断言参数个数类型一致性
			//using tuple_type = std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
			using tuple_type = std::tuple<std::decay_t<Args>...>;
			std::string str1 = typeid(tuple_type).name();
			std::string str2 = typeid(typename function_traits<decltype(f)>::tuple_type).name();
			bool bParam = std::is_same<typename function_traits<decltype(f)>::tuple_type, tuple_type>::value;
			assert(bParam);
			return f(std::get<I>(tup)...);
		}
	};
private:
	std::map<std::string, std::function<void(void*, void*)>> invokers_;
};

#endif//__MORDEN_MESSAGEBUS_HPP__
