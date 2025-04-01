#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct User       //用户结构
{
    int id;
    char password[20];
    int usertype;
} User;

typedef struct            //读者结构
{
    int id;
    char name[20];
    char workunit[20];
    long phonenumber;
    int available;
    int borrownum;
} Reader;

typedef struct book       //图书结构
{
    int recordnum;
    int id;
    char name[20];
    char author[20];
    char press[20];
    int collectionnum;
    int lendnum;
    int pointer_1;
    int pointer_2;
    int pointer_3;
} Book;

typedef struct name_index  //索引结构
{
    char name[20];
    int head;
    int length;
} Name_index;

typedef struct author_index //索引结构
{
    char author[20];
    int head;
    int length;
} Author_index;

typedef struct press_index //索引结构
{
    char press[20];
    int head;
    int length;
} Press_index;

typedef struct id_index
{
    int id;
    int recordnum;
} Id_index;

typedef struct circulation
{
    int id;
    int bookid;
    char lendtime[20];
    char returntime[20];
    int state;
} Circulation;

typedef struct Node       //链表结构
{
    User data;
    Reader reader_data;
    Circulation circulation_data;
    struct Node *next;
} Node;


Node *load(void)
{
    User user;
    FILE *fp;
    fp = fopen("userinformation.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    Node *head, *p, *q;  //p末尾节点，q新节点
    head = p = malloc(sizeof(Node));
    while (fscanf(fp, "%d %s %d", &user.id, user.password, &user.usertype) != EOF)
    {
        q = malloc(sizeof(Node));
        q->data.id = user.id;
        strcpy(q->data.password, user.password);
        q->data.usertype = user.usertype;
        p->next = q;
        p = q;
        //printf("%d %s %d\n", user.id, user.password, user.usertype);
    }
    fclose(fp);
    p->next = NULL;
    return head;
}

int user_id;

void login(Node *head, int *usertype)
{
    int id;
    char password[20];
    int try = 0;
    while (try < 3)
    {
        printf("请输入用户名：");
        scanf("%d", &id);
        printf("请输入密码：");
        scanf("%s", password);
        Node *p;
        p = head->next;
        while (p != NULL)
        {
            if (p->data.id == id && strcmp(p->data.password, password) == 0)
            {
                printf("登录成功\n");
                *usertype = p->data.usertype;
                user_id = id;
                return;
            }
            else
            {
                p = p->next;
            }
        }
        if (p == NULL)
        {
            printf("登录失败\n");
            try++;
        }
    }
    if (try == 3)
    {
        printf("登录失败次数过多\n");
        exit(1);
    }
}

int password_check(char *password)
{
    if (strlen(password) != 8)
    {
        printf("密码长度不为8位\n");
        return 0;
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
        return 0;
    }
    else
    {
        printf("密码符合要求\n");
        return 1;
    }
}

void add_user(Node *head, User user)
{
    Node *p, *q;
    p = head;
    while (p->next != NULL)
    {
        p = p->next;
    }
    q = malloc(sizeof(Node));
    q->data.id = user.id;
    strcpy(q->data.password, user.password);
    q->data.usertype = user.usertype;
    p->next = q;
    q->next = NULL;
}

void modify_user(Node *head, int id)
{
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        if (p->data.id == id)
        {
            while (1)
            {
                printf("请输入新密码：");
                scanf("%s", p->data.password);
                if (password_check(p->data.password) == 1)
                {
                    break;
                }
            }
            printf("请输入新用户类型：");
            scanf("%d", &p->data.usertype);
            printf("修改成功\n");
            return;
        }
        else
        {
            p = p->next;
        }
    }
    if (p == NULL)
    {
        printf("用户不存在\n");
    }
}

Node *loadreader(void);

void delete_user(Node *head, int id)
{
    Node *p, *q;
    p = head;
    while (p->next != NULL)
    {
        if (p->next->data.id == id)
        {
            Node *p_reader = loadreader();
            while (p_reader->next != NULL)
            {
                if (p_reader->next->reader_data.id == id)
                {
                    printf("已找到该读者\n");
                    if (p_reader->next->reader_data.borrownum != 0)
                    {
                        printf("该读者仍有书未还，无法删除\n");
                        return;
                    }
                    else
                    {
                        break;
                    }
                }
                p_reader= p_reader->next;
            }
            if (p_reader->next == NULL)
            {
                printf("读者不存在\n");
            }
            q = p->next;
            p->next = q->next;
            free(q);
            printf("删除成功\n");
            return;
        }
        else
        {
            p = p->next;
        }
    }
    if (p->next == NULL)
    {
        printf("用户不存在\n");
    }
}

/*
void search_user(Node *head, int id)
{
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        if (p->data.id == id)
        {
            printf("%d %s %d\n", p->data.id, p->data.password, p->data.usertype);
            return;
        }
        else
        {
            p = p->next;
        }
    }
    if (p == NULL)
    {
        printf("用户不存在\n");
    }
}
*/

void show_user(Node *head)
{
    Node *p = head->next;
    while (p != NULL)
    {
        printf("用户名：%d, 密码：%s, 用户类型：%d\n", p->data.id, p->data.password, p->data.usertype);
        p = p->next;
    }
}

