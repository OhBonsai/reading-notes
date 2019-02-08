package main

import (
	"os/user"
	"fmt"
	"github.com/OhBonsai/yolang/repl"
	"os"
)

func main() {
	user, err := user.Current()

	if err != nil {
		panic(err)
	}

	fmt.Printf("Hello %s, welcome to golang world", user.Username)
	repl.Start(os.Stdin, os.Stdout)
}
