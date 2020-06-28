#ifndef __ANY_HPP__
#define __ANY_HPP__

#include <typeindex>
#include <memory>
#include <string>
#include <exception>


class Any
{
public:
	Any(void) : m_tpIndex(std::type_index(typeid(void))) {}
	Any(const Any& that) : m_ptr(that.Clone()), m_tpIndex(that.m_tpIndex){}
	Any(Any&& that) : m_ptr(std::move(that.m_ptr)), m_tpIndex(that.m_tpIndex)
	{
		that.m_tpIndex = std::type_index(typeid(void));
	}
	template<typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value>::type>
	Any(U&& value) : m_ptr(new Derived<typename std::decay<U>::type>(std::forward<U>(value))), m_tpIndex(std::type_index(typeid(std::decay<U>::type))) {}

	bool IsNull() const { return !bool(m_ptr); }

	template<typename U>
	bool Is() const
	{
		return m_tpIndex == std::type_index(typeid(U));
	}

	template<typename U>
	U& AnyCast()
	{
		if (!Is<U>())
		{
			std::string str = std::string("Any::AnyCast can not cast [") + typeid(U).name() + "] to [" + m_tpIndex.name() + "]";
			throw std::logic_error(str.c_str());
		}
		auto derived = dynamic_cast<Derived<U>*>(m_ptr.get());
		return derived->m_value;
	}
	//¸³Öµ
	Any& operator=(const Any& a)
	{
		if (m_ptr == a.m_ptr) return *this;
		m_ptr = a.Clone();
		m_tpIndex = a.m_tpIndex;
		return *this;
	}
	//ÒÆ¶¯¸³Öµ
	Any& operator=(Any&& a)
	{
		if (m_ptr == a.m_ptr) return *this;
		m_ptr = std::move(a.m_ptr);
		m_tpIndex = a.m_tpIndex;
		return *this;
	}

private:
	class Base;
	using BasePtr = std::unique_ptr<Base>;

	class Base
	{
	public:
		virtual ~Base() {}
		virtual BasePtr Clone() const = 0;
	};

	template<typename T>
	class Derived : public Base
	{
	public:
		template<typename U>
		Derived(U&& value) : m_value(std::forward<U>(value)){}

		virtual BasePtr Clone() const
		{
			return BasePtr(new Derived<T>(m_value));
		}

		T m_value;
	};

	BasePtr Clone() const
	{
		if (nullptr != m_ptr) return m_ptr->Clone();
		return nullptr;
	}

	BasePtr m_ptr;
	std::type_index m_tpIndex;
};




#endif //__ANY_HPP__
