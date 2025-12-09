package main

import (
	"bufio"
	"fmt"
	"io"
	"net"
	"os"
)

func sendToEcho(conn net.Conn, filename string, times int) error {
	for i := 0; i < times; i += 1 {
		file, err := os.Open(filename)
		if err != nil {
			return err
		}
		scanner := bufio.NewScanner(file)
		for scanner.Scan() {
			line := scanner.Text()
			_, err := io.WriteString(conn, line)
			if err != nil {
				return err
			}
		}

	}
	fmt.Println("All data was sent")
	fmt.Scanln()
	return conn.Close()
}

func runClient(addr, port, filename string, queryCount int) error {
	conn, err := net.Dial("tcp", fmt.Sprintf("%s:%s", addr, port))
	if err != nil {
		return err
	}
	return sendToEcho(conn, filename, queryCount)
}

func main() {
	filename := os.Args[1]
	addr := os.Args[2]
	port := os.Args[3]

	err := runClient(addr, port, filename, 10)
	if err != nil {
		fmt.Println(err)
	}
}
