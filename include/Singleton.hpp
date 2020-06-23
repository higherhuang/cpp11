#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#include "NonCopyable.hpp"


template<typename T>
class Singleton : public NonCopyable
{
public:
	template<typename... Args>
	static T* Instance(Args&&... args)
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new T(std::forward<Args>(args)...);
		}

		return m_pInstance;
	}

	static T* GetInstance()
	{
		if (nullptr == m_pInstance)
		{
			throw std::logic_error("the instance is not init, please initialize the instance first");
		}

		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (nullptr != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static T* m_pInstance;
};


template<typename T>
T* Singleton<T>::m_pInstance = nullptr;



#endif //__SINGLETON_HPP__
