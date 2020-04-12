/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	animal3D Math (A3DM) SDK
	By Daniel S. Buckstein

	a3sqrt_impl.inl
	Implementations for fast square root functions.
*/

#ifdef __ANIMAL3D_A3DM_SQRT_H
#ifndef __ANIMAL3D_A3DM_SQRT_IMPL_INL
#define __ANIMAL3D_A3DM_SQRT_IMPL_INL


A3_BEGIN_IMPL


//-----------------------------------------------------------------------------

// compute fast sqrt/inverse sqrt using Quake's method 
//	("0x5f3759df") using Newton's approximation method
// this only works with float precision (f32)
A3_INLINE a3f32 a3sqrtf0x(const a3f32 x)
{
	// sqrt(x) = x^.5 = x/x^.5 = x/sqrt(x) = x*sqrt^-1(x)
	return x*a3sqrtf0xInverse(x);
}

A3_INLINE a3f32 a3sqrtf0xInverse(const a3f32 x)
{
	union {
		a3f32 f;
		a3integer i;
	} u = { x };

/*
	// input is converted to an integer, shifted one bit, wizardry occurs, 
	//	then it is converted back to decimal as a guess for Newton's method
	const a3i32 i = 0x5f3759df - (*(a3i32 *)&x >> 1);
	a3f32 y = *(a3f32 *)&i;
*/

	// 0x5f3759df method
	u.i = 0x5f3759df - (u.i >> 1);
	return u.f;
}



#if A3_32_BIT
#define A3_NAKED	__declspec(naked)
#define A3_FAST		__fastcall
#else	// !A3_32_BIT
#ifndef A3_INTRIN_INCLUDED
#define A3_INTRIN_INCLUDED
#include <xmmintrin.h>
#endif	// !A3_INTRIN_INCLUDED
#define A3_NAKED	
#define A3_FAST		
#endif	// A3_32_BIT



// apparently the fastest ever (32-bit only): 
// https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
// help with 64-bit (...if it was supported): 
// https://stackoverflow.com/questions/15786404/fld-instruction-x64-bit
// https://msdn.microsoft.com/en-us/library/wbk4z78b(v=vs.140).aspx

A3_INLINE a3f32 A3_FAST a3sqrtf(const a3f32 x)
{
	return (a3f32)a3sqrtd((a3f64)x);
}

#pragma warning(push)
#pragma warning(disable: 4716)	// suppress warnings
A3_NAKED 
A3_INLINE a3f64 A3_FAST a3sqrtd(const a3f64 x)
{
#if (defined _WINDOWS || defined _WIN32)
#if A3_32_BIT
	// Windows inline assembly (masm) implementation
	_asm fld qword ptr[esp + 4];
	_asm fsqrt;
	_asm ret 8;
#else	// !A3_32_BIT
	return (a3f64)a3sqrtf0x((a3f32)x);
#endif	// A3_32_BIT
#else	// !(defined _WINDOWS || defined _WIN32)
#if A3_32_BIT
#else	// !A3_32_BIT
#endif	// A3_32_BIT
	return (a3f64)a3sqrtf0x((a3f32)x);
#endif	// (defined _WINDOWS || defined _WIN32)
}
#pragma warning(pop)

A3_INLINE a3f32 A3_FAST a3sqrtfInverse(const a3f32 x)
{
	return (a3f32)a3sqrtdInverse((a3f64)x);
}

A3_INLINE a3f64 A3_FAST a3sqrtdInverse(const a3f64 x)
{
	if (x != __a3f64zero)
		return __a3recipF64(a3sqrtd(x));
	return __a3f64zero;
}


// done
#undef A3_NAKED
#undef A3_FAST


//-----------------------------------------------------------------------------


A3_END_IMPL


#endif	// !__ANIMAL3D_A3DM_SQRT_IMPL_INL
#endif	// __ANIMAL3D_A3DM_SQRT_H