if (*node == NULL) {
        return;
    }

    delete_tree(&(*node)->left);
    delete_tree(&(*node)->right);

    free((*node)->name);
    free(*node);

    *node = NULL;