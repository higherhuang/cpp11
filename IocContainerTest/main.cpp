#include <iostream>
#include <string>
#include "IocContainer.hpp"


class Base
{
public:
	Base(const std::string& str, int n, int* p) : m_str(str) {}
	virtual ~Base(){}

	virtual void DoJob(const std::string& str) = 0;

protected:
	std::string m_str;
};

class Derived_0 : public Base
{
public:
	using Base::Base;

	virtual void DoJob(const std::string& str) override
	{
		std::cout << "Derived_0::DoJob called." << std::endl;
		std::cout << m_str << std::endl;
		std::cout << str << std::endl;
	}
};

class Derived_1 : public Base
{
public:
	using Base::Base;

	virtual void DoJob(const std::string& str) override
	{
		std::cout << "Derived_1::DoJob called." << std::endl;
		std::cout << m_str << std::endl;
		std::cout << str << std::endl;
	}
};


void InitIocContainer()
{
	//初始化单例对象
	IocContainerAgent::Instance();

	//注入子类与业务类型的依赖关系 使得IOC获得创建对象的控制权
	//实现控制的反转
	IocContainerAgent::GetInstance()->RegisterType<Base, Derived_0, const std::string&, int, int*>("BS,0");
	IocContainerAgent::GetInstance()->RegisterType<Base, Derived_1, const std::string&, int, int*>("BS,1");
}

void BusinessProcess(int nProType, const std::string& strConstructParam, const std::string& strJobParam, int nParam)
{
	std::string strIocKey = std::string("BS,") + std::to_string(nProType);
	//根据不同的业务类型创建不同的子类来处理业务，实现处理的地方与子类解耦只依赖基类和IOC容器，实现控制反转
	//对于const std::string&类似类型的参数需要显式指出模板参数的类型，
	//而对于int、int*类似的参数则不用显式的指出模板参数的类型，可以自动推导
	//当然你也可以显式的指定参数的类型
	auto pBase = IocContainerAgent::GetInstance()->ResolveShared<Base, const std::string&>(strIocKey, strConstructParam, nParam, &nParam);
	if (pBase)
	{
		pBase->DoJob(strJobParam);
	}
	else
	{
		std::cout << "无法解析的业务类型Base[" << strIocKey << "]" << std::endl;
	}
}


int main()
{
	//初始化 IOC 容器
	InitIocContainer();

	//处理业务
	BusinessProcess(0, "ConstructParam : for test constructor param.", "JobParam : for test base class IF param.", 0);
	BusinessProcess(1, "ConstructParam : for test constructor param.", "JobParam : for test base class IF param.", 0);
	BusinessProcess(2, "ConstructParam : for test constructor param.", "JobParam : for test base class IF param.", 0);


	return 0;
}