void modify_password(Node *head, int id)
{
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        if (p->data.id == id)
        {
            while (1)
            {
                printf("请输入新密码：");
                scanf("%s", p->data.password);
                if (password_check(p->data.password) == 1)
                {
                    break;
                }
            }
            printf("修改成功\n");
            return;
        }
        else
        {
            p = p->next;
        }
    }
    if (p == NULL)
    {
        printf("用户不存在\n");
    }
}

void write_user(Node *head)
{
    FILE *fp;
    fp = fopen("userinformation.txt", "w");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        fprintf(fp, "%d %s %d\n", p->data.id, p->data.password, p->data.usertype);
        p = p->next;
    }
    fclose(fp);
}

void mainmenu(void)
{
    printf("***************\n");
    printf("1.用户管理\n");
    printf("2.读者管理\n");
    printf("3.图书管理\n");
    printf("4.图书流通管理\n");
    printf("5.退出系统\n");
    printf("***************\n");
}

void usermanage(Node *head, int usertype)
{
    while (1)
    {
        printf("***************\n");
        printf("1.用户信息输入\n");
        printf("2.用户信息修改\n");
        printf("3.用户信息删除\n");
        printf("4.用户信息显示\n");
        printf("5.用户密码修改\n");
        printf("6.返回主菜单\n");
        printf("***************\n");

        int choice;
        scanf("%d", &choice);
        int id;
        switch (choice)
        {
            case 1:
                if (usertype != 1)
                {
                    printf("权限不足\n");
                    break;
                }
                printf("用户信息输入\n");
                User user;
                printf("请输入用户id: ");
                scanf("%d", &user.id);
                Node *p = head->next;
                while (p != NULL)
                {
                    if (p->data.id == user.id)
                    {
                        printf("用户已存在\n");
                        return;
                    }
                    p = p->next;
                }
                while (1)
                {
                    printf("请输入用户密码: ");
                    scanf("%s", user.password);
                    if (password_check(user.password) == 1)
                    {
                        break;
                    }
                }
                printf("请输入用户类型: ");
                scanf("%d", &user.usertype);
                add_user(head, user);
                printf("添加成功\n");
                break;
            case 2:
                if (usertype != 1)
                {
                    printf("权限不足\n");
                    break;
                }
                printf("用户信息修改\n");
                printf("请输入要修改的用户id: ");
                scanf("%d", &id);
                modify_user(head, id);
                break;
            case 3:
                if (usertype != 1)
                {
                    printf("权限不足\n");
                    break;
                }
                printf("用户信息删除\n");
                printf("请输入要删除的用户id: ");
                scanf("%d", &id);
                delete_user(head, id);
                break;
            case 4:
                printf("--------------------\n");
                show_user(head);
                printf("--------------------\n");
                break;
                /*
                if (usertype != 1)
                {
                    printf("权限不足\n");
                    break;
                }
                printf("用户信息显示\n");
                printf("请输入要显示的用户id: ");
                scanf("%d", &id);
                search_user(head, id);
                break;
                */
            case 5:
                printf("用户密码修改\n");
                modify_password(head, user_id);
                break;
            case 6:
                printf("返回主菜单\n");
                write_user(head);
                return;
        }
    }
}

Node *loadreader(void)
{
    FILE *fp;
    fp = fopen("readerinformation.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    Reader reader;
    Node *head, *p, *q;  //p末尾节点，q新节点
    head = p = malloc(sizeof(Node));
    while (fscanf(fp, "%d %s %s %ld %d %d", &reader.id, reader.name, reader.workunit, &reader.phonenumber, &reader.available, &reader.borrownum) != EOF)
    {
        q = malloc(sizeof(Node));
        q->reader_data.id = reader.id;
        strcpy(q->reader_data.name, reader.name);
        strcpy(q->reader_data.workunit, reader.workunit);
        q->reader_data.phonenumber = reader.phonenumber;
        q->reader_data.available = reader.available;
        q->reader_data.borrownum = reader.borrownum;
        p->next = q;
        p = q;
        //printf("%d %s %s %d %d %d\n", reader.id, reader.name, reader.workunit, reader.phonenumber, reader.available, reader.borrownum);
    }
    fclose(fp);
    p->next = NULL;
    return head;
}

void add_reader(Node *head, Reader reader)
{
    Node *p ,*q;
    p = head;
    while (p->next != NULL)
    {
        p = p->next;
    }
    q = malloc(sizeof(Node));
    q->reader_data.id = reader.id;
    strcpy(q->reader_data.name, reader.name);
    strcpy(q->reader_data.workunit, reader.workunit);
    q->reader_data.phonenumber = reader.phonenumber;
    q->reader_data.available = reader.available;
    q->reader_data.borrownum = reader.borrownum;
    p->next = q;
    q->next = NULL;
}

void write_reader(Node *head)
{
    FILE *fp;
    fp = fopen("readerinformation.txt", "w");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        fprintf(fp, "%d %s %s %ld %d %d\n", p->reader_data.id, p->reader_data.name, p->reader_data.workunit, p->reader_data.phonenumber, p->reader_data.available, p->reader_data.borrownum);
        p = p->next;
    }
    fclose(fp);
}

