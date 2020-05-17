#ifndef __NETSI_MESSAGE_TYPES_CLASS__
#define __NETSI_MESSAGE_TYPES_CLASS__

enum MessageType : char {
	INITIATE = 0,
	DATA = 1,
	PING = 2,
	END = 3
};

#endif
