//
//  AnimaMD5.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaMD5.h"

#ifndef _MSC_VER
#include <machine/endian.h>
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

/// same as reset()
AnimaMD5::AnimaMD5()
{
	reset();
}


/// restart
void AnimaMD5::reset()
{
	m_numBytes   = 0;
	m_bufferSize = 0;
	
	// according to RFC 1321
	m_hash[0] = 0x67452301;
	m_hash[1] = 0xefcdab89;
	m_hash[2] = 0x98badcfe;
	m_hash[3] = 0x10325476;
}


namespace
{
	// mix functions for processBlock()
	inline uint32_t f1(uint32_t b, uint32_t c, uint32_t d)
	{
		return d ^ (b & (c ^ d)); // original: f = (b & c) | ((~b) & d);
	}
	
	inline uint32_t f2(uint32_t b, uint32_t c, uint32_t d)
	{
		return c ^ (d & (b ^ c)); // original: f = (b & d) | (c & (~d));
	}
	
	inline uint32_t f3(uint32_t b, uint32_t c, uint32_t d)
	{
		return b ^ c ^ d;
	}
	
	inline uint32_t f4(uint32_t b, uint32_t c, uint32_t d)
	{
		return c ^ (b | ~d);
	}
	
	inline uint32_t rotate(uint32_t a, uint32_t c)
	{
		return (a << c) | (a >> (32 - c));
	}
	
#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
	inline uint32_t swap(uint32_t x)
	{
#if defined(__GNUC__) || defined(__clang__)
		return __builtin_bswap32(x);
#endif
#ifdef MSC_VER
		return _byteswap_ulong(x);
#endif
		
		return (x >> 24) |
		((x >>  8) & 0x0000FF00) |
		((x <<  8) & 0x00FF0000) |
		(x << 24);
	}
#endif
}


