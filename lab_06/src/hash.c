#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"


#define ALLOC_ERR 1
#define MAX_COLLISION 2
#define NOT_FOUND 3

#define INIT_SIZE 53

static ht_item HT_DELETED_ITEM = {0, 0};



hash_table *htHashTableCreate(int init_size) {
    hash_table *ht = calloc(1, sizeof(hash_table));
    if (!ht) {
        return NULL;
    }
    ht->base_size = init_size;
    ht->size = next_prime(init_size);
    ht->count = 0;
    ht->ht_items = calloc(ht->size, sizeof(ht_item *));
    if (!ht->ht_items) {
        free(ht);
        return NULL;
    }

    return ht;
}
hash_table *ht_new() {
    return htHashTableCreate(INIT_SIZE);
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
static void htResize(hash_table *ht, const int base_size) {
    if (base_size < INIT_SIZE) {
        return;
    }
    hash_table *new_ht = htHashTableCreate(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item *item = ht->ht_items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            htInsert(new_ht, item->value, INIT_SIZE);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item **tmp_items = ht->ht_items;
    ht->ht_items = new_ht->ht_items;
    new_ht->ht_items = tmp_items;

    htHashTableDestroy(new_ht);
}
void reHash(hash_table *ht) {
    hash_table *new_ht = htHashTableCreate(ht->base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item *item = ht->ht_items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            htInsertHashStr(new_ht, item->value, ht->base_size);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item **tmp_items = ht->ht_items;
    ht->ht_items = new_ht->ht_items;
    new_ht->ht_items = tmp_items;

    htHashTableDestroy(new_ht);
}

static void htResizeUp(hash_table* ht) {
    const int new_size = ht->base_size * 2;
    htResize(ht, new_size);
}


static void htResizeDown(hash_table* ht) {
    const int new_size = ht->base_size / 2;
    htResize(ht, new_size);
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
    const int load = table->count * 100 / table->size;
    if (load > 70) {
        htResizeUp(table);
    }
    ht_item *item = htHashItemCreate(value);
    if (!item) {
        return ALLOC_ERR;
    }
    int index = htHashDiv(value, table->size);
    ht_item *cur_item = table->ht_items[index];

    int i = 0;
    while (cur_item != NULL && i < max_iter && item != &HT_DELETED_ITEM) {
        if (value == cur_item->value) {
            htHashItemDestroy(cur_item);
            table->ht_items[index] = item;
            return EXIT_SUCCESS;
        }
        index++;
        if (index >= table->size) {
            index = 0;
        }
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
int htInsertHashStr(hash_table *table, const int value, int max_iter) {
    const int load = table->count * 100 / table->size;
    if (load > 70) {
        htResizeUp(table);
    }
    ht_item *item = htHashItemCreate(value);
    if (!item) {
        return ALLOC_ERR;
    }
    int a = next_prime(table->base_size);
    int index = htHashStr(value, a, table->size);
    ht_item *cur_item = table->ht_items[index];

    int i = 1;
    while (cur_item != NULL && item != &HT_DELETED_ITEM) {
        if (value == cur_item->value) {
            htHashItemDestroy(cur_item);
            table->ht_items[index] = item;
            return EXIT_SUCCESS;
        }
        index++;
        if (index == table->size) {
            index = index % table->size;
        }
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
    int i = 0;
    while (item != NULL && item != &HT_DELETED_ITEM && i < table->size) {
        if (value == item->value) {
            return item;
        }
        index++;
        if (index % table->size == 0) {
            index = 0;
        }
        item = table->ht_items[index];
        i++;
    }
    return NULL;
}
ht_item *htSearchHashStr(hash_table *table, int value) {
    int index = htHashStr(value, table->size, table->size);
    ht_item *item = table->ht_items[index];
    int i = 0;
    while (item != NULL && item != &HT_DELETED_ITEM && i < table->size) {
        if (value == item->value) {
            return item;
        }
        index++;
        i++;
          if (index % table->size == 0) {
            index = 0;
        }
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
    const int load = table->count * 100 / table->size;
    if (load < 10) {
        htResizeDown(table);
    }
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
int htDeleteHashStr(hash_table *table, int value) {
    const int load = table->count * 100 / table->size;
    if (load < 10) {
        htResizeDown(table);
    }
    int index = htHashStr(value,table->size, table->size);
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
    if (!*ht) {
        *ht = htHashTableCreate(INIT_SIZE);
    }
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
