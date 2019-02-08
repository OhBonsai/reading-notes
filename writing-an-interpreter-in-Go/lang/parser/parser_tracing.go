package parser

import (
	"strings"
	"fmt"
)

const (
	traceIndentPlaceHolder = "\t"
	active = false
)

var traceLevel = 0

func indentLevel() string {
	return strings.Repeat(traceIndentPlaceHolder, traceLevel - 1)
}

func tracePrint(fs string) {
	if active {
		fmt.Printf("%s%s\n", indentLevel(), fs)
	}
}

func incIndent() { traceLevel += 1}
func decIndent() { traceLevel -= 1}

func trace(msg string) string {
	incIndent()
	tracePrint("BEGIN " + msg)
	return msg
}

func unTrace(msg string) {
	tracePrint("END " + msg)
	decIndent()
}
