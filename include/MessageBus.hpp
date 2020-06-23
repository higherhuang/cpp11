#ifndef __MESSAGEBUS_HPP__
#define __MESSAGEBUS_HPP__

/************************************************************************/
/* INCLUDE                                                              */
/************************************************************************/
#include <string>
#include <functional>
#include <map>
#include "NonCopyable.hpp"
#include "function_traits.hpp"
#include "Any.hpp"




class MessageBus : public NonCopyable
{
public:
	//注册消息
	template<typename F>
	void Attach(F&& f, const std::string& strTopic = "")
	{
		auto func = to_function(std::forward<F>(f));
		Add(strTopic, std::move(func));
	}

	//发送消息
	template<typename R>
	void SendReq(const std::string& strTopic = "")
	{
		std::string strMsgType = strTopic + typeid(std::function<R()>).name();
		auto range = m_map.equal_range(strMsgType);
		for (auto& it = range.first; it != range.second; it++)
		{
			auto f = it->second.AnyCast<std::function<R()>>();
			f();
		}
	}

	//发送消息
	template<typename R, typename... Args>
	void SendReq(Args&&... args,const std::string& strTopic = "")
	{
		std::string strMsgType = strTopic + typeid(std::function<R(Args...)>).name();
		auto range = m_map.equal_range(strMsgType);
		for (auto& it = range.first; it != range.second; it++)
		{
			auto f = it->second.AnyCast<std::function<R(Args...)>>();
			f(std::forward<Args>(args)...);
		}
	}

	//移除某个主题，需要主题和消息类型
	template<typename R, typename... Args>
	void Remove(const std::string& strTopic = "")
	{
		std::string strMsgType = strTopic + typeid(std::function<R(Args...)>).name();
		auto range = m_map.equal_range(strMsgType);
		m_map.erase(range.first, range.second);
	}

private:
	template<typename F>
	void Add(const std::string& strTopic, F&& f)
	{
		std::string strMsgType = strTopic + typeid(F).name();
		m_map.emplace(std::move(strMsgType), std::forward<F>(f));
	}

private:
	std::multimap<std::string, Any> m_map;
};






#endif //__MESSAGEBUS_HPP__
