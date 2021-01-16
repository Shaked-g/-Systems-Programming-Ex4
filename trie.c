#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define NUM_LETTERS ((int)26)
typedef enum
{
    NO = 0,
    YES = 1
} boolean;

typedef struct node
{
    char letter;
    long unsigned int count;
    boolean is_word;
    boolean has_kids;
    struct node *children[NUM_LETTERS];
} node;

typedef struct trie
{
    node *children[NUM_LETTERS];
    node *current;
    boolean empty;
    int max_word_length;
    char *word;
} trie;

void memory_allocation_error(void)
{
    printf("Memory allocation error. exiting the program\n");
    exit(1); // indicates unsucessful termination
}

node *initialize_node(node *n, char letter)
{
    int i;
    n->letter = letter;
    n->count = 0;
    n->is_word = NO;
    n->has_kids = NO;
    for (i = 0; i < NUM_LETTERS; ++i)
    {
        (n->children)[i] = NULL;
    }
    return n;
}

node *create_node(char letter)
{
    node *created_node;
    created_node = (node *)malloc(sizeof(node));
    if (created_node == 0)
    {
        memory_allocation_error();
    }

    //created_node=new_node();
    return initialize_node(created_node, letter);
}

// free up the space of the node and all his childrens
void free_node(node *n)
{

    int i;
    if (n == NULL)
    {
        return;
    }
    if (0 == (n->has_kids))
    {

        free(n);
    }
    else
    {
        for (i = 0; i < NUM_LETTERS; ++i)
        {
            free_node((n->children)[i]);
        }
    }
    return;
}

trie *initialize_trie(trie *t)
{
    int i;
    for (i = 0; i < NUM_LETTERS; ++i)
    {
        t->children[i] = NULL;
    }
    t->current = NULL;
    t->empty = YES;
    t->max_word_length = 0;
    return t;
}

trie *create_trie(void)
{
    trie *created_trie;
    created_trie = (trie *)malloc(sizeof(trie));
    if (created_trie == 0)
    {
        memory_allocation_error();
    }

    return initialize_trie(created_trie);
}

void close_word(trie *root)
{
    if (root->current == NULL)
        return;
    root->current->count++;
    root->current->is_word = YES;
    root->current = NULL;
}

boolean is_empty(trie *root)
{
    return root->empty;
}

int read_character(trie *root, int c)
{
    int index;
    int word_length = 0;
    if (!isalpha(c))
    {
        close_word(root);
        return word_length;
    }
    word_length++;
    c = tolower(c);
    index = (c - 'a');
    if (root->current == NULL)
    { // new word - starts from root
        if (root->children[index] == NULL)
            root->children[index] = create_node(c);
        root->current = root->children[index];
        root->empty = NO;
    }
    else
    {
        root->current->has_kids = YES;
        if (root->current->children[index] == NULL)
            root->current->children[index] = create_node(c);
        root->current = root->current->children[index];
    }
    return word_length;
}

void allocate_word(trie *root)
{
    free(root->word);
    if (!(root->word = (char *)malloc(1 + sizeof(char) * (root->max_word_length))))
        memory_allocation_error();
}

trie *read_text()
{
    int c;
    int word_length;
    trie *root;
    root = create_trie();
    while (EOF != (c = getchar()))
    {
        word_length = read_character(root, c);
        if (word_length > root->max_word_length)
            root->max_word_length = word_length;
    }
    allocate_word(root);
    return root;
}

void print_words_reverse(trie *root)
{
    static int p = 0;
    int i;
    node *current;
    root->word[p++] = root->current->letter;
    if (root->current->has_kids)
    {
        for (i = NUM_LETTERS - 1; i >= 0; --i)
        {
            if (root->current->children[i] == NULL)
                continue;
            current = root->current; /* remember */
            root->current = root->current->children[i];
            print_words_reverse(root);
            root->current = current; /* recall */
        }
    }
    else
    {
        if (root->current->is_word)
        {
            root->word[p] = '\0';
            printf("%s %ld\n", root->word, root->current->count);
        }
        --p;
        return;
    }
    if (root->current->is_word)
    {
        root->word[p] = '\0';
        printf("%s %ld\n", root->word, root->current->count);
    }
    --p;
}

void print_trie_reverse(trie *root)
{
    int i;
    if (root == NULL)
        return;
    if (is_empty(root))
        return;
    for (i = NUM_LETTERS - 1; i >= 0; --i)
    {
        if (root->children[i] == NULL)
            continue;
        root->current = root->children[i];
        print_words_reverse(root);
    }
}

void print_words(trie *root)
{
    static int p = 0;
    int i;
    node *current;
    root->word[p++] = root->current->letter;
    if (root->current->is_word)
    {
        root->word[p] = '\0';
        printf("%s %ld\n", root->word, root->current->count);
    }
    if (root->current->has_kids)
    {
        for (i = 0; i < NUM_LETTERS; ++i)
        {
            if (root->current->children[i] == NULL)
                continue;
            current = root->current; /* remember */
            root->current = root->current->children[i];
            print_words(root);
            root->current = current; /* recall */
        }
    }
    else
    {
        --p;
        return;
    }
    --p;
}

void print_trie(trie *root)
{

    int i;
    if (root == NULL)
        return;
    if (is_empty(root))
        return;
    for (i = 0; i < NUM_LETTERS; ++i)
    {
        if (root->children[i] == NULL)
            continue;
        root->current = root->children[i];
        print_words(root);
    }
}

void free_trie(trie *t)
{
    int i;
    if (t == NULL)
        return;
    for (i = 0; i < NUM_LETTERS; ++i)
    {
        free_node(t->children[i]);
    }
    free(t);
}

int main(int argc, char *argv[])
{
    trie *root;
    root = read_text();

    // only for input "./frequency" runs the first program
    if (argc == 1)
    {
        print_trie(root);
    }

    // only for input "./frequency r" or "./frequency R" runs the second program
    if ((argc == 2) && (argv[1][0] == 'r' || argv[1][0] == 'R'))
    {
        print_trie_reverse(root);
    }

    free_trie(root);
    return 0;
}