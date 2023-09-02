package main

import (
	"crypto/hmac"
	"crypto/sha256"
	"fmt"
)

func main() {

	secretKey := []byte("Secret Key")
	message := []byte("Message")

	// Calculate message hash
	hash := sha256.New()
	hash.Write(message)
	messageHash := hash.Sum(nil)

	// Create HMAC signature
	hmacHash := hmac.New(sha256.New, secretKey)
	hmacHash.Write(messageHash)
	signature := hmacHash.Sum(nil)

	fmt.Printf("Message hash: %x\n", messageHash)
	fmt.Printf("Signature: %x\n", signature)

	if verifySignature(secretKey, message, signature) {
		fmt.Println("The signature is correct")
	} else {
		fmt.Println("The signature is invalid")
	}
}

func verifySignature(secretKey, message, signature []byte) bool {

	// Calculate message hash
	hash := sha256.New()
	hash.Write(message)
	messageHash := hash.Sum(nil)

	// Create an HMAC signature using a secret key
	hmacHash := hmac.New(sha256.New, secretKey)
	hmacHash.Write(messageHash)
	expectedSignature := hmacHash.Sum(nil)

	// Comparing the expected signature with the received one
	return hmac.Equal(signature, expectedSignature)
}
