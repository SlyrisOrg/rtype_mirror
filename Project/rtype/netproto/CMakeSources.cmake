set(NETPROTO ${CMAKE_SOURCE_DIR}/Project/rtype/netproto/)

set(RTYPE_NETPROTO_HEADERS
        ${NETPROTO}/TCPProtocolConnection.hpp
        ${NETPROTO}/WeakCallback.hpp
        ${NETPROTO}/IOThread.hpp
        ${NETPROTO}/ClientIOThread.hpp
        PARENT_SCOPE)
