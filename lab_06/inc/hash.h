#ifndef __HASH_H_
#define __HASH_H_


#ifndef NDEBUG
#define DBG_PRINT(s, ...) printf(s, __VA_ARGS__)
#else
#define DBG_PRINT(s, ...) ((void) 0)
#endif

typedef struct {
    // int key;
    int value;
} ht_item;

typedef struct {
    int size; //prime num
    int base_size;
    int count;
    ht_item **ht_items;
} hash_table;

hash_table *htHashTableCreate(int size);

void htHashTableDestroy(hash_table *table);

// ht_item *htHashItemCreate(int key, int value);
ht_item *htHashItemCreate(int value);

void htHashItemDestroy(ht_item *item);

// int htHashDiv(int key, int ht_size);
int htHashDiv(int value, int ht_size);

// int htHashStr(const int key, const int a, const int ht_size);
int htHashStr(const int value, const int a, const int ht_size);

// int htInsert(hash_table *table, const int key, const int value, const int max_iter);

int htInsert(hash_table *table, const int value, const int max_iter);

// ht_item *htSearch(hash_table *table, int key);

// int htDelete(hash_table *table, int key);
ht_item *htSearch(hash_table *table, int value);

int htDelete(hash_table *table, int value);

int readHashFromFile(hash_table **ht, const char *f_name, int cmp_k);

int htFillPercent(hash_table *ht);

void printTable(hash_table *table);

int is_prime(const int x);

int next_prime(int x);

void reHash(hash_table *ht);

int htInsertHashStr(hash_table *table, const int value, int max_iter);
int htDeleteHashStr(hash_table *table, int value);
ht_item *htSearchHashStr(hash_table *table, int value);



#endif // __HASH_H__