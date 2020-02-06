#ifndef CPOINTER_H
#define CPOINTER_H

/*template<bool, typename T = void>
struct TYPEDEF {};

template<typename T>
struct TYPEDEF<true, T> { typedef T TYPE; };

// when TYPE(someVar) is called, if the type is void, it will return false
// doing nothing, but if not void then it will return true and call the above
// structed template as the bool would be true
#define TYPE(T)		TYPEDEF<T != void>::TYPE

// assures type using for template is never void
#define NONVOID(T) template<typename T = TYPE(T)>

template<typename T>
class cNonVoidTest
{
public: 
	NONVOID(T) t somefunc(int value) {};
};*/


/* Example of use cases for above templates (Throw it in a main, issue is it won't run in a .c main
and too lazy to make a .cpp main

cNonVoidTest<int> testNonVoid;
testNonVoid.somefunc(3);

// this function will complain as it can't compile as it is of type void
// demo is not 100% working but that is what should happen
cNonVoidtest<void> testVoid;
testVoid.someFunc(3);

*/


struct blah
{
	int stuff;
};

class cDummy
{
	size_t size;
	blah* data;

public:
	// default constructor
	cDummy();

	// copy constructor
	cDummy(cDummy const& rhs);

	// move semantics
	cDummy(cDummy&& rhs);

	// destructor
	~cDummy();

	// overload equals assignment operator
	// setting it to default assigns this operator to the default implementation
	cDummy& operator=(cDummy const& rhs) = default;
	
	// move semantics
	// if you do not want a particular default constructor or overload, assign it to delete
	// so that it does exist / will not work
	cDummy& operator=(cDummy&& rhs) = delete;

	// param ctor
	cDummy(size_t newSize);

	// accessor operator
	blah* operator->() const
	{
		return data;
	}


	void* operator new(size_t sz);
		/*{
			return ::operator new(sz);
		}*/

	void operator delete(void* ptr);
	/*{
		::operator delete(ptr);
	}*/

	void* operator new(size_t sz, void* buffer);
	/*{
		//return ::operator new(buffer, sz);
		return buffer;
	} */

	void operator delete(void* buffer, void* ptr);
	/*{
		
	}*/

	void* operator new[](size_t sz);

	void operator delete[](void* ptr);

	void* operator new[](size_t sz, void* buffer);
	void operator delete[](void* buffer, size_t sz);
};

class cPointer
{

};

#endif // !CPOINTER_H

#include "cPointer.inl"