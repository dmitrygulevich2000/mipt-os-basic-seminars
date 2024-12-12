import os
import socket
import socketserver
import sys

import redis

class TcpEchoHandler(socketserver.BaseRequestHandler):
    BUF_SIZE = 4096

    def handle(self):
        sock = self.request

        data = sock.recv(TcpEchoHandler.BUF_SIZE)
        while (data):
            sock.sendall(data)
            data = sock.recv(TcpEchoHandler.BUF_SIZE)

        sock.shutdown(socket.SHUT_RDWR)
        sock.close()

class TcpKvHandler(socketserver.BaseRequestHandler):

    def handle(self):
        sock = self.request
        with sock.makefile('r') as sockfile:
            for line in sockfile:
                result = self.run_command(line)
                if result is not None:
                    sock.sendall((result + '\n').encode('utf-8'))

        self.request.shutdown(socket.SHUT_RDWR)
        self.request.close()

    def run_command(self, line):
        if not line:
            return None
        tokens = line.strip().split(' ')
        if tokens[0] == 'set':
            self.server.storage.set(tokens[1], tokens[2])
            return None
        elif tokens[0] == 'get':
            value = self.server.storage.get(tokens[1])
            if value is None:
                return ""
            return value
        else:
            raise ValueError(f"unknown command {tokens[0]}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"usage: python3 {sys.argv[0]} port", file=sys.stderr)
        exit(1)
    listen_addr = ("0.0.0.0", int(sys.argv[1]))

    redis_host = os.getenv("REDIS_HOST")
    redis_port = os.getenv("REDIS_PORT")
    redis_kv = redis.Redis(host=redis_host, port=int(redis_port), decode_responses=True)

    with socketserver.ThreadingTCPServer(listen_addr, TcpKvHandler) as server:
        server.storage = redis_kv
        server.serve_forever()