void modify_reader(Node *head, int id)
{
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        if (p->reader_data.id == id)
        {
            printf("请输入新读者姓名: ");
            scanf("%s", p->reader_data.name);
            printf("请输入新读者工作单位: ");
            scanf("%s", p->reader_data.workunit);
            printf("请输入新读者电话号码: ");
            scanf("%ld", &p->reader_data.phonenumber);
            printf("请输入新读者可借阅数量: ");
            scanf("%d", &p->reader_data.available);
            printf("请输入新读者已借阅数量: ");
            scanf("%d", &p->reader_data.borrownum);
            printf("修改成功\n");
            return;
        }
        else
        {
            p = p->next;
        }
    }
    if (p == NULL)
    {
        printf("读者不存在\n");
    }
}

void delete_reader(Node *head, int id)
{
    Node *p, *q;
    p = head;
    while (p->next != NULL)
    {
        if (p->next->reader_data.id == id)
        {
            if (p->next->reader_data.borrownum != 0)
            {
                printf("该读者有书未还，无法删除\n");
                return;
            }
            q = p->next;
            p->next = q->next;
            free(q);
            printf("删除成功\n");
            return;
        }
        else
        {
            p = p->next;
        }
    }
    if (p->next == NULL)
    {
        printf("读者不存在\n");
    }
}

void search_reader(Node *head, int id)
{
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        if (p->reader_data.id == id)
        {
            printf("读者号：%d, 读者名：%s, 读者工作单位：%s, 读者电话号码：%ld, 读者可借书数：%d, 读者已借书数：%d\n", p->reader_data.id, p->reader_data.name, p->reader_data.workunit, p->reader_data.phonenumber, p->reader_data.available, p->reader_data.borrownum);
            return;
        }
        else
        {
            p = p->next;
        }
    }
    if (p == NULL)
    {
        printf("读者不存在\n");
    }
}

void show_reader(Node *head)
{
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        printf("读者号：%d, 读者名：%s, 读者工作单位：%s, 读者电话号码：%ld, 读者可借书数：%d, 读者已借书数：%d\n", p->reader_data.id, p->reader_data.name, p->reader_data.workunit, p->reader_data.phonenumber, p->reader_data.available, p->reader_data.borrownum);
        p = p->next;
    }
}

void readermanage(void)
{
    Node *head = loadreader();
    while (1)
    {
        printf("***************\n");
        printf("1.读者信息输入\n");
        printf("2.读者信息修改\n");
        printf("3.读者信息删除\n");
        printf("4.读者信息查询\n");
        printf("5.读者信息显示\n");
        printf("6.返回主菜单\n");
        printf("***************\n");

        int choice;
        scanf("%d", &choice);
        int id;
        switch (choice)
        {
            case 1:
                printf("读者信息输入\n");
                Reader reader;
                printf("请输入读者id: ");
                scanf("%d", &reader.id);
                Node *p;
                p = head->next;
                while (p != NULL)
                {
                    if (p->reader_data.id == reader.id)
                    {
                        printf("读者号重复\n");
                        return;
                    }
                    p = p->next;
                }
                printf("请输入读者姓名: ");
                scanf("%s", reader.name);
                printf("请输入读者工作单位: ");
                scanf("%s", reader.workunit);
                printf("请输入读者电话号码: ");
                scanf("%ld", &reader.phonenumber);
                printf("请输入读者可借阅数量: ");
                scanf("%d", &reader.available);
                printf("请输入读者已借阅数量: ");
                scanf("%d", &reader.borrownum);
                add_reader(head, reader);
                printf("添加成功\n");
                break;
            case 2:
                printf("读者信息修改\n");
                printf("请输入要修改的读者id: ");
                scanf("%d", &id);
                modify_reader(head, id);
                break;
            case 3:
                printf("读者信息删除\n");
                printf("请输入要删除的读者id: ");
                scanf("%d", &id);
                delete_reader(head, id);
                break;
            case 4:
                printf("读者信息查询\n");
                printf("请输入要查询的读者id: ");
                scanf("%d", &id);
                search_reader(head, id);
                break;
            case 5:
                printf("读者信息显示\n");
                show_reader(head);
                break;
            case 6:
                printf("返回主菜单\n");
                write_reader(head);
                return;
        }
    }
}

void printnode(Node *head)
{
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        printf("%d %s %d\n", p->data.id, p->data.password, p->data.usertype);
        p = p->next;
    }
}

Book book[100];

