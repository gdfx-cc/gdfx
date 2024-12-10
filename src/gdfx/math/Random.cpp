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
#include <gdfx/math/Random.hpp>

namespace gdfx {

RandomMersenne32::RandomMersenne32()
	: mti(N + 1)
{
	mag01[0] = 0x0UL;
	mag01[1] = MATRIX_A;
}

/* initializes mt[N] with a seed */
void RandomMersenne32::seed(const uint32_t s)
{
	mt[0] = s & 0xffffffffUL;
	for (mti = 1; (uint32_t)mti < N; mti++) {
		mt[mti] = (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 

		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array mt[].                        */
		/* 2002/01/09 modified by Makoto Matsumoto             */
		mt[mti] &= 0xffffffffUL;
		/* for >32 bit machines */
	}
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void RandomMersenne32::seedArray(const uint32_t initKey[], const uint32_t keyLength)
{
	int32_t i, j, k;
	seed(19650218UL);

	i = 1; j = 0;
	k = (N > keyLength ? N : keyLength);

	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL)) 
			+ initKey[j] + j; /* non linear */
		mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
		i++; j++;
		if ((uint32_t)i >= N) { 
			mt[0] = mt[N-1]; 
			i = 1; 
		}
		if (j >= (int32_t)(keyLength)) 
			j = 0;
	}

	for (k = N-1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
			- i; /* non linear */
		mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
		i++;
		if ((uint32_t)i >= N) {
			mt[0] = mt[N-1]; 
			i = 1; 
		}
	}

	mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
uint32_t RandomMersenne32::randUInteger()
{
	uint32_t y;
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if ((uint32_t)mti >= N) { /* generate N words at one time */
		int kk;

		if (mti == N+1)   /* if seed() has not been called, */
			seed(5489UL); /* a default initial seed is used */

		for (kk = 0; (uint32_t)kk < N-M; kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (; (uint32_t)kk < N-1; kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
	}

	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
int32_t RandomMersenne32::randInteger()
{
	return (int32_t)(randUInteger() >> 1);
}

/* generates a random number on [0,1]-real-interval */
double RandomMersenne32::randDouble()
{
	return randUInteger()*(1.0/4294967295.0); 
	/* divided by 2^32-1 */ 
}

RandomMersenne64::RandomMersenne64()
	: mti(NN + 1)
{
	mag01[0] = 0x0ULL;
	mag01[1] = MATRIX_A;
}

/* initializes mt[NN] with a seed */
void RandomMersenne64::seed(const uint64_t s)
{
	mt[0] = s;
	for (mti = 1; (uint64_t)mti < NN; mti++)
		mt[mti] = (6364136223846793005ULL * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
void RandomMersenne64::seedArray(const uint64_t initKey[], const uint64_t keyLength)
{
	uint64_t i, j, k;
	seed(19650218ULL);

	i = 1; j = 0;
	k = (NN > keyLength ? NN : keyLength);

	for (; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 3935559000370003845ULL)) 
			+ initKey[j] + j; /* non linear */
		i++; j++;
		if (i >= NN) { 
			mt[0] = mt[NN-1]; 
			i = 1; 
		}
		if (j >= keyLength) 
			j=0;
	}

	for (k = NN-1; k; k--) {
		mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 2862933555777941757ULL))
			- i; /* non linear */
		i++;
		if (i >= NN) { 
			mt[0] = mt[NN-1]; 
			i = 1; 
		}
	}

	mt[0] = 1ULL << 63; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0, 2^64-1]-interval */
uint64_t RandomMersenne64::randUInteger()
{
	int32_t i;
	uint64_t x;

	if ((uint64_t)mti >= NN) { /* generate NN words at one time */

		if (mti == NN+1)   /* if seed() has not been called, */
			seed(5489ULL); /* a default initial seed is used */

		for (i = 0; (uint32_t)i < NN-MM; i++) {
			x = (mt[i]&UM)|(mt[i+1]&LM);
			mt[i] = mt[i+MM] ^ (x >> 1) ^ mag01[(int32_t)(x & 1ULL)];
		}
		for (; (uint32_t)i < NN-1; i++) {
			x = (mt[i]&UM)|(mt[i+1]&LM);
			mt[i] = mt[i+(MM-NN)] ^ (x >> 1) ^ mag01[(int32_t)(x & 1ULL)];
		}
		x = (mt[NN-1]&UM)|(mt[0]&LM);
		mt[NN-1] = mt[MM-1] ^ (x >> 1) ^ mag01[(int32_t)(x & 1ULL)];

		mti = 0;
	}

	x = mt[mti++];

	/* Tempering */
	x ^= (x >> 29) & 0x5555555555555555ULL;
	x ^= (x << 17) & 0x71D67FFFEDA60000ULL;
	x ^= (x << 37) & 0xFFF7EEE000000000ULL;
	x ^= (x >> 43);

	return x;
}

/* generates a random number on [0, 2^63-1]-interval */
int64_t RandomMersenne64::randInteger()
{
	return (int64_t)(randUInteger() >> 1);
}

/* generates a random number on [0,1]-real-interval */
double RandomMersenne64::randDouble()
{
	return (randUInteger() >> 11)*(1.0/9007199254740991.0); 
}

} // gdfx
