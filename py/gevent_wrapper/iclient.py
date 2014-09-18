import logging
import struct
import gevent.queue
import gevent.pool
from gevent import socket

logger = logging.getLogger(__name__)

class iClient(object):
    def __init__(self, hostname, port):
        self.hostname = hostname
        self.port = port
        self._socket = None

    def send_message(self, cmd, protomessage):
        data = struct.pack('<B', cmd) + protomessage.SerializeToString()
        data = struct.pack('<i', len(data)) + data
        try:
          self._socket.sendall(data)
        except Exception, e:
          logger.error('send error, msg %s', e)
        #logger.debug('send to %s:%s data %d', self.hostname, self.port, len(msg))
        msg = ""
        try:
          data_size = struct.unpack('<i',self._socket.recv(4))[0]
          if (data_size <= 0):
            logger.error('get data size error')
          msg = ""
          while(len(msg) != data_size):
            last_count = data_size - len(msg)
            msg = msg + self._socket.recv(last_count)
        except Exception, e:
          logger.error('receive error, msg %s',e)
        #logger.debug('from %s:%s receive data len %s', self.hostname, self.port, len(msg))
        msg = str(msg)
        cmd = ord(msg[0])
        protostring = msg[1:]
        return protostring

    def connect(self):
        address = None
        try:
          address = (socket.gethostbyname(self.hostname), self.port)
        except socket.gaierror:
          logger.error('hostname not found')
          raise
        logger.info('connecting to %r...' % (address,))
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
          self._socket.connect(address)
        except Exception, e:
          logger.error('connecting error msg:%s', e)
          raise
