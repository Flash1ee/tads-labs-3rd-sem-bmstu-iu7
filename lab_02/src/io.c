#include <stdio.h>
#include <string.h>
#include "io.h"

/**
*@brief Массив строк с жанрами спектаклей
 */
const char *genre[] = {
    "Fairy tale",
    "Piece",
    "Drama",
    "Comedy"
};

void menu()
{
    printf("It is menu for using program.\n");
    printf("Entry 0 for quit\n");
    printf("Entry 1 for load text file with data\n");
    printf("Entry 2 for add struct in file with data\n");
    printf("Entry 3 for print list all music show for kids\n");
    printf("1) Min age = N\n2) Max durability is M\nWhere N and M input from keyboard\n");
}
int input_num(int *num, char string[])
{
    if (!num || (strlen(string) < 2 && *string == '\n'))
        return 1;
    int res;
    char *key = NULL;
    
    res = strtol(string, &key, 10);
    if (key != NULL && *key == '\n')
    {
        *num = res;
        return 0;
    }
    return 1;
    
}
int validation_mode(int8_t mode)
{
    for (size_t i = 0; i < COUNT_MODE; i++)
        if (mode == i)
            return EXIT_SUCCESS;
    return EXIT_FAILURE;
}
int entry(theatre *tmp, FILE *stream)
{
    //TODO создать временные переменные для считывания данных от пользователя
    // ДЛя отслеживания переполнения буфера.
    if (!tmp)
        return STRUCT_ERR;
    char buf[NUM + 2];
    memset(buf, 0, NUM+2);
    printf("Entry title: ");
    if (!fgets(tmp->title, TITLE + 3, stream))
        return READ_ERR;
    if (strlen(tmp->title) - 1 > TITLE)
        return LIMIT_INPUT;
    (tmp->title)[strlen(tmp->title) - 1] = '\0';

    printf("\nEntry name of show: ");
    if (!fgets(tmp->show, SHOW + 3, stream))
        return READ_ERR;
    if (strlen(tmp->show) - 1 > SHOW)
        return LIMIT_INPUT;
    (tmp->show)[strlen(tmp->show) - 1] = '\0';


    printf("\nEntry full producer name: ");
    if (!fgets(tmp->producer, NAME + 3, stream))
        return READ_ERR;
    if (strlen(tmp->producer) - 1 > NAME)
        return LIMIT_INPUT;
    (tmp->producer)[strlen(tmp->producer) - 1] = '\0';


    printf("\nEntry min price for show: ");
    if (!fgets(buf, NUM + 2, stream) || input_num(&(tmp->min_price), buf))
        return READ_ERR;
    memset(buf, 0, NUM+2);
    
    printf("\nEntry max price for show: ");
    if (!fgets(buf, NUM + 2, stream) || input_num(&(tmp->max_price), buf))
        return READ_ERR;
    memset(buf, 0, NUM+2);
    
    printf("\nEntry\n0 if show for childs\n1 if show for adults\n2 if it's music show...\n ");
    if (!fgets(buf, NUM + 2, stream) || input_num((int*)&(tmp->category), buf))
        return READ_ERR;
    memset(buf, 0, NUM+2);
    
    switch (tmp->category)
    {
    case KIDS:
        printf("Your choice - show for kids\n");
        printf("Entry min_age for child ticket: ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int*)&(tmp->choice.minor.min_age), buf) 
        || tmp->choice.minor.min_age < 0)
            return READ_ERR;
        memset(buf, 0, NUM+2);
        
        printf("\nEntry\n0 if show - fairy tale\n1 if show - piece...\n ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int*)&(tmp->choice.minor.type), buf))
            return READ_ERR;
        memset(buf, 0, NUM+2);
       
        if (tmp->choice.minor.type != FAIRY_TALE && tmp->choice.minor.type != PIECE)
            return READ_ERR;
        strcpy(tmp->choice.minor.genre, genre[tmp->choice.minor.type]);
        printf("Struct successfull create.\n");
        break;
    case ADULTS:
        printf("Your choice - show for adults\n");
        printf("\nEntry\n1 if show - piece\n2 if show - drama\n3 if show - comedy...\n ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int*)&(tmp->choice.major.type), buf))
            return READ_ERR;
        memset(buf, 0, NUM+2);
        
        if (tmp->choice.major.type != PIECE && tmp->choice.major.type != DRAMA && tmp->choice.major.type != COMEDY)
            return READ_ERR;
        strcpy(tmp->choice.major.genre, genre[tmp->choice.major.type]);
        printf("Struct successfull create.\n");
        break;
    case MUSIC:
        printf("Your choice - music show\n");
        printf("Entry composer name: ");
        if (!fgets(tmp->choice.sound.composer, NAME + 3, stream))
            return READ_ERR;

        if (strlen(tmp->choice.sound.composer) - 1 > NAME)
            return LIMIT_INPUT;
        (tmp->choice.sound.composer)[strlen(tmp->choice.sound.composer) - 1] = '\0';

        printf("\nEntry composer country: ");
        if (!fgets(tmp->choice.sound.country, COUNTRY + 3, stream))
            return READ_ERR;
        if (strlen(tmp->choice.sound.country) - 1 > COUNTRY)
            return LIMIT_INPUT;
        (tmp->choice.sound.country)[strlen(tmp->choice.sound.country) - 1] = '\0';

        printf("\nEntry min age for listening: ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int*)&(tmp->choice.sound.min_age), buf))
            return READ_ERR;
        memset(buf, 0, NUM+2);
       
        printf("\nEntry track duration: ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int*)&(tmp->choice.sound.time), buf))
            return READ_ERR;
        memset(buf, 0, NUM+2);
        
        printf("Struct successfull create.\n");
        break;
    default:
        return READ_ERR;
    }
    return EXIT_SUCCESS;
}
int cnt_structs(FILE *f, size_t *cnt)
{
    if (!f)
        return FILE_ERR;
    size_t k = 0;
    theatre tmp;
    int rc = 0;
    if (!entry(&tmp, f))
    {
        k += 1;
        rc = entry(&tmp, f);
        while (!rc)
        {
            k++;
            rc = entry(&tmp, f);
        }
    }
    if (!feof(f))
        return READ_ERR;
    if (!k)
        return EMPTY_FILE;
    *cnt += k;
    return EXIT_SUCCESS;
}
int read(theatre src[], size_t len, FILE *in, size_t cnt)
{
    if (!in || !src)
    {
        return READ_ERR;
    }
    int rc = 0;
    theatre tmp;

    for (size_t i = 0; i < cnt; i++)
    {
        rc = entry(&tmp, in);
        if (rc)
        {
            return READ_ERR;
        }
        src[i + len] = tmp;
        memset(&tmp, 0, sizeof(tmp));
    }
    return rc;
}
int output(theatre src[], size_t cnt, size_t pos)
{
    if (!src || (pos > cnt && pos != cnt))
    {
        return PRINT_ERR;
    }
    if (pos == cnt)
    {
        for (size_t i = 0; i < cnt; i++)
        {
            print_pos(src, i);
        }
    }
    else
    {
        print_pos(src, pos);
    }
    return EXIT_SUCCESS;
}
void print_pos(theatre src[], size_t pos)
{
    theatre cur = src[pos];

    printf("title - %s\n", cur.title);
    printf("show - %s\n", cur.show);
    printf("producer - %s\n", cur.producer);
    printf("min_price: %d\n", cur.min_price);
    printf("max_price: %d\n", cur.max_price);
    switch(cur.category)
    {
        case KIDS:
            printf("min_age: %d\n", cur.choice.minor.min_age);
            printf("genre is %s\n", cur.choice.minor.genre);
            break;
        case ADULTS:
            printf("genre is %s\n", cur.choice.major.genre);
            break;
        case MUSIC:
            printf("composer - %s\n", cur.choice.sound.composer);
            printf("country - %s\n", cur.choice.sound.country);
            printf("min_age: %d\n", cur.choice.sound.min_age);
            printf("time in minutes: %d\n", cur.choice.sound.time);
            break;
    }
}
