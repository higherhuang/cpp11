#ifndef __IOCCONTAINER_H__
#define __IOCCONTAINER_H__



#include <sstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>
#include "AnyVar.h"
#include "NonCopy.h"
#include "Singleton.h"



class IocContainer : public NonCopy
{
public:
	IocContainer() {}
	~IocContainer() {}

	template<class T, typename Depend, typename... Args>
	typename std::enable_if<!std::is_base_of<T, Depend>::value>::type RegisterType(const std::string& strKey)
	{
		std::function<T* (Args...)> function = [](Args... args) {
			return new T(new Depend(args...));
		};

		RegisterType(strKey, function);
	}

	template<class T, typename Depend, typename... Args>
	typename std::enable_if<std::is_base_of<T, Depend>::value>::type RegisterType(const std::string& strKey)
	{
		std::function<T* (Args...)> function = [](Args... args) {
			return new Depend(args...);
		};

		RegisterType(strKey, function);
	}

	template<class T, typename... Args>
	T* Resolve(const std::string& strKey, Args... args)
	{
		auto iter = m_creatorMap.find(strKey);
		if (m_creatorMap.end() == iter)
		{
			return NULL;
		}

		AnyVar resolver = iter->second;
		std::function<T* (Args...)> function = any_cast<std::function<T* (Args...)>>(resolver);
		return function(args...);
	}

	template<class T, typename... Args>
	std::shared_ptr<T> ResolveShared(const std::string& strKey, Args... args)
	{
		T* tmp = Resolve<T, Args...>(strKey, args...);
		return std::shared_ptr<T>(tmp);
	}

private:
	void RegisterType(const std::string& strKey, AnyVar constructor)
	{
		if (m_creatorMap.end() != m_creatorMap.find(strKey))
		{
			throw std::invalid_argument(strKey + " : this key has already exist!");
		} 
		else
		{
			m_creatorMap.emplace(strKey, constructor);
		}
	}

private:
	std::unordered_map<std::string, AnyVar> m_creatorMap;
};



typedef Singleton<IocContainer> IocContainerAgent;

#endif // !__IOCCONTAINER_H__
