//
//  AnimaMD5.h
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//	Got from http://www.zedwood.com/article/cpp-md5-function
//
//

#ifndef __Anima__AnimaMD5__
#define __Anima__AnimaMD5__

#include "AnimaEngineCore.h"
#include <string>

// define fixed size integer types
#ifdef _MSC_VER
// Windows
typedef unsigned __int8  uint8_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
// GCC
#include <stdint.h>
#endif

BEGIN_ANIMA_ENGINE_NAMESPACE

class ANIMA_ENGINE_EXPORT AnimaMD5
{
public:
	/// split into 64 byte blocks (=> 512 bits), hash is 16 bytes long
	enum { BlockSize = 512 / 8, HashBytes = 16 };
	
	/// same as reset()
	AnimaMD5();
	
	/// compute MD5 of a memory block
	std::string operator()(const void* data, size_t numBytes);
	/// compute MD5 of a string, excluding final zero
	std::string operator()(const std::string& text);
	
	/// add arbitrary number of bytes
	void add(const void* data, size_t numBytes);
	
	/// return latest hash as 32 hex characters
	std::string getHash();
	/// return latest hash as bytes
	void        getHash(unsigned char buffer[HashBytes]);
	
	/// restart
	void reset();
	
private:
	/// process 64 bytes
	void processBlock(const void* data);
	/// process everything left in the internal buffer
	void processBuffer();
	
	/// size of processed data in bytes
	uint64_t m_numBytes;
	/// valid bytes in m_buffer
	size_t   m_bufferSize;
	/// bytes not processed yet
	uint8_t  m_buffer[BlockSize];
	
	enum { HashValues = HashBytes / 4 };
	/// hash, stored as integers
	uint32_t m_hash[HashValues];
};

//class ANIMA_ENGINE_EXPORT AnimaMD5
//{
//public:
//	typedef unsigned int size_type; // must be 32bit
//
//	AnimaMD5();
//	AnimaMD5(const std::string& text);
//	void update(const unsigned char *buf, size_type length);
//	void update(const char *buf, size_type length);
//	AnimaMD5& finalize();
//	std::string hexdigest() const;
//
//	static std::string MD5(const std::string str);
//
//private:
//	void init();
//	typedef unsigned char uint1; //  8bit
//	typedef unsigned int uint4;  // 32bit
//	enum { blocksize = 64 }; // VC6 won't eat a const static int here
//
//	void transform(const uint1 block[blocksize]);
//	static void decode(uint4 output[], const uint1 input[], size_type len);
//	static void encode(uint1 output[], const uint4 input[], size_type len);
//
//	bool finalized;
//	uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
//	uint4 count[2];   // 64bit counter for number of bits (lo, hi)
//	uint4 state[4];   // digest so far
//	uint1 digest[16]; // the result
//
//	// low level logic operations
//	static inline uint4 F(uint4 x, uint4 y, uint4 z);
//	static inline uint4 G(uint4 x, uint4 y, uint4 z);
//	static inline uint4 H(uint4 x, uint4 y, uint4 z);
//	static inline uint4 I(uint4 x, uint4 y, uint4 z);
//	static inline uint4 rotate_left(uint4 x, int n);
//	static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
//	static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
//	static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
//	static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
//};
//
//std::ostream& operator<<(std::ostream&, AnimaMD5 md5);

END_ANIMA_ENGINE_NAMESPACE

#endif /* defined(__Anima__AnimaMD5__) */
