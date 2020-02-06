#ifdef CPOINTER_H
#ifndef _CPOINTER_INL_
#define _CPOINTER_INL_
// pass-thru custom placement new/delete

/*template <typename bufferType>
inline void* operator new(size_t sz, bufferType* buffer)
{
	// do whatever management buffer does
	// rough example - do not have a manager currently
	//void* ret = buffer->alloc(sz);
	//	return ret;

	void* p = 0;
	return p;
}

template<typename bufferType>
inline void operator delete(void* ptr, bufferType* buffer)
{
	// do whatever manager buffer does
	buffer->dealloc(ptr);
}

template <typename bufferType>
inline void* operator new[](size_t sz, bufferType* buffer)
{
	// do whatever management buffer does
	// rough example - do not have a manager currently
	return operator::new<bufferType>(sz, buffer);
}

template<typename bufferType>
inline void operator delete[](void* ptr, bufferType* buffer)
{
	// do whatever manager buffer does
	operator::operator delete<bufferType>(ptr, buffer);
}*/



#endif
#endif