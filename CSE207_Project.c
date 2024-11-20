#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_LEVEL 5// Maximum levels for the skip list

typedef struct SkipListNode {
    int key;
    struct SkipListNode** forward;
} SkipListNode;

typedef struct SkipList {
    int level;
    SkipListNode* header;
} SkipList;

SkipListNode* createNode(int key, int level) {
    SkipListNode* newNode = (SkipListNode*)malloc(sizeof(SkipListNode));
    newNode->key = key;
    newNode->forward = (SkipListNode**)malloc((level + 1) * sizeof(SkipListNode*));
    return newNode;
}

SkipList* createSkipList() {
    SkipList* list = (SkipList*)malloc(sizeof(SkipList));
    list->level = 0;
    list->header = createNode(-1, MAX_LEVEL);
    for (int i = 0; i <= MAX_LEVEL; i++) {
        list->header->forward[i] = NULL;
    }
    return list;
}

int randomLevel() {
    int level = 0;
    while (rand() < RAND_MAX / 2 && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

void insert(SkipList* list, int key) {
    SkipListNode* update[MAX_LEVEL + 1];
    SkipListNode* current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current == NULL || current->key != key) {
        int newLevel = randomLevel();

        if (newLevel > list->level) {
            for (int i = list->level + 1; i <= newLevel; i++) {
                update[i] = list->header;
            }
            list->level = newLevel;
        }

        SkipListNode* newNode = createNode(key, newLevel);

        for (int i = 0; i <= newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
}

void delete(SkipList* list, int key) {
    SkipListNode* update[MAX_LEVEL + 1];
    SkipListNode* current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];

    if (current != NULL && current->key == key) {
        for (int i = 0; i <= list->level; i++) {
            if (update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }
        free(current);
        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;
        }
    }
}

SkipListNode* search(SkipList* list, int key) {
    SkipListNode* current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current != NULL && current->key == key) {
        return current;
    } else {
        return NULL;
    }
}

void display(SkipList* list) {
    printf("Skip List (level %d):\n", list->level);

    for (int i = list->level; i >= 0; i--) {
        SkipListNode* current = list->header->forward[i];
        printf("Level %d: ", i);

        while (current != NULL) {
            printf("%d ", current->key);
            current = current->forward[i];
        }

        printf("\n");
    }
}

int main() {
    int choice, key,n;

    while (1) {
        printf("\nSkip List Operations:\n");
        printf("1. Create\n2. Insert\n3. Delete\n4. Search\n5. Display\n6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                srand(time(NULL));
                SkipList* list = createSkipList();
                printf("Enter the number of values in Skiplist: ");
                scanf("%d",&n);
                printf("Enter keys to insert: ");
                while(n--){
                scanf("%d", &key);
                insert(list, key);
                }
                break;
            case 2:
                printf("Enter the key to insert: ");
                scanf("%d", &key);
                insert(list, key);
                break;
            case 3:
                printf("Enter the key to delete: ");
                scanf("%d", &key);
                delete(list, key);
                break;
            case 4:
                printf("Enter the key to search: ");
                scanf("%d", &key);
                if (search(list, key) != NULL) {
                    printf("Key %d found in the Skip List.\n", key);
                } else {
                    printf("Key %d not found in the Skip List.\n", key);
                }
                break;
            case 5:
                display(list);
                break;
            case 6:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

