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
    * To encrypt a message *`M`*, the sender uses the recipient's public key *`(n, e)`*
    * Convert the message *`M`* into an integer m such that *`0 ≤ m < n`*
    * Compute the ciphertext *`C`* using the formula *`C ≡ m^e (mod n)`*
    * Send the ciphertext *`C`* to the recipient
3. Decryption:
    * To decrypt the ciphertext *`C`*, the recipient uses their private key *`(n, d)`*
    * Compute the plaintext *`M`* using the formula *`M ≡ C^d (mod n)`*
    * Convert the integer *`M`* back into the original message

## Repeat Square
