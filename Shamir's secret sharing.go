package main

import (
	"crypto/rand"
	"errors"
	"math/big"
)

// SecretShare represents one fraction of a secret
type SecretShare struct {
	X, Y *big.Int
}

// ShamirSecretSharing contains the parameters of the SSS algorithm
type ShamirSecretSharing struct {
	Threshold int
	Modulus   *big.Int
}

// NewShamirSecretSharing creates a new instance of ShamirSecretSharing
func NewShamirSecretSharing(threshold int, modulus *big.Int) *ShamirSecretSharing {
	return &ShamirSecretSharing{
		Threshold: threshold,
		Modulus:   modulus,
	}
}

// SplitSecret divides a secret into the specified number of shares
func (sss *ShamirSecretSharing) SplitSecret(secret *big.Int, totalShares int) ([]SecretShare, error) {
	if totalShares < sss.Threshold {
		return nil, errors.New("total shares must be greater than or equal to the threshold")
	}

	// Generation of polynomial coefficients
	coefficients := make([]*big.Int, sss.Threshold)
	coefficients[0] = secret
	for i := 1; i < sss.Threshold; i++ {
		coeff, err := rand.Int(rand.Reader, sss.Modulus)
		if err != nil {
			return nil, err
		}
		coefficients[i] = coeff
	}

	// Generation of secretion lobes
	shares := make([]SecretShare, totalShares)
	for i := 1; i <= totalShares; i++ {
		x := big.NewInt(int64(i))
		y := sss.evaluatePolynomial(coefficients, x)
		shares[i-1] = SecretShare{X: x, Y: y}
	}

	return shares, nil
}

// evaluatePolynomial evaluates the value of the polynomial at a given point
func (sss *ShamirSecretSharing) evaluatePolynomial(coefficients []*big.Int, x *big.Int) *big.Int {
	result := big.NewInt(0)
	xPower := big.NewInt(1)
	for _, coeff := range coefficients {
		temp := new(big.Int).Mul(coeff, xPower)
		result.Add(result, temp)
		result.Mod(result, sss.Modulus)

		xPower.Mul(xPower, x)
		xPower.Mod(xPower, sss.Modulus)
	}
	return result
}

// CombineSecret reconstructs a secret from a given number of shares
func (sss *ShamirSecretSharing) CombineSecret(shares []SecretShare) (*big.Int, error) {
	if len(shares) < sss.Threshold {
		return nil, errors.New("not enough shares to reconstruct the secret")
	}

	secret := big.NewInt(0)
	for i, share := range shares {
		li := sss.lagrangePolynomial(shares, i)
		term := new(big.Int).Mul(share.Y, li)
		secret.Add(secret, term)
		secret.Mod(secret, sss.Modulus)
	}

	return secret, nil
}

// lagrangePolynomial computes the Lagrangian polynomial for the fraction
func (sss *ShamirSecretSharing) lagrangePolynomial(shares []SecretShare, i int) *big.Int {
	li := big.NewInt(1)
	for j, otherShare := range shares {
		if j != i {
			numerator := new(big.Int).Neg(otherShare.X)
			denominator := new(big.Int).Sub(shares[i].X, otherShare.X)
			denominator.ModInverse(denominator, sss.Modulus)
			factor := new(big.Int).Mul(numerator, denominator)
			li.Mul(li, factor)
			li.Mod(li, sss.Modulus)
		}
	}
	return li
}

// shuffleAndSliceShares shuffles parts of the secret
func shuffleAndSliceShares(shares []SecretShare, threshold int) ([]SecretShare, error) {
	n := len(shares)
	for i := range shares {
		jBig, err := rand.Int(rand.Reader, big.NewInt(int64(n)))
		if err != nil {
			return nil, err
		}
		j := int(jBig.Int64())

		shares[i], shares[j] = shares[j], shares[i]
	}
	return shares[:threshold], nil
}

func main() {
	// Configuring Shamir's Secret Sharing algorithm
	modulus := big.NewInt(257)                  // Finite field, it is better to use a large prime number
	secret, _ := rand.Int(rand.Reader, modulus) // Secret
	threshold := 4
	totalShares := 6

	println("Secret:", secret.String())

	shamir := NewShamirSecretSharing(threshold, modulus)

	// Splitting the secret
	shares, err := shamir.SplitSecret(secret, totalShares)
	if err != nil {
		panic(err)
	}

	println("\nParts of the secret:")
	for _, share := range shares {
		println("Share", share.X.String(), ":", share.Y.String())
	}

	shuffledAndSlicedShares, err := shuffleAndSliceShares(shares, threshold)

	if err != nil {
		panic(err)
		return
	}

	println("\nMixed parts of the secret:")
	for _, share := range shuffledAndSlicedShares {
		println("Share", share.X.String(), ":", share.Y.String())
	}

	// Restoring the secretion
	reconstructedSecret, err := shamir.CombineSecret(shares[:4])
	if err != nil {
		panic(err)
	}

	println("\nReconstructed Secret:", reconstructedSecret.String())
}
