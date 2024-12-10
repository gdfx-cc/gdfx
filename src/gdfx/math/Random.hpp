//-----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
//-----------------------------------------------------------------------------
/*
   MT19937 - Mersenne Twister - 2002/1/26
   MT19937-64 - Mersenne Twister - 2004/9/29

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   Copyright (C) 2004, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef __GDFX_MATH_RANDOM_HPP__
#define __GDFX_MATH_RANDOM_HPP__

#include <cstdint>

namespace gdfx {

/**
 * RandomPosix - POSIX.1-2001 inspired prng
 */
class RandomPosix {
public:
	static const uint32_t MAX = 0x7fffffffUL;

	RandomPosix() : next(1) {}
	~RandomPosix() {}

	void seed(const uint32_t seed) { next = seed; }

	int32_t randInteger(int32_t min, int32_t max) { return min + randInteger() % (max - min); }
	int32_t randInteger()
	{
		next = next * 1103515245 + 12345;
		return (next % (MAX + 1));
	}

	uint32_t randUInteger(uint32_t min, uint32_t max) { return min + randUInteger() % (max - min); }
	uint32_t randUInteger() { return (uint32_t)randInteger(); }
	double randDouble() { return randUInteger()*(1.0/(double)MAX); }
	float randFloat() { return (float)randDouble(); }

private:
	uint32_t next;
};

/**
 * RandomMersenne32 - 32-bit Mersenne Twister prng
 */
class RandomMersenne32 {
public:
	/* Period parameters */
	static const uint32_t N = 624;
	static const uint32_t M = 397;
	static const uint32_t MATRIX_A = 0x9908b0dfUL; /* constant vector a */
	static const uint32_t UPPER_MASK = 0x80000000UL; /* most significant w-r bits */
	static const uint32_t LOWER_MASK = 0x7fffffffUL; /* least significant r bits */

	RandomMersenne32();
	~RandomMersenne32() {}

	void seed(const uint32_t s);
	void seedArray(const uint32_t initKey[], const uint32_t keyLength);

	uint32_t randUInteger(uint32_t min, uint32_t max) { return min + randUInteger() % (max - min); }
	uint32_t randUInteger();

	int32_t randInteger(int32_t min, int32_t max) { return min + randInteger() % (max - min); }
	int32_t randInteger();

	double randDouble();
	float randFloat() { return (float)randDouble(); }

private:
	uint32_t mt[N]; /* the array for the state vector  */
	int32_t mti; /* mti==N+1 means mt[N] is not initialized */
	uint32_t mag01[2];
};

/**
 * RandomMersenne64 - 64-bit Mersenne Twister prng
 */
class RandomMersenne64 {
public:
	/* Period parameters */
	static const uint64_t NN = 312ULL;
	static const uint64_t MM = 156ULL;
	static const uint64_t MATRIX_A = 0xB5026F5AA96619E9ULL; /* constant vector a */
	static const uint64_t UM = 0xFFFFFFFF80000000ULL; /* most significant 33 bits */
	static const uint64_t LM = 0x7FFFFFFFULL; /* least significant 31 bits */

	RandomMersenne64();
	~RandomMersenne64() {}

	void seed(const uint64_t s);
	void seedArray(const uint64_t initKey[], const uint64_t keyLength);

	uint64_t randUInteger(uint64_t min, uint64_t max) { return min + randUInteger() % (max - min); }
	uint64_t randUInteger();
	int64_t randInteger(int64_t min, int64_t max) { return min + randInteger() % (max - min); }
	int64_t randInteger();
	double randDouble();
	float randFloat() { return (float)randDouble(); }

private:
	uint64_t mt[NN]; /* the array for the state vector  */
	int64_t mti; /* mti==N+1 means mt[N] is not initialized */
	uint64_t mag01[2];
};

/* default random number generator */
typedef RandomMersenne32 Random;

} // gdfx

#endif // __GDFX_MATH_RANDOM_HPP__

