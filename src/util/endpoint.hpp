#ifndef __NETSI_ENDPOINT_CLASS__
#define __NETSI_ENDPOINT_CLASS__

#include <boost/asio.hpp>

namespace netsi {
	using endpoint = boost::asio::ip::udp::endpoint;
}

#endif