/// process 64 bytes
void AnimaMD5::processBlock(const void* data)
{
	// get last hash
	uint32_t a = m_hash[0];
	uint32_t b = m_hash[1];
	uint32_t c = m_hash[2];
	uint32_t d = m_hash[3];
	
	// data represented as 16x 32-bit words
	const uint32_t* words = (uint32_t*) data;
	
	// computations are little endian, swap data if necessary
#if defined(__BYTE_ORDER) && (__BYTE_ORDER != 0) && (__BYTE_ORDER == __BIG_ENDIAN)
#define LITTLEENDIAN(x) swap(x)
#else
#define LITTLEENDIAN(x) (x)
#endif
	
	// first round
	uint32_t word0  = LITTLEENDIAN(words[ 0]);
	a = rotate(a + f1(b,c,d) + word0  + 0xd76aa478,  7) + b;
	uint32_t word1  = LITTLEENDIAN(words[ 1]);
	d = rotate(d + f1(a,b,c) + word1  + 0xe8c7b756, 12) + a;
	uint32_t word2  = LITTLEENDIAN(words[ 2]);
	c = rotate(c + f1(d,a,b) + word2  + 0x242070db, 17) + d;
	uint32_t word3  = LITTLEENDIAN(words[ 3]);
	b = rotate(b + f1(c,d,a) + word3  + 0xc1bdceee, 22) + c;
	
	uint32_t word4  = LITTLEENDIAN(words[ 4]);
	a = rotate(a + f1(b,c,d) + word4  + 0xf57c0faf,  7) + b;
	uint32_t word5  = LITTLEENDIAN(words[ 5]);
	d = rotate(d + f1(a,b,c) + word5  + 0x4787c62a, 12) + a;
	uint32_t word6  = LITTLEENDIAN(words[ 6]);
	c = rotate(c + f1(d,a,b) + word6  + 0xa8304613, 17) + d;
	uint32_t word7  = LITTLEENDIAN(words[ 7]);
	b = rotate(b + f1(c,d,a) + word7  + 0xfd469501, 22) + c;
	
	uint32_t word8  = LITTLEENDIAN(words[ 8]);
	a = rotate(a + f1(b,c,d) + word8  + 0x698098d8,  7) + b;
	uint32_t word9  = LITTLEENDIAN(words[ 9]);
	d = rotate(d + f1(a,b,c) + word9  + 0x8b44f7af, 12) + a;
	uint32_t word10 = LITTLEENDIAN(words[10]);
	c = rotate(c + f1(d,a,b) + word10 + 0xffff5bb1, 17) + d;
	uint32_t word11 = LITTLEENDIAN(words[11]);
	b = rotate(b + f1(c,d,a) + word11 + 0x895cd7be, 22) + c;
	
	uint32_t word12 = LITTLEENDIAN(words[12]);
	a = rotate(a + f1(b,c,d) + word12 + 0x6b901122,  7) + b;
	uint32_t word13 = LITTLEENDIAN(words[13]);
	d = rotate(d + f1(a,b,c) + word13 + 0xfd987193, 12) + a;
	uint32_t word14 = LITTLEENDIAN(words[14]);
	c = rotate(c + f1(d,a,b) + word14 + 0xa679438e, 17) + d;
	uint32_t word15 = LITTLEENDIAN(words[15]);
	b = rotate(b + f1(c,d,a) + word15 + 0x49b40821, 22) + c;
	
	// second round
	a = rotate(a + f2(b,c,d) + word1  + 0xf61e2562,  5) + b;
	d = rotate(d + f2(a,b,c) + word6  + 0xc040b340,  9) + a;
	c = rotate(c + f2(d,a,b) + word11 + 0x265e5a51, 14) + d;
	b = rotate(b + f2(c,d,a) + word0  + 0xe9b6c7aa, 20) + c;
	
	a = rotate(a + f2(b,c,d) + word5  + 0xd62f105d,  5) + b;
	d = rotate(d + f2(a,b,c) + word10 + 0x02441453,  9) + a;
	c = rotate(c + f2(d,a,b) + word15 + 0xd8a1e681, 14) + d;
	b = rotate(b + f2(c,d,a) + word4  + 0xe7d3fbc8, 20) + c;
	
	a = rotate(a + f2(b,c,d) + word9  + 0x21e1cde6,  5) + b;
	d = rotate(d + f2(a,b,c) + word14 + 0xc33707d6,  9) + a;
	c = rotate(c + f2(d,a,b) + word3  + 0xf4d50d87, 14) + d;
	b = rotate(b + f2(c,d,a) + word8  + 0x455a14ed, 20) + c;
	
	a = rotate(a + f2(b,c,d) + word13 + 0xa9e3e905,  5) + b;
	d = rotate(d + f2(a,b,c) + word2  + 0xfcefa3f8,  9) + a;
	c = rotate(c + f2(d,a,b) + word7  + 0x676f02d9, 14) + d;
	b = rotate(b + f2(c,d,a) + word12 + 0x8d2a4c8a, 20) + c;
	
	// third round
	a = rotate(a + f3(b,c,d) + word5  + 0xfffa3942,  4) + b;
	d = rotate(d + f3(a,b,c) + word8  + 0x8771f681, 11) + a;
	c = rotate(c + f3(d,a,b) + word11 + 0x6d9d6122, 16) + d;
	b = rotate(b + f3(c,d,a) + word14 + 0xfde5380c, 23) + c;
	
	a = rotate(a + f3(b,c,d) + word1  + 0xa4beea44,  4) + b;
	d = rotate(d + f3(a,b,c) + word4  + 0x4bdecfa9, 11) + a;
	c = rotate(c + f3(d,a,b) + word7  + 0xf6bb4b60, 16) + d;
	b = rotate(b + f3(c,d,a) + word10 + 0xbebfbc70, 23) + c;
	
	a = rotate(a + f3(b,c,d) + word13 + 0x289b7ec6,  4) + b;
	d = rotate(d + f3(a,b,c) + word0  + 0xeaa127fa, 11) + a;
	c = rotate(c + f3(d,a,b) + word3  + 0xd4ef3085, 16) + d;
	b = rotate(b + f3(c,d,a) + word6  + 0x04881d05, 23) + c;
	
	a = rotate(a + f3(b,c,d) + word9  + 0xd9d4d039,  4) + b;
	d = rotate(d + f3(a,b,c) + word12 + 0xe6db99e5, 11) + a;
	c = rotate(c + f3(d,a,b) + word15 + 0x1fa27cf8, 16) + d;
	b = rotate(b + f3(c,d,a) + word2  + 0xc4ac5665, 23) + c;
	
	// fourth round
	a = rotate(a + f4(b,c,d) + word0  + 0xf4292244,  6) + b;
	d = rotate(d + f4(a,b,c) + word7  + 0x432aff97, 10) + a;
	c = rotate(c + f4(d,a,b) + word14 + 0xab9423a7, 15) + d;
	b = rotate(b + f4(c,d,a) + word5  + 0xfc93a039, 21) + c;
	
	a = rotate(a + f4(b,c,d) + word12 + 0x655b59c3,  6) + b;
	d = rotate(d + f4(a,b,c) + word3  + 0x8f0ccc92, 10) + a;
	c = rotate(c + f4(d,a,b) + word10 + 0xffeff47d, 15) + d;
	b = rotate(b + f4(c,d,a) + word1  + 0x85845dd1, 21) + c;
	
	a = rotate(a + f4(b,c,d) + word8  + 0x6fa87e4f,  6) + b;
	d = rotate(d + f4(a,b,c) + word15 + 0xfe2ce6e0, 10) + a;
	c = rotate(c + f4(d,a,b) + word6  + 0xa3014314, 15) + d;
	b = rotate(b + f4(c,d,a) + word13 + 0x4e0811a1, 21) + c;
	
	a = rotate(a + f4(b,c,d) + word4  + 0xf7537e82,  6) + b;
	d = rotate(d + f4(a,b,c) + word11 + 0xbd3af235, 10) + a;
	c = rotate(c + f4(d,a,b) + word2  + 0x2ad7d2bb, 15) + d;
	b = rotate(b + f4(c,d,a) + word9  + 0xeb86d391, 21) + c;
	
	// update hash
	m_hash[0] += a;
	m_hash[1] += b;
	m_hash[2] += c;
	m_hash[3] += d;
}


