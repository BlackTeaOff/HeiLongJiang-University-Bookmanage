#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(void)
{
    char password[20];
    scanf("%s", password);
    if (strlen(password) != 8)
    {
        printf("密码长度不为8位\n");
        exit(1);
    }
    int flag = 0;
    for (int i = 0; i < strlen(password); i++)
    {
        if (isalpha(password[i]))
        {
            flag++;
            break;
        }
    }
    if (flag == 0)
    {
        printf("密码不包含字母\n");
        exit(1);
    }
    else
    {
        printf("密码符合要求\n");
    }
}