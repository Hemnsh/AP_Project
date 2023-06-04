#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CURRENCY_LENGTH 20
#define MAX_COMMAND_LENGTH 10

struct currency
{
    long int value;
    char name[MAX_CURRENCY_LENGTH];
};

struct wallet
{
    long double amount;
    struct currency *currency;
};

void getInput(long int *n, struct currency **currencyData, struct wallet **walletData, long int **currencyInfo, long int *m)
{
    scanf("%ld", n);

    *currencyData = (struct currency *)malloc(*n * sizeof(struct currency));
    *walletData = (struct wallet *)malloc(*n * sizeof(struct wallet));
    *currencyInfo = (long int *)malloc(*n * sizeof(long int));

    for (long int i = 0; i < *n; i++)
    {
        scanf("%s %ld", (*currencyData)[i].name, &(*currencyData)[i].value);
        (*currencyInfo)[i] = (*currencyData)[i].value;
        (*walletData)[i].amount = 0.0;
        (*walletData)[i].currency = &(*currencyData)[i];
    }

    for (long int i = 0; i < *n; i++)
    {
        scanf("%Lf", &(*walletData)[i].amount);
    }

    scanf("%ld", m);
}

void DEPOSIT(char *name, long double amount, struct wallet *wallets, long int *currencyInfo, long int size)
{
    for (long int i = 0; i < size; i++)
    {
        if (strcmp(name, wallets[i].currency->name) == 0)
        {
            wallets[i].amount += amount;
            return;
        }
    }
}

void WITHDRAWL(char *name, long double amount, struct wallet *wallets, long int *currencyInfo, long int size)
{
    for (long int i = 0; i < size; i++)
    {
        if (strcmp(name, wallets[i].currency->name) == 0)
        {
            if (wallets[i].amount >= amount)
            {
                wallets[i].amount -= amount;
                return;
            }
            else
            {
                printf("Insufficient Funds\n");
                return;
            }
        }
    }
}

void EXCHANGE(char *from, char *to, long double amount, struct wallet *wallets, long int *currencyInfo, long int size)
{
    long int fromIndex = -1, toIndex = -1;

    for (long int i = 0; i < size; i++)
    {
        if (strcmp(from, wallets[i].currency->name) == 0)
        {
            fromIndex = i;
        }
        else if (strcmp(to, wallets[i].currency->name) == 0)
        {
            toIndex = i;
        }
    }

    if (fromIndex == -1 || toIndex == -1)
    {
        printf("Currency not found\n");
        return;
    }

    long double convertedAmount = amount * currencyInfo[fromIndex] / currencyInfo[toIndex];
    long double transactionFee = (amount > 1000) ? (10.0 / currencyInfo[fromIndex]) : (amount * 0.01);

    if (wallets[fromIndex].amount < amount + transactionFee)
    {
        printf("Insufficient Funds\n");
        return;
    }

    wallets[fromIndex].amount -= amount + transactionFee;
    wallets[toIndex].amount += convertedAmount;
}

void executeCommand(char *command, struct wallet *wallets, long int *currencyInfo, long int size)
{
    if (strcmp(command, "EXCHANGE") == 0)
    {
        char from[MAX_CURRENCY_LENGTH], to[MAX_CURRENCY_LENGTH];
        long double amount;
        scanf("%s %s %Lf", from, to, &amount);
        EXCHANGE(from, to, amount, wallets, currencyInfo, size);
    }
    else if (strcmp(command, "WITHDRAWL") == 0)
    {
        char from[MAX_CURRENCY_LENGTH];
        long double amount;
        scanf("%s %Lf", from, &amount);
        WITHDRAWL(from, amount, wallets, currencyInfo, size);
    }
    else if (strcmp(command, "DEPOSIT") == 0)
    {
        char to[MAX_CURRENCY_LENGTH];
        long double amount;
        scanf("%s %Lf", to, &amount);
        DEPOSIT(to, amount, wallets, currencyInfo, size);
    }
}

int main()
{
    long int n, m;
    struct currency *currencyData;
    struct wallet *walletData;
    long int *currencyInfo;

    getInput(&n, &currencyData, &walletData, &currencyInfo, &m);

    for (long int i = 0; i < m; i++)
    {
        char command[MAX_COMMAND_LENGTH];
        scanf("%s", command);
        executeCommand(command, walletData, currencyInfo, n);
    }

    for (long int i = 0; i < n; i++)
    {
        printf("%.2Lf ", walletData[i].amount);
    }

    free(currencyData);
    free(walletData);
    free(currencyInfo);

    return 0;
}
