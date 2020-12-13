#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_ERR 1
#define MAX_COLLISION 2
#define NOT_FOUND 3

#include "hash.h"

hash_table *htHashTableCreate(int init_size) {
    hash_table *ht = calloc(1, sizeof(hash_table));
    if (!ht) {
        return NULL;
    }
    ht->ht_items = calloc(init_size, sizeof(ht_item *));
    if (!ht->ht_items) {
        free(ht);
        return NULL;
    }
    ht->size = init_size;
    ht->count = 0;

    return ht;
}

void htHashTableDestroy(hash_table *table) {
    for (int i = 0; i < table->size; i++) {
        if (table->ht_items[i]) {
            htHashItemDestroy(table->ht_items[i]);
        }
    }
    free(table->ht_items);
    free(table);
}

// ht_item *htHashItemCreate(int key, int value) {
//     ht_item *item = malloc(sizeof(ht_item));
//     if (!item) {
//         return NULL;
//     }
//     item->key = key;
//     item->value = value;
//     return item;
// }
ht_item *htHashItemCreate(int value) {
    ht_item *item = malloc(sizeof(ht_item));
    if (!item) {
        return NULL;
    }
    item->value = value;
    return item;
}

void htHashItemDestroy(ht_item *item) {
    free(item);
}
// int htHashDiv(int key, int ht_size) {
//     return key % ht_size;
// }
int htHashDiv(int value, int ht_size) {
    return (abs(value)) % ht_size;
}
// int htHashStr(const int key, const int a, const int ht_size) {
//     long long hash = 0;
//     char str[12] = {};
//     snprintf(str, 12, "%d", key);
//     const int s_len = strlen(str);

//     for (int i = 0; i < s_len; ++i) {
//         hash += (long long)pow(a, s_len - (i + 1)) * str[i];
//         hash = hash % ht_size;
//     }

//     return (int)hash;
// }
int htHashStr(const int value, const int a, const int ht_size) {
    long long hash = 0;
    char str[12] = {0};
    snprintf(str, 12, "%d", value);
    const int s_len = strlen(str);

    for (int i = 0; i < s_len; ++i) {
        hash += (long long)pow(a, s_len - (i + 1)) * str[i];
        hash = hash % ht_size;
    }

    return (int)hash;
}
static ht_item HT_DELETED_ITEM = {0, 0};

// int htInsert(hash_table *table, const int key, const int value, const int max_iter) {
//     ht_item *item = htHashItemCreate(key, value);
//     if (!item) {
//         return ALLOC_ERR;
//     }
//     int index = htHashDiv(key, table->size);
//     ht_item *cur_item = table->ht_items[index];

//     int i = 1;
//     while (cur_item != NULL && i < max_iter && item != &HT_DELETED_ITEM) {
//         if (key == cur_item->key) {
//             htHashItemDestroy(cur_item);
//             table->ht_items[index] = item;
//             return EXIT_SUCCESS;
//         }
//         index++;
//         cur_item = table->ht_items[index];
//         i++;
//     }
//     if (i == max_iter) {
//         htHashItemDestroy(cur_item);
//         return MAX_COLLISION;
//     }
//     table->ht_items[index] = item;
//     table->count++;
//     return EXIT_SUCCESS;
// }

int htInsert(hash_table *table, const int value, const int max_iter) {
    ht_item *item = htHashItemCreate(value);
    if (!item) {
        return ALLOC_ERR;
    }
    int index = htHashDiv(value, table->size);
    ht_item *cur_item = table->ht_items[index];

    int i = 1;
    while (cur_item != NULL && i < max_iter && item != &HT_DELETED_ITEM) {
        if (value == cur_item->value) {
            htHashItemDestroy(cur_item);
            table->ht_items[index] = item;
            return EXIT_SUCCESS;
        }
        index++;
        cur_item = table->ht_items[index];
        i++;
    }
    if (i == max_iter) {
        htHashItemDestroy(item);
        return MAX_COLLISION;
    }
    table->ht_items[index] = item;
    table->count++;
    return EXIT_SUCCESS;
}

