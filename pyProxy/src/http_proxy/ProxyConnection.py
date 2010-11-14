import time

__author__="alstein"
__date__ ="$11.11.2010 15:00:59$"

class Connection:
    """ represents abstract class for network connections """
    def __init__(self, time=int(time.time())):
        self._last_upd_time = time
    

class ProxyClient(Connection):
    """ represents client side of the proxy connection """
    # client was just created
    INIT = 0
    # client is in process of getting request
    GETTING_REQUEST = 1
    # client is in process of parsing request
    PARSING_REQUEST = 2
    # client is used in retranslator
    RETRANSLATOR = 3
    # client is used in cache transmission
    CACHE = 4

    def __init__(self, c_sock, c_addr):
        self._c_sock = c_sock
        self._c_addr = c_addr
        self._request = ''

        self._url = None
        self._parsed_url = None
        self._method = None

        self._bytes_received = 0
        # what kind of connection is this client?
        self._status = ProxyClient.INIT

    def close(self):
        self._c_sock.close()

    def fileno(self):
        return self._c_sock.fileno()
    

class ProxyHTTPServer(Connection):
    """ represents server side of the proxy connection """
    # server was just created
    INIT = 0
    # server is in process of sending request
    SENDING_REQUEST = 1
    # server is in process of sending request
    GETTING_RESPONSE = 2
    # server is used in retranslator
    RETRANSLATOR = 3
    # server is used in cache transmissions
    CACHE = 4
    
    def __init__(self, s_sock):
        self._s_sock = s_sock
        self._bytes_sent = 0
        self._status = ProxyHTTPServer.SENDING_REQUEST
    def send(self, buf):
        sent = self._s_sock.send(buf)
        self._bytes_sent += sent
    def close(self):
        self._s_sock.close()
    def fileno(self):
        return self._s_sock.fileno()