void loadbook(void)
{
    FILE *fp;
    fp = fopen("bookinformation.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    int i = 0;
    while (fscanf(fp, "%d %d %s %s %s %d %d %d %d %d", &book[i].recordnum, &book[i].id, book[i].name, book[i].author, book[i].press, &book[i].collectionnum, &book[i].lendnum, &book[i].pointer_1, &book[i].pointer_2, &book[i].pointer_3) != EOF)
    {
        i++;
    }
    fclose(fp);
}

Name_index name_index[100];
Author_index author_index[100];
Press_index press_index[100];
Id_index id_index[100];

void loadindex(void)
{
    FILE *fp;
    fp = fopen("name_index.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }

    int i = 0;
    while (fscanf(fp, "%s %d %d", name_index[i].name, &name_index[i].head, &name_index[i].length) != EOF)
    {
        i++;
    }
    fclose(fp);

    fp = fopen("author_index.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }

    i = 0;
    while (fscanf(fp, "%s %d %d", author_index[i].author, &author_index[i].head, &author_index[i].length) != EOF)
    {
        i++;
    }
    fclose(fp);

    fp = fopen("press_index.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }

    i = 0;
    while (fscanf(fp, "%s %d %d", press_index[i].press, &press_index[i].head, &press_index[i].length) != EOF)
    {
        i++;
    }
    fclose(fp);
}

void createid_index(void)
{
    FILE *fp;
    fp = fopen("id_index.txt", "w");
    if (fp == NULL)
    {
        printf("文件创建失败\n");
        exit(1);
    }
    int amount = 0;
    for (int i = 0; book[i].id != '\0'; i++)
    {
        id_index[i].id = book[i].id;
        id_index[i].recordnum = book[i].recordnum;
        amount++;
    }

    for (int i = 0; i < amount - 1; i++) //书号升序
    {
        int min = i;
        for (int j = i; j < amount; j++)
        {
            if (id_index[j].id < id_index[min].id)
            {
                min = j;
            }
        }
        Id_index temp;
        temp = id_index[i];
        id_index[i] = id_index[min];
        id_index[min] = temp;
    }

    for (int i = 0; id_index[i].id != '\0'; i++)
    {
        fprintf(fp, "%d %d\n", id_index[i].id, id_index[i].recordnum);
    }
    fclose(fp);
}

void add_book(void)
{
    Book temp;
    printf("请输入图书号：");
    scanf("%d", &temp.id);
    printf("请输入图书名：");
    scanf("%s", temp.name);
    printf("请输入作者：");
    scanf("%s", temp.author);
    printf("请输入出版社：");
    scanf("%s", temp.press);
    printf("请输入藏书量：");
    scanf("%d", &temp.collectionnum);
    printf("请输入借出量：");
    scanf("%d", &temp.lendnum);

    int booknum = 0;
    int nameposition = 0;  //指针一
    int authorposition = 0;//指针二
    int pressposition = 0; //指针三
    long match_position;   //匹配位置

    for (int i = 0; book[i].id != '\0'; i++)
    {
        if (book[i].id == temp.id)
        {
            printf("图书号重复\n");
            return;
        }

        if (strcmp(book[i].name, temp.name) == 0)
        {
            nameposition = i + 1;
            //printf("nameposition = %d\n", nameposition);
        }

        if (strcmp(book[i].author, temp.author) == 0)
        {
            authorposition = i + 1;
            //printf("authorposition = %d\n", authorposition);
        }

        if (strcmp(book[i].press, temp.press) == 0)
        {
            pressposition = i + 1;
            //printf("pressposition = %d\n", pressposition);
        }
        booknum++;
    }

    if (nameposition != 0) //书名有重复的情况
    {
        FILE *fp;
        fp = fopen("name_index.txt", "r+");  //读写模式
        if (fp == NULL)
        {
            printf("文件打开失败\n");
            exit(1);
        }
        char buffer[100];
        while (fgets(buffer, sizeof(buffer), fp) != NULL)  //逐行翻阅文件
        {
            if (strstr(buffer, temp.name) != NULL)  //strstr用于在一个字符串中查找是否有另一个字符串在里面，并返回该字符串地址
            {
                match_position = ftell(fp) - strlen(buffer);  //减去字符串长度，得到匹配位置(因为fgets跳过了)
                //printf("GOT IT\n");
                break;
            }
        }

        int length = 0; //此段用于修改长度(已修改)
        for (int i = 0; name_index[i].head != 0; i++)
        {
            if (strcmp(name_index[i].name, temp.name) == 0)
            {
                length = name_index[i].length;
                break;
            }
        }

        fseek(fp, match_position, SEEK_SET);  //将文件指针移动到匹配位置
        fprintf(fp, "%s %d %d", temp.name, booknum + 1, length + 1);  //修改长度
        fclose(fp);
    }
    else //书名无重复的情况
    {
        FILE *fp;
        fp = fopen("name_index.txt", "a");  //追加模式
        if (fp == NULL)
        {
            printf("文件打开失败\n");
            exit(1);
        }
        fprintf(fp, "\n%s %d %d", temp.name, booknum + 1, 1);
        fclose(fp);
    }

    if (authorposition != 0) //作者有重复的情况
    {
        FILE *fp;
        fp = fopen("author_index.txt", "r+");
        if (fp == NULL)
        {
            printf("文件打开失败\n");
            exit(1);
        }
        char buffer[100];
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if (strstr(buffer, temp.author) != NULL)
            {
                match_position = ftell(fp) - strlen(buffer);
                //printf("GOT IT\n");
                break;
            }
        }

        int length = 0;
        for (int i = 0; i < author_index[i].head != 0; i++)
        {
            if (strcmp(author_index[i].author, temp.author) == 0)
            {
                length = author_index[i].length;
                break;
            }
        }

        fseek(fp, match_position, SEEK_SET);
        fprintf(fp, "%s %d %d", temp.author, booknum + 1, length + 1);
        fclose(fp);
    }
    else //作者无重复的情况
    {
        FILE *fp;
        fp = fopen("author_index.txt", "a");
        if (fp == NULL)
        {
            printf("文件打开失败\n");
            exit(1);
        }
        fprintf(fp, "\n%s %d %d", temp.author, booknum + 1, 1);
        fclose(fp);
    }

    if (pressposition != 0) //出版社有重复的情况
    {
        FILE *fp;
        fp = fopen("press_index.txt", "r+");
        if (fp == NULL)
        {
            printf("文件打开失败\n");
            exit(1);
        }
        char buffer[100];
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if (strstr(buffer, temp.press) != NULL)
            {
                match_position = ftell(fp) - strlen(buffer);
                //printf("GOT IT\n");
                break;
            }
        }

        int length = 0;
        for (int i = 0; i < press_index[i].head != 0; i++)
        {
            if (strcmp(press_index[i].press, temp.press) == 0)
            {
                length = press_index[i].length;
                break;
            }
        }

        fseek(fp, match_position, SEEK_SET);
        fprintf(fp, "%s %d %d", temp.press, booknum + 1, length + 1);
        fclose(fp);
    }
    else //出版社无重复的情况
    {
        FILE *fp;
        fp = fopen("press_index.txt", "a");
        if (fp == NULL)
        {
            printf("文件打开失败\n");
            exit(1);
        }
        fprintf(fp, "\n%s %d %d", temp.press, booknum + 1, 1);
        fclose(fp);
    }

    FILE *fp;
    fp = fopen("bookinformation.txt", "a"); //写入图书文件
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    fprintf(fp, "\n%d %d %s %s %s %d %d %d %d %d", booknum + 1, temp.id, temp.name, temp.author, temp.press, temp.collectionnum, temp.lendnum, nameposition, authorposition, pressposition);
    fclose(fp);
    /*
    book[booknum + 1].id = temp.id; //写入book结构
    book[booknum + 1].recordnum = booknum + 1;
    strcpy(book[booknum + 1].name, temp.name);
    strcpy(book[booknum + 1].author, temp.author);
    strcpy(book[booknum + 1].press, temp.press);
    book[booknum + 1].collectionnum = temp.collectionnum;
    book[booknum + 1].lendnum = temp.lendnum;
    */
}

void modify_book(void)
{
    int id;
    printf("请输入要修改的图书号：");
    scanf("%d", &id);
    int booknum = -1;
    for (int i = 0; book[i].id != '\0'; i++)
    {
        if (book[i].id == id)
        {
            booknum = i;
            break;
        }
    }
    if (booknum == -1)
    {
        printf("图书不存在\n");
        return;
    }
    printf("请输入新藏书量：");
    scanf("%d", &book[booknum].collectionnum);
    printf("请输入新借出量：");
    scanf("%d", &book[booknum].lendnum);

    FILE *fp;
    fp = fopen("bookinformation.txt", "r+");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }

    char buffer[100];
    char id_str[20];
    /*
    sprintf(id_str, "%d", id);
    long match_position;
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strstr(buffer, id_str) != NULL)
        {
            match_position = ftell(fp) - strlen(buffer);
            fseek(fp, match_position, SEEK_SET);
            fprintf(fp, "%d %d %s %s %s %d %d %d %d %d", book[booknum].recordnum, book[booknum].id, book[booknum].name, book[booknum].author, book[booknum].press, book[booknum].collectionnum, book[booknum].lendnum, book[booknum].pointer_1, book[booknum].pointer_2, book[booknum].pointer_3);
            break;
        }
    }
    fclose(fp);*/

    FILE *tefp; //临时文件转储法(上插入法舍弃)
    tefp = fopen("bookinformation.tmp", "w+");
    if (tefp == NULL)
    {
        printf("临时文件创建失败\n");
        exit(1);
    }
    int i = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        fprintf(tefp, "%d %d %s %s %s %d %d %d %d %d\n", book[i].recordnum, book[i].id, book[i].name, book[i].author, book[i].press, book[i].collectionnum, book[i].lendnum, book[i].pointer_1, book[i].pointer_2, book[i].pointer_3);
        i++;
    }
    fclose(tefp);

    char oldname[] = "bookinformation.tmp"; //重命名
    char newname[] = "bookinformation.txt";
    rename(oldname, newname);
}

