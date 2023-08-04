#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TrieNode {
    int count;
    struct TrieNode* children[26];
};

struct TrieNode* createTrieNode() {
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (newNode) {
        newNode->count = 0;
        for (int i = 0; i < 26; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Trie structure
struct Trie {
    struct TrieNode* root;
};

// Initializes a trie structure
struct Trie* createTrie() {
    struct Trie* newTrie = (struct Trie*)malloc(sizeof(struct Trie));
    if (newTrie) {
        newTrie->root = createTrieNode();
    }
    return newTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie* pTrie, char* word) {
    struct TrieNode* currentNode = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (index < 0 || index >= 26) {
            return;
        }
        if (currentNode->children[index] == NULL) {
            currentNode->children[index] = createTrieNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->count++;
}

// computes the number of occurances of the word
int numberOfOccurrences(struct Trie* pTrie, char* word) {
    struct TrieNode* currentNode = pTrie->root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (currentNode->children[index] == NULL) {
            return 0;
        }
        currentNode = currentNode->children[index];
    }
    return currentNode->count;
}

struct TrieNode* deallocateTrieNode(struct TrieNode* node) {
    if (node) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                deallocateTrieNode(node->children[i]);
            }
        }
        free(node);
    }
    return NULL;
}

// deallocate the trie structure
struct Trie* deallocateTrie(struct Trie* pTrie) {
    if (pTrie) {
        pTrie->root = deallocateTrieNode(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Can't open file%s\n", filename);
        exit(1);
    }

    int numWords = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];

	//read the number of the words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

	// parse lineby line, and insert each word to the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    // Deallocate the trie data structure
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL) {
        printf("There is an error in this program\n");
    }

    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}