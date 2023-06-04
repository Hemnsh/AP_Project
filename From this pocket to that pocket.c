
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CURRENCY_LENGTH 20
#define MAX_COMMAND_LENGTH 100
#define USER_LENGTH 10
#define SARAFI_LENGTH 10
#define MAX_SIZE 1600
#define MAX_TOKEN 5

struct Currency
{
    long double amount;
    int value;
    char name[MAX_CURRENCY_LENGTH];
} typedef Currency;

struct User
{
    char name[USER_LENGTH];
    Currency *currency;
} typedef User;

struct Sarafi
{
    char name[SARAFI_LENGTH];
    Currency *currency;
} typedef Sarafi;

struct Decrypt
{
    long int array_length;
    long int encrypted[MAX_COMMAND_LENGTH];
    long int decr_int[MAX_COMMAND_LENGTH];
    char decrypt[MAX_COMMAND_LENGTH];
    long int amount;
} typedef Decrypt;

struct Command
{
    char *amount;
    char *command;
    char *user1;
    char *user2;
    char *currency_from;
    char *currency_to;
}typedef Command;

void DEPOSIT(char *name, char* to,char *amount,  User *userData, Sarafi *sarafiData, long int *currencyInfo,long int m,long int size)
{
    int toIndex = -1 , i_user;
    long double i_amount = atof(amount);
    for (int i = 0; i < m; i++){
        if (strcmp(name , userData[i].name) == 0){
            i_user = i;
        }
    }
    for (int i = 0; i < size; i++)
    {
        if (strcmp(to, sarafiData->currency[i].name) == 0)
        {
            toIndex = i;
            break;
        }
    }
    if (i_user == -1){
        printf("Invalid User: %s\n", name);
        return;
    }
    if (sarafiData->currency[toIndex].amount < i_amount)
    {
        printf("Insufficient Funds: %s\n",sarafiData->name);
        return ;
    }
    
    sarafiData->currency[toIndex].amount -= i_amount;
    userData[i_user].currency[toIndex].amount += i_amount;
}

void WITHDRAWL(char *name, char* to,char *amount,  User *userData, Sarafi *sarafiData, long int *currencyInfo,long int m,long int size)
{
    int toIndex = -1 , i_user = -1;
    long double i_amount = atof(amount);
    for (int i = 0; i < m; i++){
        if (strcmp(name , userData[i].name) == 0){
            i_user = i;
        }
    }
    for (int i = 0; i < size; i++)
    {
        if (strcmp(to, sarafiData->currency[i].name) == 0)
        {
            toIndex = i;
            break;
        }
    }
    if (i_user == -1){
        printf("Invalid User: %s\n", name);
        return;
    }
    if (userData[i_user].currency[toIndex].amount < i_amount)
    {
        printf("Insufficient Funds: %s\n",userData[i_user].name);
        return ;
    }
    userData[i_user].currency[toIndex].amount -= i_amount;
    sarafiData->currency[toIndex].amount += i_amount;
}

void transfer(char *user1 ,char *user2, char *currency, char *amount, User *userData, Sarafi *sarafiData , long int *currencyInfo,long int m ,long int n)
{
    int i_user1 = -1 , i_user2 = -1 , i_cur = -1 ;
    for (int i = 0; i<m; i++)
    {
        if(strcmp(user1, userData[i].name) == 0)
        {
            i_user1 = i;
            break;
        }
    }
    for (int i = 0; i < m; i++)
    {
        if(strcmp(user2, userData[i].name) == 0)
        {
            i_user2 = i;
            break;
        }
    }

    for (int j = 0; j < n; j++)
    {
        if(strcmp(currency, sarafiData->currency[j].name) == 0)
        {
            i_cur = j;
            break;
        }
    }
    if (i_user1 == -1){
        printf("Invalid User: %s\n", user1);
    }
    if (i_user2 == -1){
        printf("Invalid User: %s\n", user2);
    }
    long double i_amount = atof((amount));
    long double transactionFee = (i_amount > 100) ? (10.0 / currencyInfo[i_cur]) : (i_amount * 0.10);
    if ((userData[i_user1].currency[i_cur].amount < i_amount + transactionFee)){
        printf("Insufficient Funds: %s\n",userData[i_user1].name);
        return;
    }
    userData[i_user1].currency[i_cur].amount -= i_amount + transactionFee;
    userData[i_user2].currency[i_cur].amount += i_amount;
    sarafiData->currency[i_cur].amount += transactionFee;
    
}