int binary_search(int id)  //二分查找
{
    int booknum = 0;
    for (int i = 0; id_index[i].id != '\0'; i++)
    {
        booknum++;
    }
    int left = 0;
    int right = booknum - 1;
    int mid;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (id_index[mid].id > id)
        {
            right = mid - 1;
        }
        else if (id_index[mid].id < id)
        {
            left = mid + 1;
        }
        else
        {
            return mid;
        }
    }

    printf("图书不存在\n");
    return -1;
}


void id_search(void)
{
    int id;
    printf("请输入要查询的图书号：");
    scanf("%d", &id);
    int bookplace = binary_search(id);
    printf("记录号为: %d\n", id_index[bookplace].recordnum);
    int recordnum = id_index[bookplace].recordnum;
    printf("图书名: %s\n", book[recordnum - 1].name);
    printf("作者: %s\n", book[recordnum - 1].author);
    printf("出版社: %s\n", book[recordnum - 1].press);
    printf("藏书量: %d\n", book[recordnum - 1].collectionnum);
    printf("借出量: %d\n", book[recordnum - 1].lendnum);
}

void name_search(void)
{
    char name[20];
    printf("请输入要查询的图书名：");
    scanf("%s", name);
    int namenum = 0;
    for (int i = 0; name_index[i].head != '\0'; i++)
    {
        namenum++;
    }
    int bookplace = 0; // (已修改)
    for (int i = 0; i < namenum; i++)
    {
        if ((strcmp(name, name_index[i].name)) == 0)
        {
            bookplace = i;
            break;
        }
    }
    printf("----------------\n");
    printf("记录号为: %d\n", name_index[bookplace].head);
    int recordnum = name_index[bookplace].head;
    printf("图书名: %s\n", book[recordnum - 1].name);
    printf("作者: %s\n", book[recordnum - 1].author);
    printf("出版社: %s\n", book[recordnum - 1].press);
    printf("藏书量: %d\n", book[recordnum - 1].collectionnum);
    printf("借出量: %d\n", book[recordnum - 1].lendnum);


    while (recordnum != 0)
    {
        recordnum = book[recordnum - 1].pointer_1;
        if (recordnum == 0)
        {
            break;
        }
        printf("----------------\n");
        printf("记录号为: %d\n", recordnum);
        printf("图书名: %s\n", book[recordnum - 1].name);
        printf("作者: %s\n", book[recordnum - 1].author);
        printf("出版社: %s\n", book[recordnum - 1].press);
        printf("藏书量: %d\n", book[recordnum - 1].collectionnum);
        printf("借出量: %d\n", book[recordnum - 1].lendnum);
    }
}

