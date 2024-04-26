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

Using a big array of integer as hash table: give an index then return a different number of integer:

<img width="769" alt="Screen Shot 2024-04-26 at 10 45 38 AM" src="https://github.com/James-Z-Zhang00/FTP_Client_N_Server/assets/144994336/104f586f-7f50-4482-acb4-b5c6e1e51129">

Repeat Square algorithm implementation:

<img width="1056" alt="Screen Shot 2024-04-26 at 10 48 42 AM" src="https://github.com/James-Z-Zhang00/FTP_Client_N_Server/assets/144994336/ab159519-1de5-4d56-908f-7a55ec0af96b">

Encrypring process, using a for-loop to iterate through the input buffer:
   * Get ascii value of the char
   * Applying the binary XOR value to the ascii value and pre-assigned temp_XOR value, use the result as the position number for hash table
   * Get hashed value
   * Update the temp_XOR value for the next char encryption
   * Use RSA algorithm to get encrypted value and send to the server

<img width="1424" alt="Screen Shot 2024-04-26 at 10 48 18 AM" src="https://github.com/James-Z-Zhang00/FTP_Client_N_Server/assets/144994336/5ebfeab7-6f04-4fe7-84f0-25f1fe48dc2f">

Server Site:

Make required values for decryption:

<img width="627" alt="Screen Shot 2024-04-26 at 11 01 00 AM" src="https://github.com/James-Z-Zhang00/FTP_Client_N_Server/assets/144994336/a5b7bbba-8cc8-4fd6-b402-674548a7621e">

The Greatest Common Divisor and big prime number functions:

<img width="829" alt="Screen Shot 2024-04-26 at 11 00 54 AM" src="https://github.com/James-Z-Zhang00/FTP_Client_N_Server/assets/144994336/a6757551-d927-46f9-b36a-d3a4d46c1472">

Decrypting process:
   * Use RSA algorithm again for decrypting the first layer
   * Substitute the value into hash table to get de-hashed value
   * Apply de-hashed value to temp_XOR with binary XOR operation to get the original ascii value
   * Convert ascii value to char to get the actual message
   * Update the temp_XOR value for the next char decryption

<img width="1276" alt="Screen Shot 2024-04-26 at 11 00 34 AM" src="https://github.com/James-Z-Zhang00/FTP_Client_N_Server/assets/144994336/3d5e59c1-cea4-4f30-a10e-a846858070bd">