void EXCHANGE(char *user, char *from, char *to, char* amount,  User *userData, Sarafi *sarafiData , long int *currencyInfo,long int m ,long int n)
{
    int fromIndex = -1, toIndex = -1 , i_user = -1;

    for ( int i = 0; i < m; i++)
    {
        if (strcmp(user, userData[i].name) == 0)
        {
            i_user = i;
        }
    }
    if (i_user == -1){
        printf("Invalid User: %s\n", user);
        return;
    }
    for (int i = 0; i < n; i++){
        if (strcmp(from, userData->currency[i].name) == 0){
            fromIndex = i;
        }
        else if (strcmp(to, userData->currency[i].name) == 0)
        {
            toIndex = i;
        }
    }

    if (fromIndex == -1 || toIndex == -1)
    {
        printf("Currency not found\n");
        return;
    }

    long double i_amount = atof((amount));
    long double convertedAmount = i_amount * currencyInfo[fromIndex] / currencyInfo[toIndex];
    long double transactionFee = (i_amount > 1000) ? (10.0 / currencyInfo[fromIndex]) : (i_amount * 0.01);

    if (userData[i_user].currency[fromIndex].amount < i_amount + transactionFee)
    {
        printf("Insufficient Funds: %s\n", user);
        return;
    }
    if (sarafiData->currency[toIndex].amount < convertedAmount)
    {
        printf("Insufficient Funds: %s\n", sarafiData->name);
        return;
    }
    sarafiData->currency[toIndex].amount -= convertedAmount;
    userData[i_user].currency[fromIndex].amount -= transactionFee + i_amount;
    sarafiData->currency[fromIndex].amount += transactionFee + i_amount;
    userData[i_user].currency[toIndex].amount += convertedAmount;

}

void encrypt(int m, Decrypt *decryptData, int modulus, int privateKey)
{
    

    for (int i = 0; i < m; i++)
    {

        for (int j = 0; j < decryptData[i].array_length; j++)
        {
            decryptData[i].decr_int[j]+=1;
            for (int k = 0; k < privateKey; k++)
            {
             decryptData[i].decr_int[j] = ((decryptData[i].decr_int[j])%modulus)*decryptData[i].encrypted[j];
            }
        } 
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < decryptData[i].array_length; j++)
        {
             decryptData[i].decr_int[j] %= modulus;
        } 
    }
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
                }
                else
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
        }
        else if ((phi % primes[i]) != 0)
        {
            result = primes[i];
            break;
        }
    }

    return result;
}

void getAsciiCharacter(int m ,Decrypt *decryptData)
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < decryptData[i].array_length; j++)
        {
            decryptData[i].decrypt[j] = (char)decryptData[i].decr_int[j];
        }
    }
}

int calculatePrivateKey(int publicKey, int phi)
{
    int privateKey = 1;
    while ((privateKey * publicKey) % phi != 1)
    {
        privateKey++;
    }
    return privateKey;
}

void Decryption(int m, Decrypt *decryptData)
{
    int primeP = 19, primeQ = 101, modulus, phi, publicKey, privateKey;


    modulus = primeP * primeQ;
    phi = (primeQ - 1) * (primeP - 1);
    publicKey = calculatePublicKey(phi);
    privateKey = calculatePrivateKey(publicKey, phi);

    encrypt(m ,decryptData, modulus, privateKey);
    getAsciiCharacter(m ,decryptData);


}


char** tokenizeCommand(char* str)
{
    char** tokens = (char**) malloc(MAX_TOKEN* sizeof(char**));
    for(int i = 0 ; i < MAX_TOKEN ; i++) {
        tokens[i] = (char*) malloc(MAX_CURRENCY_LENGTH*sizeof(char*));
    }
    int i = 0;
    char *token = strtok(str, " ");
   
    while (token != NULL)
    {
        strcpy(tokens[i], token);
        i += 1;
        token = strtok(NULL, " ");
    }
    return tokens;
}