/// add arbitrary number of bytes
void AnimaMD5::add(const void* data, size_t numBytes)
{
	const uint8_t* current = (const uint8_t*) data;
	
	if (m_bufferSize > 0)
	{
		while (numBytes > 0 && m_bufferSize < BlockSize)
		{
			m_buffer[m_bufferSize++] = *current++;
			numBytes--;
		}
	}
	
	// full buffer
	if (m_bufferSize == BlockSize)
	{
		processBlock(m_buffer);
		m_numBytes  += BlockSize;
		m_bufferSize = 0;
	}
	
	// no more data ?
	if (numBytes == 0)
		return;
	
	// process full blocks
	while (numBytes >= BlockSize)
	{
		processBlock(current);
		current    += BlockSize;
		m_numBytes += BlockSize;
		numBytes   -= BlockSize;
	}
	
	// keep remaining bytes in buffer
	while (numBytes > 0)
	{
		m_buffer[m_bufferSize++] = *current++;
		numBytes--;
	}
}


/// process final block, less than 64 bytes
void AnimaMD5::processBuffer()
{
	// the input bytes are considered as bits strings, where the first bit is the most significant bit of the byte
	
	// - append "1" bit to message
	// - append "0" bits until message length in bit mod 512 is 448
	// - append length as 64 bit integer
	
	// number of bits
	size_t paddedLength = m_bufferSize * 8;
	
	// plus one bit set to 1 (always appended)
	paddedLength++;
	
	// number of bits must be (numBits % 512) = 448
	size_t lower11Bits = paddedLength & 511;
	if (lower11Bits <= 448)
		paddedLength +=       448 - lower11Bits;
	else
		paddedLength += 512 + 448 - lower11Bits;
	// convert from bits to bytes
	paddedLength /= 8;
	
	// only needed if additional data flows over into a second block
	unsigned char extra[BlockSize];
	
	// append a "1" bit, 128 => binary 10000000
	if (m_bufferSize < BlockSize)
		m_buffer[m_bufferSize] = 128;
	else
		extra[0] = 128;
	
	size_t i;
	for (i = m_bufferSize + 1; i < BlockSize; i++)
		m_buffer[i] = 0;
	for (; i < paddedLength; i++)
		extra[i - BlockSize] = 0;
	
	// add message length in bits as 64 bit number
	uint64_t msgBits = 8 * (m_numBytes + m_bufferSize);
	// find right position
	unsigned char* addLength;
	if (paddedLength < BlockSize)
		addLength = m_buffer + paddedLength;
	else
		addLength = extra + paddedLength - BlockSize;
	
	// must be little endian
	*addLength++ = msgBits & 0xFF; msgBits >>= 8;
	*addLength++ = msgBits & 0xFF; msgBits >>= 8;
	*addLength++ = msgBits & 0xFF; msgBits >>= 8;
	*addLength++ = msgBits & 0xFF; msgBits >>= 8;
	*addLength++ = msgBits & 0xFF; msgBits >>= 8;
	*addLength++ = msgBits & 0xFF; msgBits >>= 8;
	*addLength++ = msgBits & 0xFF; msgBits >>= 8;
	*addLength++ = msgBits & 0xFF;
	
	// process blocks
	processBlock(m_buffer);
	// flowed over into a second block ?
	if (paddedLength > BlockSize)
		processBlock(extra);
}


/// return latest hash as 32 hex characters
std::string AnimaMD5::getHash()
{
	// compute hash (as raw bytes)
	unsigned char rawHash[HashBytes];
	getHash(rawHash);
	
	// convert to hex string
	std::string result;
	result.reserve(2 * HashBytes);
	for (int i = 0; i < HashBytes; i++)
	{
		static const char dec2hex[16+1] = "0123456789abcdef";
		result += dec2hex[(rawHash[i] >> 4) & 15];
		result += dec2hex[ rawHash[i]       & 15];
	}
	
	return result;
}


/// return latest hash as bytes
void AnimaMD5::getHash(unsigned char buffer[AnimaMD5::HashBytes])
{
	// save old hash if buffer is partially filled
	uint32_t oldHash[HashValues];
	for (int i = 0; i < HashValues; i++)
		oldHash[i] = m_hash[i];
	
	// process remaining bytes
	processBuffer();
	
	unsigned char* current = buffer;
	for (int i = 0; i < HashValues; i++)
	{
		*current++ =  m_hash[i]        & 0xFF;
		*current++ = (m_hash[i] >>  8) & 0xFF;
		*current++ = (m_hash[i] >> 16) & 0xFF;
		*current++ = (m_hash[i] >> 24) & 0xFF;
		
		// restore old hash
		m_hash[i] = oldHash[i];
	}
}


/// compute AnimaMD5 of a memory block
std::string AnimaMD5::operator()(const void* data, size_t numBytes)
{
	reset();
	add(data, numBytes);
	return getHash();
}


/// compute AnimaMD5 of a string, excluding final zero
std::string AnimaMD5::operator()(const std::string& text)
{
	reset();
	add(text.c_str(), text.size());
	return getHash();
}

