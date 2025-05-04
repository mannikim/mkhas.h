// see LICENSE file for copyright and license information

#if !defined(MKHAS_H)
#define MKHAS_H

#include <stdint.h> // uint64_t
#include <string.h> // strcmp() strlen() strcpy()

#define mkhas_htable(bucket_count) \
	{bucket_count, (struct HashTableNode *[bucket_count]){0}}

#if !defined(mkhas_malloc)
#include <stdlib.h> // malloc() free()
#define mkhas_MALLOC(x) malloc(x)
#endif

#if !defined(mkhas_free)
#include <stdlib.h> // malloc() free()
#define mkhas_FREE(x) free(x)
#endif

#if defined(mkhas_DEBUG)

#if !defined(mkhas_assert)
#include <assert.h> // assert()
#define mkhas_ASSERT(x) assert(x)
#endif

#else

#define mkhas_mkhas_ASSERT(x)

#endif

enum HashTableInsertError {
	mkhas_INSERT_ERR_OK = 0,
	// failed to allocate memory for new key
	mkhas_INSERT_ERR_ALLOC_FAILURE = 1,
	// hash table already has this key
	mkhas_INSERT_ERR_HAS_KEY = 2,
};

enum HashTableGetError {
	mkhas_GET_ERR_OK = 0,
	// hash table doesn't have this key
	mkhas_GET_ERR_NO_KEY = 1
};

struct HashTableNode {
	char *original;
	void *data;
	struct HashTableNode *next;
};

struct HashTableKey {
	size_t index;
	struct HashTableNode *value;
};

struct HashTable {
	size_t size;
	struct HashTableNode **buckets;
};

struct HashTableGetResult {
	enum HashTableGetError err;
	struct HashTableKey data;
};

struct HashTableInsertResult {
	enum HashTableInsertError err;
	struct HashTableKey data;
};

uint64_t
mkhas_h64(const char *str);

struct HashTableInsertResult
mkhas_insert(struct HashTable table, const char *string, void *data);

struct HashTableGetResult
mkhas_get(struct HashTable table, const char *string);

void
mkhas_remove(struct HashTable table, struct HashTableKey key);

void
mkhas_clear(struct HashTable table);

void
mkhas_free(struct HashTable table);

#if defined(mkhas_IMPL)

static char *
copy_string(const char *str)
{
	mkhas_ASSERT(str);

	size_t length = strlen(str);
	char *new = mkhas_MALLOC(length * sizeof(char) + 1);

	if (new == NULL)
		return NULL;

	strcpy(new, str);
	return new;
}

// FNV-1a hash
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
uint64_t
mkhas_h64(const char *str)
{
	mkhas_ASSERT(str);

	uint64_t hash = 14695981039346656037ull;

	while (*str)
	{
		hash ^= (unsigned char)(*str++);
		hash *= 1099511628211ULL;
	}

	return hash;
}

// please check the result to see if the insertion was successful
struct HashTableInsertResult
mkhas_insert(struct HashTable table, const char *string, void *data)
{
	mkhas_ASSERT(string);

	size_t index = mkhas_h64(string) % table.size;

	if (table.buckets[index] == NULL)
	{
		table.buckets[index] =
			mkhas_MALLOC(sizeof(struct HashTableNode));

		if (table.buckets[index] == NULL)
		{
			return (struct HashTableInsertResult){
				mkhas_INSERT_ERR_ALLOC_FAILURE, {0}};
		}

		table.buckets[index]->original = copy_string(string);

		if (table.buckets[index]->original == NULL)
		{
			mkhas_FREE(table.buckets[index]);
			table.buckets[index] = NULL;
			return (struct HashTableInsertResult){
				mkhas_INSERT_ERR_ALLOC_FAILURE, {0}};
		}

		table.buckets[index]->data = data;
		table.buckets[index]->next = NULL;

		return (struct HashTableInsertResult){
			mkhas_INSERT_ERR_OK,
			{index, table.buckets[index]},
		};
	}

	struct HashTableNode *parent = table.buckets[index];

	do
	{
		if (strcmp(string, parent->original) == 0)
		{
			return (struct HashTableInsertResult){
				mkhas_INSERT_ERR_HAS_KEY,
				{0},
			};
		}

		if (parent->next)
		{
			parent = parent->next;
			continue;
		}

		break;
	} while (1);

	parent->next = mkhas_MALLOC(sizeof(struct HashTableNode));

	if (parent->next == NULL)
	{
		return (struct HashTableInsertResult){
			mkhas_INSERT_ERR_ALLOC_FAILURE,
			{0},
		};
	}

	parent->next->original = copy_string(string);

	if (parent->next->original == NULL)
	{
		mkhas_FREE(parent->next);
		parent->next = NULL;

		return (struct HashTableInsertResult){
			mkhas_INSERT_ERR_ALLOC_FAILURE,
			{0},
		};
	}

	parent->next->data = data;
	parent->next->next = NULL;

	return (struct HashTableInsertResult){
		mkhas_INSERT_ERR_OK,
		{index, parent->next},
	};
}

struct HashTableGetResult
mkhas_get(struct HashTable table, const char *string)
{
	mkhas_ASSERT(string);

	size_t index = mkhas_h64(string) % table.size;

	struct HashTableNode *node = table.buckets[index];

	while (node != NULL)
	{
		if (strcmp(node->original, string) == 0)
		{
			return (struct HashTableGetResult){
				mkhas_GET_ERR_OK,
				{index, node},
			};
		}

		node = node->next;
	}

	return (struct HashTableGetResult){mkhas_GET_ERR_NO_KEY, {0}};
}

// removes an item from the table without deallocating the data
void
mkhas_remove(struct HashTable table, struct HashTableKey key)
{
	struct HashTableNode *node = table.buckets[key.index];

	if (node == key.value)
	{
		table.buckets[key.index] = key.value->next;
		mkhas_FREE(node->original);
		mkhas_FREE(node);
		return;
	}

	while (node->next != key.value)
	{
		node = node->next;
	}

	mkhas_FREE(node->next->original);
	node->next = node->next->next;
	mkhas_FREE(key.value);
}

// removes all items from the table without deallocating the data
void
mkhas_clear(struct HashTable table)
{
	for (size_t i = 0; i < table.size; i++)
	{
		if (table.buckets[i] == NULL)
			continue;

		struct HashTableNode *node = table.buckets[i];
		while (node != NULL)
		{
			struct HashTableNode *child = node->next;
			mkhas_FREE(node->original);
			mkhas_FREE(node);
			node = child;
		}
	}
}

// frees all items of a hash table. does not free the hash table itself
// WARNING this function generates a double-free if attempting to free
// an item which was already free-d
void
mkhas_free(struct HashTable table)
{
	for (size_t i = 0; i < table.size; i++)
	{
		if (table.buckets[i] == NULL)
			continue;

		struct HashTableNode *node = table.buckets[i];
		while (node != NULL)
		{
			struct HashTableNode *child = node->next;

			mkhas_FREE(node->data);
			mkhas_FREE(node->original);
			mkhas_FREE(node);

			node = child;
		}
	}
}

#endif // defined(mkhas_IMPL)
#endif // !defined(MKHAS_H)