void author_search(void)
{
    char author[20];
    printf("请输入要查询的作者：");
    scanf("%s", author);
    int author_num = 0;
    for (int i = 0; author_index[i].head != '\0'; i++)
    {
        author_num++;
    }
    int bookplace = 0; // (已修改)
    for (int i = 0; i < author_num; i++)
    {
        if ((strcmp(author, author_index[i].author)) == 0)
        {
            bookplace = i;
            break;
        }
    }
    printf("----------------\n");
    printf("记录号为: %d\n", author_index[bookplace].head);
    int recordnum = author_index[bookplace].head;
    printf("图书名: %s\n", book[recordnum - 1].name);
    printf("作者: %s\n", book[recordnum - 1].author);
    printf("出版社: %s\n", book[recordnum - 1].press);
    printf("藏书量: %d\n", book[recordnum - 1].collectionnum);
    printf("借出量: %d\n", book[recordnum - 1].lendnum);

    while (recordnum != 0)
    {
        recordnum = book[recordnum - 1].pointer_2;
        if (recordnum == 0)
        {
            break;
        }
        printf("----------------\n");
        printf("记录号为: %d\n", recordnum);
        printf("图书名: %s\n", book[recordnum - 1].name);
        printf("作者: %s\n", book[recordnum - 1].author);
        printf("出版社: %s\n", book[recordnum - 1].press);
        printf("藏书量: %d\n", book[recordnum - 1].collectionnum);
        printf("借出量: %d\n", book[recordnum - 1].lendnum);
    }
}

void press_search(void)
{
    char press[20];
    printf("请输入要查询的出版社：");
    scanf("%s", press);
    int press_num = 0;
    for (int i = 0; press_index[i].head != '\0'; i++)
    {
        press_num++;
    }
    int bookplace = 0; // (已修改)
    for (int i = 0; i < press_num; i++)
    {
        if ((strcmp(press, press_index[i].press)) == 0)
        {
            bookplace = i;
            break;
        }
    }
    printf("----------------\n");
    printf("记录号为: %d\n", press_index[bookplace].head);
    int recordnum = press_index[bookplace].head;
    printf("图书名: %s\n", book[recordnum - 1].name);
    printf("作者: %s\n", book[recordnum - 1].author);
    printf("出版社: %s\n", book[recordnum - 1].press);
    printf("藏书量: %d\n", book[recordnum - 1].collectionnum);
    printf("借出量: %d\n", book[recordnum - 1].lendnum);

    while (recordnum != 0)
    {
        recordnum = book[recordnum - 1].pointer_3;
        if (recordnum == 0)
        {
            break;
        }
        printf("----------------\n");
        printf("记录号为: %d\n", recordnum);
        printf("图书名: %s\n", book[recordnum - 1].name);
        printf("作者: %s\n", book[recordnum - 1].author);
        printf("出版社: %s\n", book[recordnum - 1].press);
        printf("藏书量: %d\n", book[recordnum - 1].collectionnum);
        printf("借出量: %d\n", book[recordnum - 1].lendnum);
    }
}

void search_book(void)
{
    int choice;
    printf("***************\n");
    printf("1.按书号查询\n");
    printf("2.按书名查询\n");
    printf("3.按作者查询\n");
    printf("4.按出版社查询\n");
    printf("5.返回主菜单\n");
    printf("***************\n");
    scanf("%d", &choice);
    switch (choice)
    {
        case 1:
            printf("按书号查询\n");
            id_search();
            break;
        case 2:
            printf("按书名查询\n");
            name_search();
            break;
        case 3:
            printf("按作者查询\n");
            author_search();
            break;
        case 4:
            printf("按出版社查询\n");
            press_search();
            break;
        case 5:
            printf("返回主菜单\n");
            return;
    }
}