int main()
{
    long int n;
    scanf("%ld", &n);

    Currency *currencyData = (Currency *)malloc(n * sizeof(Currency));
    long int *currencyInfo = (long int *)malloc(n * sizeof(long int));
    User *userData = (User *)malloc(n * sizeof(User));
    Sarafi *sarafiData = (Sarafi *)malloc(sizeof(Sarafi));
    userData->currency = NULL;

    for (long int i = 0; i < n; i++)
    {
        scanf("%s %d", currencyData[i].name, &currencyData[i].value);
        currencyInfo[i] = currencyData[i].value;
    }

    scanf("%s", sarafiData->name);
    sarafiData->currency = (Currency *)malloc(n * sizeof(Currency));
    memcpy(sarafiData->currency, currencyData, n * sizeof(Currency));
    for (int i = 0; i < n; i++)
    {
        scanf("%Lf", &(sarafiData->currency[i].amount));
    }

    long int n_user;
    scanf("%ld", &n_user);
    for (int i = 0; i < n_user; i++)
    {
        scanf("%s", userData[i].name);
        userData[i].currency = (Currency *)malloc(n * sizeof(Currency));
        memcpy(userData[i].currency, currencyData, n * sizeof(Currency));
        for (int j = 0; j < n; j++)
        {
            scanf("%Lf", &(userData[i].currency[j].amount));
        }
    }


    long int m;
    scanf("%ld", &m);
    Decrypt *decryptData = (Decrypt *)malloc(m * sizeof(Decrypt));

    for (int i = 0; i < m; i++)
    {
        scanf("%ld", &(decryptData[i].array_length));
        for (int j = 0; j < decryptData[i].array_length; j++)
        {
            scanf("%ld", &(decryptData[i].encrypted[j]));
        }
    }

    Decryption(m, decryptData);

    Command *commandData = (Command *)malloc(m * sizeof(Command));

    for (int i = 0; i < m; i++)
    {
        char ** tokens = tokenizeCommand(decryptData[i].decrypt);
        if (strcmp(tokens[0], "TRANSFER") == 0){
            commandData[i].command = tokens[0];
            commandData[i].user1 = tokens[1];
            commandData[i].user2 = tokens[2];
            commandData[i].currency_to = tokens[3];
            commandData[i].amount = tokens[4];
        }
        else if (strcmp(tokens[0], "EXCHANGE") == 0){
            commandData[i].command = tokens[0];
            commandData[i].user1 = tokens[1];
            commandData[i].currency_from = tokens[2];
            commandData[i].currency_to = tokens[3];
            commandData[i].amount = tokens[4];
        }
        else if (strcmp(tokens[0], "WITHDRAW") == 0 || (strcmp(tokens[0], "DEPOSIT")) == 0)
        {
            commandData[i].command = tokens[0];
            commandData[i].user1 = tokens[1];
            commandData[i].currency_to = tokens[2];
            commandData[i].amount = tokens[3];
        }
        
    }

    for (int i = 0; i < m ; i++){
        if (strcmp(commandData[i].command , "TRANSFER") == 0)
        {
            transfer(commandData[i].user1 ,commandData[i].user2 , commandData[i].currency_to , commandData[i].amount ,userData, sarafiData , currencyInfo ,n_user , n);
        }
        else if (strcmp(commandData[i].command , "EXCHANGE") == 0)
        {
            EXCHANGE(commandData[i].user1, commandData[i].currency_from,commandData[i].currency_to,commandData[i].amount,userData,sarafiData,currencyInfo,n_user,n);
        }
        else if (strcmp(commandData[i].command , "DEPOSIT") == 0)
        {
            DEPOSIT(commandData[i].user1,commandData[i].currency_to,commandData[i].amount,userData,sarafiData,currencyInfo,n_user,n);
        }
        else if (strcmp(commandData[i].command , "WITHDRAW") == 0)
        {
            WITHDRAWL(commandData[i].user1,commandData[i].currency_to,commandData[i].amount,userData,sarafiData,currencyInfo,n_user,n);
        }
        else 
        {
            printf("Invalid Command\n");
        }   
    }
    for (long int i = 0; i < n ; i++){
        printf ("%.2Lf ", sarafiData->currency[i].amount);
    }
    printf("\n");
    for (long int i = 0; i < n_user; i++)
    {
        for (long int j = 0; j < n ; j++)
        {
        printf("%.2Lf ", userData[i].currency[j].amount);
        }
        printf("\n");
    }

    free(currencyData);
    free(currencyInfo);
    free(sarafiData->currency);
    free(sarafiData);
    for (int i = 0; i < n_user; i++)
    {
        free(userData[i].currency);
    }
    free(userData);
    free(decryptData);

    return 0;
}