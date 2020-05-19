#ifndef __NETSI_ENDPOINT_HASHER_CLASS__
#define __NETSI_ENDPOINT_HASHER_CLASS__

#include "endpoint.hpp"

namespace netsi {
	struct EndpointHasher {
		std::size_t operator()(const endpoint& endpoint) const;
	};
}

#endif