//// Constants for AnimaMD5Transform routine.
//#define S11 7
//#define S12 12
//#define S13 17
//#define S14 22
//#define S21 5
//#define S22 9
//#define S23 14
//#define S24 20
//#define S31 4
//#define S32 11
//#define S33 16
//#define S34 23
//#define S41 6
//#define S42 10
//#define S43 15
//#define S44 21
//
//// F, G, H and I are basic AnimaMD5 functions.
//inline AnimaMD5::uint4 AnimaMD5::F(uint4 x, uint4 y, uint4 z) {
//	return (x&y) | (~x&z);
//}
//
//inline AnimaMD5::uint4 AnimaMD5::G(uint4 x, uint4 y, uint4 z) {
//	return (x&z) | (y&~z);
//}
//
//inline AnimaMD5::uint4 AnimaMD5::H(uint4 x, uint4 y, uint4 z) {
//	return x^y^z;
//}
//
//inline AnimaMD5::uint4 AnimaMD5::I(uint4 x, uint4 y, uint4 z) {
//	return y ^ (x | ~z);
//}
//
//// rotate_left rotates x left n bits.
//inline AnimaMD5::uint4 AnimaMD5::rotate_left(uint4 x, int n) {
//	return (x << n) | (x >> (32 - n));
//}
//
//// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
//// Rotation is separate from addition to prevent recomputation.
//inline void AnimaMD5::FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
//	a = rotate_left(a + F(b, c, d) + x + ac, s) + b;
//}
//
//inline void AnimaMD5::GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
//	a = rotate_left(a + G(b, c, d) + x + ac, s) + b;
//}
//
//inline void AnimaMD5::HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
//	a = rotate_left(a + H(b, c, d) + x + ac, s) + b;
//}
//
//inline void AnimaMD5::II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
//	a = rotate_left(a + I(b, c, d) + x + ac, s) + b;
//}
//
////////////////////////////////////////////////
//
//// default ctor, just initailize
//AnimaMD5::AnimaMD5()
//{
//	init();
//}
//
////////////////////////////////////////////////
//
//// nifty shortcut ctor, compute AnimaMD5 for string and finalize it right away
//AnimaMD5::AnimaMD5(const std::string &text)
//{
//	init();
//	update(text.c_str(), text.length());
//	finalize();
//}
//
////////////////////////////////
//
//void AnimaMD5::init()
//{
//	finalized = false;
//
//	count[0] = 0;
//	count[1] = 0;
//
//	// load magic initialization constants.
//	state[0] = 0x67452301;
//	state[1] = 0xefcdab89;
//	state[2] = 0x98badcfe;
//	state[3] = 0x10325476;
//}
//
////////////////////////////////
//
//// decodes input (unsigned char) into output (uint4). Assumes len is a multiple of 4.
//void AnimaMD5::decode(uint4 output[], const uint1 input[], size_type len)
//{
//	for (unsigned int i = 0, j = 0; j < len; i++, j += 4)
//		output[i] = ((uint4)input[j]) | (((uint4)input[j + 1]) << 8) |
//		(((uint4)input[j + 2]) << 16) | (((uint4)input[j + 3]) << 24);
//}
//
////////////////////////////////
//
//// encodes input (uint4) into output (unsigned char). Assumes len is
//// a multiple of 4.
//void AnimaMD5::encode(uint1 output[], const uint4 input[], size_type len)
//{
//	for (size_type i = 0, j = 0; j < len; i++, j += 4) {
//		output[j] = input[i] & 0xff;
//		output[j + 1] = (input[i] >> 8) & 0xff;
//		output[j + 2] = (input[i] >> 16) & 0xff;
//		output[j + 3] = (input[i] >> 24) & 0xff;
//	}
//}
//
////////////////////////////////
//
//// apply AnimaMD5 algo on a block
//void AnimaMD5::transform(const uint1 block[blocksize])
//{
//	uint4 a = state[0], b = state[1], c = state[2], d = state[3], x[16];
//	decode(x, block, blocksize);
//
//	/* Round 1 */
//	FF(a, b, c, d, x[0], S11, 0xd76aa478); /* 1 */
//	FF(d, a, b, c, x[1], S12, 0xe8c7b756); /* 2 */
//	FF(c, d, a, b, x[2], S13, 0x242070db); /* 3 */
//	FF(b, c, d, a, x[3], S14, 0xc1bdceee); /* 4 */
//	FF(a, b, c, d, x[4], S11, 0xf57c0faf); /* 5 */
//	FF(d, a, b, c, x[5], S12, 0x4787c62a); /* 6 */
//	FF(c, d, a, b, x[6], S13, 0xa8304613); /* 7 */
//	FF(b, c, d, a, x[7], S14, 0xfd469501); /* 8 */
//	FF(a, b, c, d, x[8], S11, 0x698098d8); /* 9 */
//	FF(d, a, b, c, x[9], S12, 0x8b44f7af); /* 10 */
//	FF(c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
//	FF(b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
//	FF(a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
//	FF(d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
//	FF(c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
//	FF(b, c, d, a, x[15], S14, 0x49b40821); /* 16 */
//
//	/* Round 2 */
//	GG(a, b, c, d, x[1], S21, 0xf61e2562); /* 17 */
//	GG(d, a, b, c, x[6], S22, 0xc040b340); /* 18 */
//	GG(c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
//	GG(b, c, d, a, x[0], S24, 0xe9b6c7aa); /* 20 */
//	GG(a, b, c, d, x[5], S21, 0xd62f105d); /* 21 */
//	GG(d, a, b, c, x[10], S22, 0x2441453); /* 22 */
//	GG(c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
//	GG(b, c, d, a, x[4], S24, 0xe7d3fbc8); /* 24 */
//	GG(a, b, c, d, x[9], S21, 0x21e1cde6); /* 25 */
//	GG(d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
//	GG(c, d, a, b, x[3], S23, 0xf4d50d87); /* 27 */
//	GG(b, c, d, a, x[8], S24, 0x455a14ed); /* 28 */
//	GG(a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
//	GG(d, a, b, c, x[2], S22, 0xfcefa3f8); /* 30 */
//	GG(c, d, a, b, x[7], S23, 0x676f02d9); /* 31 */
//	GG(b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */
//
//	/* Round 3 */
//	HH(a, b, c, d, x[5], S31, 0xfffa3942); /* 33 */
//	HH(d, a, b, c, x[8], S32, 0x8771f681); /* 34 */
//	HH(c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
//	HH(b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
//	HH(a, b, c, d, x[1], S31, 0xa4beea44); /* 37 */
//	HH(d, a, b, c, x[4], S32, 0x4bdecfa9); /* 38 */
//	HH(c, d, a, b, x[7], S33, 0xf6bb4b60); /* 39 */
//	HH(b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
//	HH(a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
//	HH(d, a, b, c, x[0], S32, 0xeaa127fa); /* 42 */
//	HH(c, d, a, b, x[3], S33, 0xd4ef3085); /* 43 */
//	HH(b, c, d, a, x[6], S34, 0x4881d05); /* 44 */
//	HH(a, b, c, d, x[9], S31, 0xd9d4d039); /* 45 */
//	HH(d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
//	HH(c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
//	HH(b, c, d, a, x[2], S34, 0xc4ac5665); /* 48 */
//
//	/* Round 4 */
//	II(a, b, c, d, x[0], S41, 0xf4292244); /* 49 */
//	II(d, a, b, c, x[7], S42, 0x432aff97); /* 50 */
//	II(c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
//	II(b, c, d, a, x[5], S44, 0xfc93a039); /* 52 */
//	II(a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
//	II(d, a, b, c, x[3], S42, 0x8f0ccc92); /* 54 */
//	II(c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
//	II(b, c, d, a, x[1], S44, 0x85845dd1); /* 56 */
//	II(a, b, c, d, x[8], S41, 0x6fa87e4f); /* 57 */
//	II(d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
//	II(c, d, a, b, x[6], S43, 0xa3014314); /* 59 */
//	II(b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
//	II(a, b, c, d, x[4], S41, 0xf7537e82); /* 61 */
//	II(d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
//	II(c, d, a, b, x[2], S43, 0x2ad7d2bb); /* 63 */
//	II(b, c, d, a, x[9], S44, 0xeb86d391); /* 64 */
//
//	state[0] += a;
//	state[1] += b;
//	state[2] += c;
//	state[3] += d;
//
//	// Zeroize sensitive information.
//	memset(x, 0, sizeof x);
//}
//
////////////////////////////////
//
//// AnimaMD5 block update operation. Continues an AnimaMD5 message-digest
//// operation, processing another message block
//void AnimaMD5::update(const unsigned char input[], size_type length)
//{
//	// compute number of bytes mod 64
//	size_type index = count[0] / 8 % blocksize;
//
//	// Update number of bits
//	if ((count[0] += (length << 3)) < (length << 3))
//		count[1]++;
//	count[1] += (length >> 29);
//
//	// number of bytes we need to fill in buffer
//	size_type firstpart = 64 - index;
//
//	size_type i;
//
//	// transform as many times as possible.
//	if (length >= firstpart)
//	{
//		// fill buffer first, transform
//		memcpy(&buffer[index], input, firstpart);
//		transform(buffer);
//
//		// transform chunks of blocksize (64 bytes)
//		for (i = firstpart; i + blocksize <= length; i += blocksize)
//			transform(&input[i]);
//
//		index = 0;
//	}
//	else
//		i = 0;
//
//	// buffer remaining input
//	memcpy(&buffer[index], &input[i], length - i);
//}
//
////////////////////////////////
//
//// for convenience provide a verson with signed char
//void AnimaMD5::update(const char input[], size_type length)
//{
//	update((const unsigned char*)input, length);
//}
//
////////////////////////////////
//
//// AnimaMD5 finalization. Ends an AnimaMD5 message-digest operation, writing the
//// the message digest and zeroizing the context.
//AnimaMD5& AnimaMD5::finalize()
//{
//	static unsigned char padding[64] = {
//		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//	};
//
//	if (!finalized) {
//		// Save number of bits
//		unsigned char bits[8];
//		encode(bits, count, 8);
//
//		// pad out to 56 mod 64.
//		size_type index = count[0] / 8 % 64;
//		size_type padLen = (index < 56) ? (56 - index) : (120 - index);
//		update(padding, padLen);
//
//		// Append length (before padding)
//		update(bits, 8);
//
//		// Store state in digest
//		encode(digest, state, 16);
//
//		// Zeroize sensitive information.
//		memset(buffer, 0, sizeof buffer);
//		memset(count, 0, sizeof count);
//
//		finalized = true;
//	}
//
//	return *this;
//}
//
////////////////////////////////
//
//// return hex representation of digest as string
//std::string AnimaMD5::hexdigest() const
//{
//	if (!finalized)
//		return "";
//
//	char buf[33];
//	for (int i = 0; i<16; i++)
//		sprintf(buf + i * 2, "%02x", digest[i]);
//	buf[32] = 0;
//
//	return std::string(buf);
//}
//
////////////////////////////////
//
//std::ostream& operator<<(std::ostream& out, AnimaMD5 md5)
//{
//	return out << md5.hexdigest();
//}
//
////////////////////////////////
//
//std::string AnimaMD5::MD5(const std::string str)
//{
//	AnimaMD5 md5 = AnimaMD5(str);
//	return md5.hexdigest();
//}

END_ANIMA_ENGINE_NAMESPACE
