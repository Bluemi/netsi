#ifndef __NETSI_ENDPOINT_CLASS__
#define __NETSI_ENDPOINT_CLASS__

#include <boost/asio/ip/udp.hpp>

namespace netsi {
	using Endpoint = boost::asio::ip::udp::endpoint;

	struct EndpointHasher {
		std::size_t operator()(const Endpoint& endpoint) const;
	};
}

#endif
