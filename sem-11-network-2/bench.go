package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"os"
	"sync"
)

func sendToEcho(conn net.Conn, filename string, times int) error {
	for i := 0; i < times; i += 1 {
		file, _ := os.Open(filename)
		scanner := bufio.NewScanner(file)
		size := 0
		for scanner.Scan() {
			line := scanner.Text()
			size += len(line)
			_, err := io.WriteString(conn, line)
			if err != nil {
				return err
			}
		}

		for size > 0 {
			buffer := make([]byte, 4096)
			rbytes, err := conn.Read(buffer)
			if err != nil {
				return err
			}
			size -= rbytes
		}
	}

	return conn.Close()
}

func runClient(addr, port, filename string, queryCount int) error {
	conn, err := net.Dial("tcp", fmt.Sprintf("%s:%s", addr, port))
	if err != nil {
		return err
	}
	return sendToEcho(conn, filename, queryCount)
}

const (
	clientCount = 10000
	queryCount  = 5
)

func main() {
	filename := os.Args[1]
	addr := os.Args[2]
	port := os.Args[3]

	wg := sync.WaitGroup{}
	wg.Add(clientCount)

	for i := 0; i < clientCount; i += 1 {
		go func() {
			err := runClient(addr, port, filename, queryCount)
			if err != nil {
				fmt.Println(err)
			}
			wg.Done()
		}()
	}

	wg.Wait()
}
