#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode
{
    char name[50];
    int is_folder;
    struct TreeNode *parent;
    struct TreeNode *first_child;
    struct TreeNode *next_sibling;
};

struct TreeNode *root = NULL;

struct TreeNode *create_node(const char *name, int is_folder, struct TreeNode *parent)
{
    struct TreeNode *new_node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    strcpy(new_node->name, name);
    new_node->is_folder = is_folder;
    new_node->parent = parent;
    new_node->first_child = NULL;
    new_node->next_sibling = NULL;
    return new_node;
}

void create_folder(const char *name, struct TreeNode *parent)
{
    struct TreeNode *new_folder = create_node(name, 1, parent);
    if (parent->first_child == NULL)
    {
        parent->first_child = new_folder;
    }
    else
    {
        struct TreeNode *current = parent->first_child;
        while (current->next_sibling != NULL)
        {
            current = current->next_sibling;
        }
        current->next_sibling = new_folder;
    }
}

void add_file(const char *name, struct TreeNode *parent)
{
    struct TreeNode *new_file = create_node(name, 0, parent);
    if (parent->first_child == NULL)
    {
        parent->first_child = new_file;
    }
    else
    {
        struct TreeNode *current = parent->first_child;
        while (current->next_sibling != NULL)
        {
            current = current->next_sibling;
        }
        current->next_sibling = new_file;
    }
}

void print_tree(struct TreeNode *node, int depth)
{
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("|-- %s\n", node->name);

    struct TreeNode *current = node->first_child;
    while (current != NULL)
    {
        print_tree(current, depth + 1);
        current = current->next_sibling;
    }
}

// Function to find a node by name
struct TreeNode *find_node(struct TreeNode *current, const char *name)
{
    if (current == NULL)
    {
        return NULL;
    }
    if (strcmp(current->name, name) == 0)
    {
        return current;
    }
    struct TreeNode *found = find_node(current->first_child, name);
    if (found != NULL)
    {
        return found;
    }
    return find_node(current->next_sibling, name);
}

// Function to move an item to a new location
void move_item(const char *item_name, const char *new_location_name)
{
    struct TreeNode *item = find_node(root, item_name);
    if (item == NULL)
    {
        printf("Item '%s' not found.\n", item_name);
        return;
    }

    struct TreeNode *new_location = find_node(root, new_location_name);
    if (new_location == NULL || !new_location->is_folder)
    {
        printf("Location '%s' is not a valid folder.\n", new_location_name);
        return;
    }

    if (item->parent != NULL)
    {
        // Remove item from its current location
        if (item->parent->first_child == item)
        {
            item->parent->first_child = item->next_sibling;
        }
        else
        {
            struct TreeNode *current = item->parent->first_child;
            while (current->next_sibling != item)
            {
                current = current->next_sibling;
            }
            current->next_sibling = item->next_sibling;
        }
    }

    // Add item to the new location
    if (new_location->first_child == NULL)
    {
        new_location->first_child = item;
    }
    else
    {
        struct TreeNode *current = new_location->first_child;
        while (current->next_sibling != NULL)
        {
            current = current->next_sibling;
        }
        current->next_sibling = item;
    }

    item->parent = new_location;
    item->next_sibling = NULL;

    printf("Moved '%s' to '%s'.\n", item_name, new_location_name);
}

int main()
{
    root = create_node("root", 1, NULL);

    create_folder("documents", root);
    create_folder("photos", root);
    add_file("file1.txt", root);
    add_file("file2.txt", root->first_child);

    create_folder("summer", root->first_child);
    add_file("vacation.jpg", root->first_child->first_child);

    printf("File System Tree:\n");
    print_tree(root, 0);

    move_item("file1.txt", "documents");
    move_item("summer", "photos");

    printf("\nUpdated File System Tree:\n");
    print_tree(root, 0);

    // Free memory (in a real application, you would need to recursively free the nodes)
    free(root);

    return 0;
}