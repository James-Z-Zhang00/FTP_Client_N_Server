# FTP_Client_N_Server

FTP: File Transfer Protocol

This project include secured server and secured client

Secure features achieved by RSA & Repeat Square algorithm 

## RSA (Rivest-Shamir-Adleman) Asymmetric Cryptographic Algorithm

1. Key Generation:
    * Choose 2 distinct prime numbers *`p`* and *`q`*
    * Computer their product, *`n = p X q`* which becomes the modulus for both the public and private keys
    * Compute the totient of *`n`* denoted as *φ(n)* where *`φ(n) = (p - 1) * (q - 1)`*
    * Choose an integer *`e`* such that *`1 < e < φ(n)`* and *`e`* is coprime with *`φ(n)`* where *`e`* is the public exponent
    * Compute the modular multiplicative inverse of *`e`* modulo *`φ(n)`*, denoted as *`d`*, where *`d * e ≡ 1 (mod φ(n))`* where d is the private exponent
    * The public key consists of *`(n, e)`*, and the private key consists of *`(n, d)`*
2. Encryption:
    * To encrypt a message *`M`*, the sender uses the recipient`s public key *`(n, e)`*
    * Convert the message *`M`* into an integer m such that *`0 ≤ m < n`*
    * Compute the ciphertext *`C`* using the formula *`C ≡ m^e (mod n)`*
    * Send the ciphertext *`C`* to the recipient
3. Decryption:
    * To decrypt the ciphertext *`C`*, the recipient uses their private key *`(n, d)`*
    * Compute the plaintext *`M`* using the formula *`M ≡ C^d (mod n)`*
    * Convert the integer *`M`* back into the original message

## Repeat Square

The repeat square algorithm, also known as the square-and-multiply algorithm or binary exponentiation, is a method for efficiently computing the exponentiation of a number to a large power, especially in modular arithmetic contexts.

The basic idea behind the repeat square algorithm is to reduce the number of multiplications needed to compute the result compared to a straightforward approach.

Here's how the repeat square algorithm works:

Start with the base number *`b`*.
Represent the exponent *`e`* in binary form.
Scan the binary representation of the exponent from left to right.
For each bit:
If the bit is `0`, square the current result.
If the bit is `1`, square the current result and multiply it by the base.
After processing all the bits, the final result is obtained.
Let`s illustrate this with an example:

Suppose we want to compute *`b^e`* using the repeat square algorithm.

Let *`b = 3`* and *`e = 13`*.
The binary representation of *`e`* is *`1101`*.
Now, applying the algorithm:

Start with *`b = 3`*.
Since the rightmost bit of *`e`* is 1, multiply the result by *`b`* giving *`3 X 3 = 9`*.
The next bit is *`0`*, so square the result, giving *`9 X 9 = 81`*.
The next bit is *`1`*, so multiply the result by *`b`*, giving *`81 X 3 = 243`*.
The leftmost bit is 1, so multiply the result by *`b`*, giving *`243 X 3 = 729`*.
So, *`3^13 = 729`*.

This algorithm significantly reduces the number of multiplications needed compared to a straightforward approach that directly computes `b^e` by multiplying *`b`* by itself *`e`* times. In cryptography, where large exponentiation operations are common, this efficiency is crucial for performance.

## Encryption & Decryption Implementation

Client Site:

Server Site:




