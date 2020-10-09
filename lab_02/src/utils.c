#include "utils.h"

/**
*@brief Массив строк с жанрами спектаклей
 */
const char *genre[] = {
    "Fairy tale",
    "Piece",
    "Drama",
    "Comedy"};

int cnt_structs(FILE *f, size_t *cnt)
{
    if (!f)
        return FILE_ERR;
    size_t k = 0;
    theatre_t tmp;
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
int add(theatre_t src[], size_t *len_src, size_t cnt, theatre_key_t arr_key[], size_t *id_count)
{
    if (!src || !arr_key)
        return ARG_ERR;

    theatre_t dst[cnt];
    size_t len_dst = 0;
    if (read(dst, len_dst, stdin, cnt, id_count))
    {
        return READ_ERR;
    }
    for (size_t i = 0; i < cnt; i++)
    {
        src[i + *len_src] = dst[i];
        arr_key[i + *len_src].main_table = &src[i + *len_src];
        arr_key[i + *len_src].min_price = src[i + *len_src].min_price;

    }
    *len_src += cnt;
    return EXIT_SUCCESS;
}
int fill_arr_keys(theatre_t src[], theatre_key_t dst[], size_t len, size_t cnt)
{
    if (!src || !dst)
    {
        return READ_ERR;
    }
    for (size_t i = 0; i < cnt; i++)
    {
        dst[i + len].main_table = &(src[i + len]);
        dst[i + len].min_price = src[i + len].min_price;
    }
    return EXIT_SUCCESS;
}
int entry(theatre_t *tmp, FILE *stream)
{
    if (!tmp)
        return STRUCT_ERR;
    char buf[NUM + 2];
    memset(buf, 0, NUM + 2);
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
    memset(buf, 0, NUM + 2);

    printf("\nEntry max price for show: ");
    if (!fgets(buf, NUM + 2, stream) || input_num(&(tmp->max_price), buf))
        return READ_ERR;
    memset(buf, 0, NUM + 2);

    printf("\nEntry\n0 if show for childs\n1 if show for adults\n2 if it's music show...\n ");
    if (!fgets(buf, NUM + 2, stream) || input_num((int *)&(tmp->category), buf))
        return READ_ERR;
    memset(buf, 0, NUM + 2);

    switch (tmp->category)
    {
    case KIDS:
        printf("Your choice - show for kids\n");
        printf("Entry min_age for child ticket: ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int *)&(tmp->choice.minor.min_age), buf) || tmp->choice.minor.min_age < 0)
            return READ_ERR;
        memset(buf, 0, NUM + 2);

        printf("\nEntry\n0 if show - fairy tale\n1 if show - piece...\n ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int *)&(tmp->choice.minor.type), buf))
            return READ_ERR;
        memset(buf, 0, NUM + 2);

        if (tmp->choice.minor.type != FAIRY_TALE && tmp->choice.minor.type != PIECE)
            return READ_ERR;
        strcpy(tmp->choice.minor.genre, genre[tmp->choice.minor.type]);
        printf("Struct successfull create.\n");
        break;
    case ADULTS:
        printf("Your choice - show for adults\n");
        printf("\nEntry\n1 if show - piece\n2 if show - drama\n3 if show - comedy...\n ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int *)&(tmp->choice.major.type), buf))
            return READ_ERR;
        memset(buf, 0, NUM + 2);

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
        if (!fgets(buf, NUM + 2, stream) || input_num((int *)&(tmp->choice.sound.min_age), buf))
            return READ_ERR;
        memset(buf, 0, NUM + 2);

        printf("\nEntry track duration: ");
        if (!fgets(buf, NUM + 2, stream) || input_num((int *)&(tmp->choice.sound.time), buf))
            return READ_ERR;
        memset(buf, 0, NUM + 2);

        printf("Struct successfull create.\n");
        break;
    default:
        return READ_ERR;
    }
    return EXIT_SUCCESS;
}
int read(theatre_t src[], size_t len, FILE *in, size_t cnt, size_t *id_count)
{
    if (!in || !src)
    {
        return READ_ERR;
    }
    int rc = 0;
    theatre_t tmp;

    for (size_t i = 0; i < cnt; i++)
    {
        rc = entry(&tmp, in);
        if (rc)
        {
            return READ_ERR;
        }
        tmp.id = *id_count;
        *id_count += 1;
        src[i + len] = tmp;
        memset(&tmp, 0, sizeof(theatre_t));
    }
    return rc;
}
int del(theatre_t src[], size_t *len, theatre_key_t dst[], size_t id)
{
    if (!src || !dst)
    {
        return ARG_ERR;
    }
    if (!*len)
    {
        return EMPTY_ARR;
    }
    int pos = -1;
    for (size_t i = 0; i < *len; i++)
    {
        if (src[i].id == id)
        {
            pos = i;
            break;
        }
    }
    if (pos == -1)
    {
        return ID_NOT_FOUND;
    }
    if (pos == *len - 1)
    {
        memset(src + pos, 0, sizeof(theatre_t));
        memset(dst + pos, 0, sizeof(theatre_key_t));
        *len -= 1;
        return EXIT_SUCCESS;
    }
    for (size_t i = pos; i < *len - 1; i++)
    {
        src[i] = src[i+1];
        dst[i].main_table = dst[i+1].main_table;
        dst[i].min_price = dst[i+1].min_price;
    }
    *len -= 1;
    return EXIT_SUCCESS;
}
int bubble_sort(theatre_t *src, size_t len)
{
    if (!src || !len)
    {
        return ARG_ERR;
    }
    for (size_t i = 0; i < len - 1; i++)
    {
        for (size_t j = 0; j < len - i - 1; j++)
        {
            if (src[j].min_price > src[j + 1].min_price)
            {
                swap_table(&src[j], &src[j+1]);
            }
        }
    }
    return EXIT_SUCCESS;
}   
void swap_table(theatre_t frst[], theatre_t sec[])
{
    theatre_t tmp = *frst;
    *frst = *sec;
    *sec = tmp;
}
void swap_keys(theatre_key_t frst[], theatre_key_t sec[])
{
    theatre_key_t tmp = *frst;
    *frst = *sec;
    *sec = tmp;
}

int bubble_sort_keys(theatre_key_t *keys, size_t len)
{
    if (!keys || !len)
    {
        return ARG_ERR;
    }
    for (size_t i = 0; i < len - 1; i++)
    {
        for (size_t j = 0; j < len - i - 1; j++)
        {
            if (keys[j].min_price> keys[j+1].min_price)
            {
                swap_keys(&keys[j], &keys[j+1]);
            }
        }
    }
    return EXIT_SUCCESS;
} 
void quick_sort_table(theatre_t src[], size_t first, size_t last)
{
    size_t i = first;
    size_t j = last;
    int x = src[(first + last) / 2].min_price;
    do
    {
        while (src[i].min_price < x)
        {
            i++;
        }
        while (src[j].min_price > x)
        {
            j--;
        }
        if (i <= j)
        {
            if (src[i].min_price > src[j].min_price)
            {
                swap_table(&src[i], &src[j]);
            }
            i++;
            j--;
        }
    } while (i <= j);   
        if (i < last)
        {
            quick_sort_table(src, i, last);
        }
        if (first < j)
        {
            quick_sort_table(src, first, j);
        }
}
void quick_sort_keys(theatre_key_t src[], size_t first, size_t last)
{
    size_t i = first;
    size_t j = last;
    int x = src[(first + last) / 2].min_price;
    while (i <= j)
    {
        while (src[i].min_price < x)
        {
            i++;
        }
        while (src[j].min_price > x)
        {
            j--;
        }
        if (i <= j)
        {
            if (src[i].min_price > src[j].min_price)
            {
                swap_keys(&src[i], &src[j]);
            }
            i++;
            j--;
        }
    }   
        if (i < last)
        {
            quick_sort_keys(src, i, last);
        }
        if (first < j)
        {
            quick_sort_keys(src, first, j);
        }
}    