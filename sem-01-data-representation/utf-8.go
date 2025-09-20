package main

import "fmt"

func main() {
    str := "Привет"
    chars := []rune(str)

    for i := 0; i < len(str); i += 1 {
        fmt.Printf("%02x ", str[i]);
    }
    println()


    for _, ch := range str {
        fmt.Printf("%d ", ch);
    }
    println()

    for _, ch := range chars {
        fmt.Printf("%08x ", ch);
    }
    println()

    for _, ch := range chars {
        fmt.Printf("%c ", ch);
    }
    println()
}
