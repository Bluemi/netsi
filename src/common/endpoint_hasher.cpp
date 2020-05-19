#include "endpoint_hasher.hpp"


namespace netsi {
	// MurmurHash taken from https://sites.google.com/site/murmurhash/
	// MurmurHash, by Austin Appleby
	std::uint64_t MurmurHash64(const void* key, std::int32_t len, std::uint32_t seed) {
		const std::uint64_t m = 0xc6a4a7935bd1e995;
		const std::int32_t r = 47;

		uint64_t h = seed ^ (len * m);

		const uint64_t * data = (const uint64_t *)key;
		const uint64_t * end = data + (len/8);

		while(data != end)
		{
			uint64_t k = *data++;

			k *= m; 
			k ^= k >> r; 
			k *= m; 
			
			h ^= k;
			h *= m; 
		}

		const unsigned char * data2 = (const unsigned char*)data;

		switch(len & 7)
		{
		case 7: h ^= uint64_t(data2[6]) << 48;
		case 6: h ^= uint64_t(data2[5]) << 40;
		case 5: h ^= uint64_t(data2[4]) << 32;
		case 4: h ^= uint64_t(data2[3]) << 24;
		case 3: h ^= uint64_t(data2[2]) << 16;
		case 2: h ^= uint64_t(data2[1]) << 8;
		case 1: h ^= uint64_t(data2[0]);
				h *= m;
		};
	 
		h ^= h >> r;
		h *= m;
		h ^= h >> r;

		return h;
	} 

	// MurmurHash, by Austin Appleby
	std::uint32_t MurmurHash32(const void* key, std::int32_t len, std::uint32_t seed) {
		const std::uint32_t m = 0x5bd1e995;
		const std::int32_t r = 24;

		std::uint32_t h = seed ^ len;

		const unsigned char * data = (const unsigned char *)key;

		while(len >= 4)
		{
			std::uint32_t k = *(std::uint32_t *)data;

			k *= m; 
			k ^= k >> r; 
			k *= m; 
			
			h *= m; 
			h ^= k;

			data += 4;
			len -= 4;
		}
		
		switch(len)
		{
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
				h *= m;
		};

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	} 

	std::size_t EndpointHasher::operator()(const endpoint& e) const {
		static_assert(sizeof(std::size_t) == 4 || sizeof(std::size_t) == 8);
		if constexpr (sizeof(std::size_t) == 4) {
			return MurmurHash32(e.data(), e.capacity(), 42);
		} else if constexpr (sizeof(std::size_t) == 8) {
			return MurmurHash64(e.data(), e.capacity(), 42);
		}
	}
}