void summary(void)
{
    int booknum = 0;
    for (int i = 0; book[i].id != '\0'; i++)
    {
        booknum++;
    }
    printf("图书总数: %d\n", booknum);
    printf("--------------------\n");
    for (int i = 0; name_index[i].length != '\0'; i++)
    {
        printf("书名为《%s 》的书有 %d 本\n", name_index[i].name, name_index[i].length);
    }
    printf("--------------------\n");
    for (int i = 0; author_index[i].length != '\0'; i++)
    {
        printf("作者为 %s 的书有 %d 本\n", author_index[i].author, author_index[i].length);
    }
    printf("--------------------\n");
    for (int i = 0; press_index[i].length != '\0'; i++)
    {
        printf("出版社为 %s 的书有 %d 本\n", press_index[i].press, press_index[i].length);
    }
    printf("--------------------\n");
}

void bookmanage(int usertype)
{
    while (1)
    {
        loadbook();
        loadindex();
        createid_index();
        printf("***************\n");
        printf("1.图书信息输入\n");
        printf("2.图书信息修改\n");
        printf("3.图书信息查询\n");
        printf("4.汇总统计\n");
        printf("5.返回主菜单\n");
        printf("***************\n");

        int choice;
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                if (usertype != 2)
                {
                    printf("权限不足\n");
                    return;
                }
                printf("图书信息输入\n");
                add_book();
                break;
            case 2:
                if (usertype != 2)
                {
                    printf("权限不足\n");
                    return;
                }
                printf("图书信息修改\n");
                modify_book();
                break;
            case 3:
                printf("图书信息查询\n");
                search_book();
                break;
            case 4:
                printf("汇总统计\n");
                summary();
                break;
            case 5:
                printf("返回主菜单\n");
                return;
        }
    }
}

Node *loadcirculation(void)
{
    FILE *fp;
    fp = fopen("bookcirculation.txt", "r");
    if (fp == NULL)
    {
        printf("文件打开失败\n");
        exit(1);
    }
    Node *head = malloc(sizeof(Node));
    head->next = NULL;
    Node *p, *q;
    p = head;
    while (1)
    {
        q = malloc(sizeof(Node));
        char buffer[1024];
        fgets(buffer, sizeof(buffer), fp); // 每次只读一行
        sscanf(buffer, "%d %d %s %s", &q->circulation_data.id, &q->circulation_data.bookid, q->circulation_data.lendtime, q->circulation_data.returntime);
        p->next = q;
        p = q;
        if (feof(fp))    //feof函数检测文件结束
        {
            break;
        }
    }
    p->next = NULL;
    fclose(fp);
    return head;
}

void lend_book(Node *head_1)
{
    int id;
    printf("请输入读者号：");
    scanf("%d", &id);
    Node *head = loadreader();
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        if (p->reader_data.id == id)
        {
            break;
        }
        else
        {
            p = p->next;
        }
    }
    if (p == NULL)
    {
        printf("读者不存在\n");
        return;
    }
    if (p->reader_data.available == p->reader_data.borrownum)
    {
        printf("借书数量已达上限\n");
        return;
    }
    printf("请输入要借阅的图书号：");
    scanf("%d", &id);
    int bookplace = binary_search(id);
    if (bookplace == -1)
    {
        return;
    }
    int recordnum = id_index[bookplace].recordnum;
    if (book[recordnum - 1].collectionnum == book[recordnum - 1].lendnum)
    {
        printf("该书已借完\n");
        return;
    }
    book[recordnum - 1].lendnum++;
    p->reader_data.borrownum++;
    printf("请输入借书日期：");
    int year, month, day;
    scanf("%d/%d/%d", &year, &month, &day);

    write_reader(head);

    Node *p_1 = head_1;  //创建新节点
    while (p_1->next != NULL)
    {
        if(p_1->next->circulation_data.id == 0)
        {
            break;
        }
        p_1 = p_1->next;
    }
    Node *q = malloc(sizeof(Node));
    q->circulation_data.id = p->reader_data.id;
    q->circulation_data.bookid = book[recordnum - 1].id;
    sprintf(q->circulation_data.lendtime, "%d/%d/%d", year, month, day);
    strcpy(q->circulation_data.returntime, "");
    p_1->next = q;
    q->next = NULL;

    FILE *tefp_1;  //重命名法写入文件
    tefp_1 = fopen("bookcirculation.tmp", "w+");
    if (tefp_1 == NULL)
    {
        printf("临时文件创建失败\n");
        exit(1);
    }
    Node *p_2 = head_1->next;
    while (p_2 != NULL)
    {
        fprintf(tefp_1, "%d %d %s %s\n", p_2->circulation_data.id, p_2->circulation_data.bookid, p_2->circulation_data.lendtime, p_2->circulation_data.returntime);
        p_2 = p_2->next;
    }
    fclose(tefp_1);
    char oldname_1[] = "bookcirculation.tmp"; //重命名
    char newname_1[] = "bookcirculation.txt";
    rename(oldname_1, newname_1);


    FILE *tefp;
    tefp = fopen("bookinformation.tmp", "w+");
    if (tefp == NULL)
    {
        printf("临时文件创建失败\n");
        exit(1);
    }
    int i = 0;
    while (book[i].id != '\0')
    {
        fprintf(tefp, "%d %d %s %s %s %d %d %d %d %d\n", book[i].recordnum, book[i].id, book[i].name, book[i].author, book[i].press, book[i].collectionnum, book[i].lendnum, book[i].pointer_1, book[i].pointer_2, book[i].pointer_3);
        i++;
    }
    fclose(tefp);
    char oldname[] = "bookinformation.tmp"; //重命名
    char newname[] = "bookinformation.txt";
    rename(oldname, newname);
    printf("借阅成功\n");
}

