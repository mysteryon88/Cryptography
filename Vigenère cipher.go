package main

import (
	"fmt"
	"strings"
	"bufio"
	"os"
)

func main() {
	var message, keyword string
	cipherText := ""

	fmt.Print("Enter your message: ")
	scanner := bufio.NewScanner(os.Stdin)
	if scanner.Scan() {
		message += scanner.Text() + " "
	}
	
	fmt.Print("Enter the key: ")
	scanner = bufio.NewScanner(os.Stdin)
	if scanner.Scan() {
		keyword += scanner.Text()
	}

	message = strings.ToUpper(message)
	keyword = strings.ToUpper(keyword)
	
	cipherText = Encryption(message, keyword)
	fmt.Println(cipherText)
	fmt.Println(Decryption(cipherText, keyword))
}

func Encryption(message string, keyword string) string {
	cipherText := ""
	keyIndex := 0
	for i := 0; i < len(message); i++ {
		c := message[i]
		if c >= 'A' && c <= 'Z' {
		
			c -= 'A'
			k := keyword[keyIndex] - 'A'

			c = (c + k) % 26 + 'A'

			keyIndex++
			keyIndex %= len(keyword)
		}
		cipherText += string(c)
	}
	return cipherText
}

func Decryption(cipherText string, keyword string) string {
	message := ""
	keyIndex := 0
	for i := 0; i < len(cipherText); i++ {
		c := cipherText[i]
		if c >= 'A' && c <= 'Z' {
			
			c -= 'A'
			k := keyword[keyIndex] - 'A'

			c = (c - k + 26 ) % 26 + 'A'

			keyIndex++
			keyIndex %= len(keyword)
		}
		message += string(c)
	}
	return message
}