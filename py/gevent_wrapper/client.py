import logging
import struct
import gevent.queue
import gevent.pool
from gevent import socket

logger = logging.getLogger(__name__)

class Client(object):

    def __init__(self, hostname, port):
        self.hostname = hostname
        self.port = port
        self._socket = None
        self._recv_queue = gevent.queue.Queue()
        self._send_queue = gevent.queue.Queue()
        self._group = gevent.pool.Group()
        self._handlers = {}

    def _on_exception(self, who):
        logger.error('some wrong, exiting %s', who)
        self.stop()

    def add_handler(self, caller, command):
        self._handlers[command] = caller

    def send_message(self, cmd, protomessage):
        data = struct.pack('<B', cmd) + protomessage.SerializeToString()
        self._send_queue.put(data)
        #logger.debug('send_message to %s:%s cmd %d', self.hostname, self.port, cmd)

    def start(self):
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
        self._group.spawn(self._send_loop)
        self._group.spawn(self._process_loop)
        self._group.spawn(self._recv_loop)
        for g in self._group:
            g.link_exception(self._on_exception)
        gevent.sleep(0)

    def _recv_loop(self):
        while True:
            try:
                data_size = struct.unpack('<i',self._socket.recv(4))[0]
                if(data_size <= 0):
                    logger.error('get data size error')
                    continue
                msg = ""
                while(len(msg) != data_size):
                    last_count = data_size - len(msg)
                    msg = msg + self._socket.recv(last_count)
            except Exception, e:
                logger.error('receive error, msg %s',e)
                self.stop()
            #logger.debug('from %s:%s receive data len %s', self.hostname, self.port, len(msg))
            self._recv_queue.put(str(msg))

    def _send_loop(self):
        while True:
            msg = self._send_queue.get()

            data = struct.pack('<i', len(msg)) + msg
            try:
                self._socket.sendall(data)
            except Exception, e:
                logger.error('send error, msg %s', e)
                self.stop()
            #logger.debug('send to %s:%s data %d', self.hostname, self.port, len(msg))

    def _process_loop(self):
        while True:
            msg = self._recv_queue.get()
            cmd = ord(msg[0])
            protostring = msg[1:]
            handler = self._handlers.get(cmd)
            #logger.debug('handle from %s:%s cmd:%s protolen:%d',  self.hostname, self.port, cmd, len(protostring))
            if handler is not None:
                self._group.spawn(handler, self, protostring)


    def stop(self):
        self._group.kill()
        if self._socket is not None:
            self._socket.close()
            self._socket = None

    def join(self):
        self._group.join()