void return_book(Node *head_1)
{
    int id;
    printf("请输入读者号：");
    scanf("%d", &id);
    Node *head = loadreader();
    Node *p;
    p = head->next;
    while (p != NULL)
    {
        if (p->reader_data.id == id)
        {
            break;
        }
        else
        {
            p = p->next;
        }
    }
    if (p == NULL)
    {
        printf("读者不存在\n");
        return;
    }
    if (p->reader_data.borrownum == 0)
    {
        printf("该读者未借阅任何书\n");
        return;
    }
    printf("请输入要归还的图书号：");
    scanf("%d", &id);
    Node *p_2 = head_1->next;  //在链表中查找
    while (p_2 != NULL)
    {
        if (p_2->circulation_data.bookid == id & p_2->circulation_data.id == p->reader_data.id)
        {
            if (p_2->circulation_data.returntime[0] != '\0')
            {
                p_2 = p_2->next;
                continue;
            }
            break;
        }
        else
        {
            p_2 = p_2->next;
        }
    }
    if (p_2 == NULL)
    {
        printf("该读者未借阅此书\n");
        return;
    }
    if (p_2->circulation_data.returntime[0] != '\0')
    {
        printf("该书已归还\n");
        return;
    }
    int bookplace = binary_search(p_2->circulation_data.bookid);
    int recordnum = id_index[bookplace].recordnum;
    book[recordnum - 1].lendnum--;
    p->reader_data.borrownum--;
    printf("请输入归还日期：");
    int year, month, day;
    scanf("%d/%d/%d", &year, &month, &day);
    sprintf(p_2->circulation_data.returntime, "%d/%d/%d", year, month, day);

    write_reader(head);

    FILE *tefp_1;  //重命名法写入文件
    tefp_1 = fopen("bookcirculation.tmp", "w+");
    if (tefp_1 == NULL)
    {
        printf("临时文件创建失败\n");
        exit(1);
    }
    Node *p_1 = head_1->next;
    while (p_1 != NULL)
    {
        fprintf(tefp_1, "%d %d %s %s\n", p_1->circulation_data.id, p_1->circulation_data.bookid, p_1->circulation_data.lendtime, p_1->circulation_data.returntime);
        p_1 = p_1->next;
    }
    fclose(tefp_1);
    char oldname_1[] = "bookcirculation.tmp"; //重命名
    char newname_1[] = "bookcirculation.txt";
    rename(oldname_1, newname_1);

    //写回图书文件
    FILE *tefp;
    tefp = fopen("bookinformation.tmp", "w+");
    if (tefp == NULL)
    {
        printf("临时文件创建失败\n");
        exit(1);
    }
    int i = 0;
    while (book[i].id != '\0')
    {
        fprintf(tefp, "%d %d %s %s %s %d %d %d %d %d\n", book[i].recordnum, book[i].id, book[i].name, book[i].author, book[i].press, book[i].collectionnum, book[i].lendnum, book[i].pointer_1, book[i].pointer_2, book[i].pointer_3);
        i++;
    }
    fclose(tefp);
    char oldname[] = "bookinformation.tmp"; // 重命名
    char newname[] = "bookinformation.txt";
    rename(oldname, newname);
}

void bookcirculation(void)
{
    loadbook();
    createid_index();
    Node *head = loadcirculation();
    while (1)
    {
        printf("***************\n");
        printf("1.借书处理\n");
        printf("2.还书处理\n");
        printf("3.返回主菜单\n");
        printf("***************\n");

        int choice;
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                printf("借书\n");
                lend_book(head);
                Node *head_1 = loadreader();
                write_reader(head_1);
                break;
            case 2:
                printf("还书\n");
                return_book(head);
                Node *head_2 = loadreader();
                write_reader(head_2);
                break;
            case 3:
                printf("返回主菜单\n");
                return;
        }
    }
}

int main(void)
{
    Node *head;
    head = load();
    //printnode(head);
    int usertype;
    login(head, &usertype);
    //printf("usertype = %d\n", usertype);
    int choice;
    while (1)
    {
        mainmenu();
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                printf("用户管理\n");
                usermanage(head, usertype);
                break;
            case 2:
                printf("读者管理\n");
                if (usertype == 2)
                {
                    readermanage();
                }
                else
                {
                    printf("权限不足\n");
                }
                break;
            case 3:
                printf("图书管理\n");
                bookmanage(usertype);
                break;
            case 4:
                if (usertype != 2)
                {
                    printf("权限不足\n");
                    break;
                }
                printf("图书流通管理\n");
                bookcirculation();
                break;
            case 5:
                printf("退出系统\n");
                return 0;
        }
    }
}