// ht_item *htSearch(hash_table *table, int key) {
//     int index = htHashDiv(key, table->size);
//     ht_item *item = table->ht_items[index];
//     while (item != NULL && item != &HT_DELETED_ITEM) {
//         if (key == item->key) {
//             return item;
//         }
//         index++;
//         item = table->ht_items[index];
//     }
//     return NULL;
// }
ht_item *htSearch(hash_table *table, int value) {
    int index = htHashDiv(value, table->size);
    ht_item *item = table->ht_items[index];
    while (item != NULL && item != &HT_DELETED_ITEM) {
        if (value == item->value) {
            return item;
        }
        index++;
        item = table->ht_items[index];
    }
    return NULL;
}

// int htDelete(hash_table *table, int key) {
//     int index = htHashDiv(key, table->size);
//     ht_item *item = table->ht_items[index];
//     while (item != NULL && item != &HT_DELETED_ITEM) {
//         if (item->key == key) {
//             htHashItemDestroy(item);
//             table->ht_items[index] = &HT_DELETED_ITEM;
//         }
//         index++;
//         item = table->ht_items[index];
//     }
//     table->count--;
//     return EXIT_SUCCESS;
// }
int htDelete(hash_table *table, int value) {
    int index = htHashDiv(value, table->size);
    ht_item *item = table->ht_items[index];
    while (item != NULL && item != &HT_DELETED_ITEM) {
        if (item->value == value) {
            htHashItemDestroy(item);
            table->ht_items[index] = &HT_DELETED_ITEM;
        }
        index++;
        item = table->ht_items[index];
    }
    table->count--;
    return EXIT_SUCCESS;
}

void printTable(hash_table *table) {
    int flag = 0;
    int i = 0;
    while (i < table->size) {
        ht_item *item = table->ht_items[i];
        if (item && item != &HT_DELETED_ITEM) {
            flag = 1;
            printf("%d\n", item->value);
        }
        i++;
    }
    if (!flag) {
        puts("Таблица пуста");
    }
}
int readHashFromFile(hash_table **ht, const char *f_name, int cmp_k) {
    FILE *in = fopen(f_name, "r");
    if (!in) {
        DBG_PRINT("%s", "OPEN FAILED\n");
        return -1;
    }
    *ht = htHashTableCreate(INIT_SIZE);
    if (!*ht) {
        fclose(in);
        return -1;
    }
    int x = 0;
    while (fscanf(in, "%d", &x) == 1) {
        if (htSearch(*ht, x) != NULL) {
            DBG_PRINT("%s", "not uniq elem\n");
            fclose(in);
            htHashTableDestroy(*ht);
            return -1;
        }
        if (htInsert(*ht, x, cmp_k)) {
            DBG_PRINT("%s", "кол-во коллизий больше введённого\n");
            fclose(in);
            htHashTableDestroy(*ht);
            return -1;
        }
    }

    if (ferror(in) || !feof(in)) {
        htHashTableDestroy(*ht);
        return -1;
    }
    fclose(in);
    DBG_PRINT("%s", "SUCCESS READ\n");
    return EXIT_SUCCESS;
}
int htFillPercent(hash_table *ht) {
    return (ht->count / (double)ht->size) * 100;
}


/*
 * Return whether x is prime or not
 *
 * Returns:
 *   1  - prime
 *   0  - not prime
 *   -1 - undefined (i.e. x < 2)
 */
int is_prime(const int x) {
    if (x < 2) {
        return -1;
    }
    if (x < 4) {
        return 1;
    }
    if ((x % 2) == 0) {
        return 0;
    }
    for (int i = 3; i <= floor(sqrt((double)x)); i += 2) {
        if ((x % i) == 0) {
            return 0;
        }
    }
    return 1;
}

/*
 * Return the next prime after x, or x if x is prime
 */
int next_prime(int x) {
    while (is_prime(x) != 1) {
        x++;
    }
    return x;
}

static hash_table* ht_new_sized(const int base_size) {
 hash_table* ht = xmalloc(sizeof(hash_table));
 ht->base_size = base_size;

 ht->size = next_prime(ht->base_size);

 ht->count = 0;
 ht->items = xcalloc((size_t)ht->size, sizeof(ht_item*));
 return ht;
}


ht_hash_table* ht_new() {
 return ht_new_sized(HT_INITIAL_BASE_SIZE);
}