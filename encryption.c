#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1600

long long int* encryptString(char* str, int publicKey, int modulus, int privateKey)
{
    long long int* encrypted = (long long int*)malloc(sizeof(long long int) * 16);

    for (int i = 0; i < 16; i++) 
    {
        encrypted[i] += 1;
        str[i] += 0;
        
        for (int j = 0; j < publicKey; j++)
        {
            encrypted[i] = (encrypted[i] % modulus) * str[i];
        }
    }

    for (int k = 0; k < 16; k++)
    {
        encrypted[k] = encrypted[k] % modulus;
    }

    return encrypted;
}

int calculatePublicKey(int phi)
{
    int result, i, j;
    int primes[phi + 1];

    for (i = 2; i <= phi; i++)
    {
        primes[i] = i;
    }

    i = 2;

    while ((i * i) <= phi)
    {
        if (primes[i] != 0)
        {
            for (j = 2; j < phi; j++)
            {
                if (primes[i] * j > phi)
                {
                    break;
                } else 
                {
                    primes[primes[i] * j] = 0;
                }
            }
        }
        i++;
    }

    for (i = 2; i <= phi; i++)
    {
        if (primes[i] == 0)
        {
            continue;
        } else if ((phi % primes[i]) != 0)
        {
            result = primes[i];
            break;
        }
    }

    return result;
}

char getAsciiCharacter(int num)
{
    char ascii;
    ascii = (num % 26) + 65;
    return ascii;
}

char* hashString(char* str, int count)
{
    int i = 0, j = 0;
    int* asciiSignature = (int*)malloc(sizeof(int) * 16);
    char* ascii = (char*)malloc(sizeof(char) * (count + 1));

    while (str[i] != '\0') 
    {
        j = i % 16;
        asciiSignature[j] += (((3 * str[i]) + 17) % 26) + 65;
        i++;
    }

    for (int i = 0; i < 16; i++) 
    {
        ascii[i] = getAsciiCharacter(asciiSignature[i]);
    }

    return ascii;
}

int calculatePrivateKey(int publicKey, int phi)
{
    int privateKey = 1;
    while ((privateKey * publicKey) % phi != 1) {
        privateKey++;
    }
    return privateKey;
}

int main() 
{
    char inputString[MAX_SIZE];
    int primeP, primeQ, modulus, phi, publicKey, privateKey;
    long long int* encryptedString;

    scanf("%[^\n]%*c", inputString);
    scanf("%d%d", &primeP, &primeQ);

    modulus = primeP * primeQ;
    phi = (primeQ - 1) * (primeP - 1);
    publicKey = calculatePublicKey(phi);
    privateKey = calculatePrivateKey(publicKey, phi);

    int counter = 0;
    for (int i = 0; i < MAX_SIZE; i++) 
    {
        if (inputString[i] == '\0')
        {
            break;
        }
        counter++;
    }

    char* hashedString = hashString(inputString, counter);
    encryptedString = encryptString(hashedString, publicKey, modulus, privateKey);

    for (int i = 0; i < 16; i++)
    {
        printf("%lld ", encryptedString[i]);
    }
    printf("\n");

    return 0;
}
