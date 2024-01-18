/**
 * Example of a main() function using a binary search tree (BST),
 * reading information about plants to add one by one from the
 * standard input. See BST tasks in Lab 3.
 *
 * Course: High Performance Programming, Uppsala University
 *
 * Authors: Anastasia Kruchinina <anastasia.kruchinina@it.uu.se>
 *          Elias Rudberg <elias.rudberg@it.uu.se>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tree_node
{
   int              ID;
   char             *name;
   struct tree_node *left;
   struct tree_node *right;
} node_t;

node_t* newNode(int ID, char* name) {
    node_t* node = (node_t*)malloc(sizeof(node_t));
    node->ID = ID;
    node->name = strdup(name);
    node->left = node->right = NULL;
    return node;
}

void print_bst(node_t *node)
{
   if(node == NULL) {printf("Tree is empty!\n"); return;}

   if (node != NULL) printf("%d %s: \t", node->ID, node->name);
   if (node->left != NULL) printf("L%d,", node->left->ID);
   if (node->right != NULL) printf("R%d", node->right->ID);
   printf("\n");

   if (node->left != NULL)
      print_bst(node->left);
   if (node->right != NULL)
      print_bst(node->right);
}


void delete_tree(node_t **node)
{
  if (*node == NULL) {
        return;
    }

    delete_tree(&(*node)->left);
    delete_tree(&(*node)->right);

    free((*node)->name);
    free(*node);

    *node = NULL;
}

void insert(node_t **node, int ID, char *name)
{
  if (*node == NULL) {
        *node = newNode(ID, name);
    } else if (ID < (*node)->ID) {
        insert(&(*node)->left, ID, name);
    } else if (ID > (*node)->ID) {
        insert(&(*node)->right, ID, name);
    }
}

char* search(node_t *node, int ID) {
    if (node == NULL) {
        return NULL;
    } else if (ID < node->ID) {
        return search(node->left, ID);
    } else if (ID > node->ID) {
        return search(node->right, ID);
    } else {
        return node->name;
    }
}


int main(int argc, char const *argv[])
{
   node_t *root = NULL;

   while(1) {
     printf("================================================\n");
     printf("Please enter ID of plant to add (zero to stop): ");
     int id;
     int r = scanf("%d", &id);
     if(r != 1 || id == 0) {
       printf("Breaking loop now.\n");
       break;
     }
     printf("OK, you entered id %d\n", id);
     printf("Please enter name of plant to add: ");
     char nameString[100]; /* Note: this will only work if the name entered is less than 100 characters long! */
     r = scanf("%s", nameString);
     if(r != 1) {
       printf("Failed to read name, breaking loop now.\n");
       break;
     }
     printf("OK, you have entered the following:  id = %d  nameString = %s\n", id, nameString);
     printf("Now insterting that into the tree...\n");
     insert(&root, id, nameString);
     printf("Now printing the tree:\n");
     print_bst(root);
   }

   printf("After loop.\n");
   
   delete_tree(&root);
   print_bst(root);
   return 0;
}
