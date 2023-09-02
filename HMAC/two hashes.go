package main

import (
	"bytes"
	"crypto/sha256"
	"fmt"
)

const N = 3

type Keys struct {
	key  []byte
	key1 []byte
}

type Signature struct {
	keys      Keys
	Signature []byte
	Message   []byte
	N         int
	// New P keys can be transferred, but they must be signed
	// newKeys   Keys
}

// S -> S1 -> S2 -> P
func generateKeys(n int, keys *Keys) Keys {
	S_ := keys.key
	S_1 := keys.key1
	if n != 0 {
		for i := 0; i < n; i++ {
			hash := sha256.New()
			hash.Write(S_)
			S_ = hash.Sum(nil)

			hash.Write(S_1)
			S_1 = hash.Sum(nil)
			//fmt.Printf("S and S1 #%d: %x %x\n", i, S_, S_1)
		}
	}
	P := S_
	P1 := S_1
	return Keys{P, P1}
}

func signMessage(message string, num int, keys *Keys) Signature {

	hash := sha256.New()
	hash.Write([]byte(message))
	hashMes := hash.Sum(nil)

	combined := append(append(hashMes, keys.key...), keys.key1...)

	hash.Write(combined)
	hashCombined := hash.Sum(nil)

	return Signature{Keys{keys.key, keys.key1}, hashCombined, hashMes, num}
}

func verifySignature(message string, sig *Signature, keys *Keys) bool {
	hash := sha256.New()
	hash.Write([]byte(message))
	hashMes := hash.Sum(nil)

	if !bytes.Equal(hashMes, sig.Message) {
		return false // message hash does not match the signed one
	}

	if !keysCheck(sig, keys) {
		return false // failed to restore P and P1
	}

	combined := append(append(hashMes, sig.keys.key...), sig.keys.key1...)
	hash.Write(combined)
	expectedSignature := hash.Sum(nil)

	if !bytes.Equal(expectedSignature, sig.Signature) {
		return false
	}

	return true
}

func keysCheck(sig *Signature, keys *Keys) bool {
	P := generateKeys(sig.N, &sig.keys)
	if bytes.Equal(P.key, keys.key) && bytes.Equal(P.key1, keys.key1) {
		return true
	}
	return false
}

func main() {

	message := "Hello, World!"
	fmt.Println("Message: ", message)

	seed := Keys{[]byte("key"), []byte("key1")} // S and S1
	fmt.Printf("Seed: S and S1: %x\n", seed)    // secrets
	P := generateKeys(N, &seed)                 // P and P1
	fmt.Printf("P and P1: %x\n\n", P)           // send to Bob

	for i := 1; i < N; i++ {
		keys := generateKeys(N-i, &seed)
		signature := signMessage(message, i, &keys)
		isValid := verifySignature(message, &signature, &P)
		fmt.Printf("Round keys: %x\n", signature.keys)
		fmt.Printf("Signature #%d: %x\n", i, signature.Signature)
		fmt.Printf("Signature is valid: %t\n\n", isValid)
	}

}
