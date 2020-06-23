#ifndef __NONCOPYABLE_HPP__
#define __NONCOPYABLE_HPP__



class NonCopyable
{
protected:
	NonCopyable() = default;
	~NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator= (const NonCopyable&) = delete;
};




#endif //__NONCOPYABLE_HPP__
