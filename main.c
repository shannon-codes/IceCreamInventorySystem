#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define FLUSH stdin=freopen(NULL,"r",stdin) 
#define MAX_PRODUCTS 200
#define MAX_LEN 1000
#define LEN_FLAVOUR 50
#define REMOVERN(str) str[strcspn(str,"\r\n")]=0

typedef struct {
    int id;
    char* flavour;
    char* description;
    double price;
    double rating;
} NodeData, *NodeDataPtr; //declare a data unit

typedef struct treeNode {
    NodeData* data; //general data
    struct treeNode*left;
    struct treeNode*right;
} TreeNode, *TreeNodePtr; //declare a tree node

typedef struct {
    TreeNodePtr root; //pointer to root
} BinarySearchTree;

void read_in();
void preOrderTraversal_writeOut(TreeNodePtr tree, FILE *fwp);
void inorderTraversal(TreeNodePtr tree);
void reverseInOrderTraversal(TreeNodePtr tree);
TreeNodePtr searchByFlavour(TreeNodePtr tree, char flavour[]);
TreeNodePtr newTreeNode(NodeDataPtr dataPtr);
TreeNodePtr insertElement(TreeNodePtr tree, NodeDataPtr dataPtr);
TreeNodePtr removeByFlavour(TreeNodePtr tree, char flavour[]);
void readEditByFlavour(TreeNodePtr tree);
void readEditByDesc(TreeNodePtr tree);
void readEditByPrice(TreeNodePtr tree);
void readEditByRating(TreeNodePtr tree);
void deleteTree(TreeNodePtr tree);
void write_out();
void readNewData(int);
unsigned int hashCode(char *key);
NodeDataPtr newNodeData(char flavour[], char description[], double price, double rating);
void visit(TreeNodePtr tree);

/*Print functions*/
void printMenu();
void printSearchItem();
void printRemoveItem();
void displayEntry(TreeNodePtr);
void listAscending(TreeNodePtr tree);
void listDescending(TreeNodePtr tree);
void removeFlavourPrompt(int);
void searchFlavourPrompt(int);
void editFlavourPrompt(int);
void listSelectionMenu();
void editFieldSelection(TreeNodePtr ptr);
void confirmationPrompt(char choice, TreeNodePtr tree, NodeDataPtr data);
void printAsciiArt();
/**Global variable*/
BinarySearchTree bst;
TreeNodePtr ptr = NULL;

int main() {
    bst.root = NULL;
    printAsciiArt();
    printf("\n************************ WELCOME TO SNOW DAY ICE CREAMS ************************\n\n");
    printf("Welcome to SNOW DAY ICE CREAMS ice cream inventory. What would you like to do?\n");
    read_in();
    printMenu();
}

/*Prints the Main Menu interface*/
void printMenu() {
    int choice;
    printf("\nMain menu:\n"
            "1. Search flavour\n"
            "2. Add flavour\n"
            "3. Edit flavour\n"
            "4. Remove flavour\n"
            "5. View flavour in ascending order\n"
            "6. View flavour in descending order\n"
            "7. Exit\n\n"
            "Enter choice here: ");
    scanf("%c", &choice);
    FLUSH;
    putchar('\n');
    switch (choice) {
        case '1':
            searchFlavourPrompt(0);
            printMenu();
            break;
        case '2':
            readNewData(0);
            printMenu();
            break;
        case '3':
            editFlavourPrompt(0);
            printMenu();
            break;
        case '4':
            removeFlavourPrompt(0);
            printMenu();
            break;
        case '5':
            listAscending(bst.root);
            printMenu();
            break;
        case '6':
            listDescending(bst.root);
            printMenu();
            break;
        case '7':
            printf("Thank you for running our code! Have a great day!\n");
            write_out();
            deleteTree(bst.root);
            return;
            break;
        default:
            printf("Sorry, your choice is not recognize.\n");
            putchar('\n');
            printMenu();
            break;
    }
}

void listAscending(TreeNodePtr tree) {
    printf("----------------------------------------------------------------\n");
    printf("%3s\t%-35s %s\t %s\n", "ID", "Flavour", "Price", "Rating");
    printf("----------------------------------------------------------------\n");
    inorderTraversal(tree);
    listSelectionMenu();
}

void listDescending(TreeNodePtr tree) {
    printf("----------------------------------------------------------------\n");
    printf("%3s\t%-35s %s\t %s\n", "ID", "Flavour", "Price", "Rating");
    printf("----------------------------------------------------------------\n");
    reverseInOrderTraversal(tree);
    listSelectionMenu();
}

/*Display the selected flavour in detail*/
void displayEntry(TreeNodePtr tree) {
    if (tree != NULL) {
        printf("\n----------------------------------------------------------------\n");
        printf("ID:\t\t%d\n"
                "Flavour:\t%s\n"
                "Description:    %s\n"
                "Price:\t\t%.2f\n"
                "Rating:\t\t%.2f\n",
                tree->data->id, tree->data->flavour,
                tree->data->description, tree->data->price, tree->data->rating);
        printf("----------------------------------------------------------------\n");
    } else {
        printf("Can't find that flavour.");
    }
}

/*Print list selection menu*/
void listSelectionMenu() {
    int choice;
    printf("\n============================================================================\n"
            "| 1 - View ice cream in the list     || 2 - Add ice cream to the list      |\n"
            "| 3 - Edit an ice cream in the list  || 4 - Remove a ice cream on the list |\n"
            "| 5 - View list in ascending order   || 6 - View list in descending order  |\n"
            "| 7 - Return to main menu ------------------------------------------------ |\n"
            "============================================================================\n");
    printf("Enter choice here: ");
    scanf("%c", &choice);
    FLUSH;
    putchar('\n');
    switch (choice) {
        case '1':
            searchFlavourPrompt(1);
            break;
        case '2':
            readNewData(1);
            break;
        case '3':
            editFlavourPrompt(1);
            break;
        case '4':
            removeFlavourPrompt(1);
            break;
        case '5':
            listAscending(bst.root);
            break;
        case '6':
            listDescending(bst.root);
            break;
        case '7':
            break;
        default:
            printf("Sorry, your choice is not recognize.\n");
            listSelectionMenu();
            break;
    }
}

/*Prompt to remove icecream*/
void removeFlavourPrompt(int option) {

    char flavour[LEN_FLAVOUR];
    char choice;
    printf("Enter flavour: ");
    fgets(flavour, sizeof (flavour), stdin);
    REMOVERN(flavour);
    FLUSH;
    ptr = searchByFlavour(bst.root, flavour);
    if (ptr == NULL) {
        printf("\nCan't find the flavour.\n");
        if (option == 1) {
            listSelectionMenu();
        }
    } else {
        printf("Are you sure you would like to remove %s? (Y/N): ", flavour);
        scanf("%c", &choice);
        FLUSH;
        switch (choice) {
            case 'y':
            case 'Y':
                bst.root = removeByFlavour(bst.root, flavour);
                printf("\nFlavour removed.\n");
                if (option == 1) {
                    listSelectionMenu();
                }
                break;
            case 'n':
            case 'N':
                if (option == 1) {
                    listSelectionMenu();
                }
                break;
            default:
                printf("\nInvalid input. Try again.\n\n");
                removeFlavourPrompt(0);
                break;
        }
    }
}

/*Prompt to search icecream flavour*/
void searchFlavourPrompt(int option) {
    char str[LEN_FLAVOUR];
    printf("Enter flavour: ");
    fgets(str, sizeof (str), stdin);
    REMOVERN(str);
    FLUSH;
    ptr = searchByFlavour(bst.root, str);
    if (ptr == NULL) {
        printf("Can't find the flavour.\n");
    } else {
        displayEntry(ptr);
        if (option == 1) {
            listSelectionMenu();
        }
    }
}

/*Prompt to edit icecream details*/
void editFlavourPrompt(int option) {
    char flavour[LEN_FLAVOUR];
    printf("Enter flavour: ");
    fgets(flavour, sizeof (flavour), stdin);
    REMOVERN(flavour);
    FLUSH;
    ptr = searchByFlavour(bst.root, flavour);
    if (ptr == NULL) {
        printf("Can't find the flavour.\n");
        if (option == 1) {
            listSelectionMenu();
        }
    } else {
        editFieldSelection(ptr);
        if (option == 1) {
            listSelectionMenu();
        }
    }
}

/*Prompt user which field to edit*/
void editFieldSelection(TreeNodePtr ptr) {
    int choice;
    printf("\nSelect the following options:\n"
            "1. Edit flavour name\n"
            "2. Edit description\n"
            "3. Edit price\n"
            "4. Edit rating\n"
            "5. Return to menu\n");
    printf("\nEnter choice here: ");
    scanf("%c", &choice);
    FLUSH;
    switch (choice) {
        case '1':
            readEditByFlavour(ptr);
            break;
        case '2':
            readEditByDesc(ptr);
            break;
        case '3':
            readEditByPrice(ptr);
            break;
        case '4':
            readEditByRating(ptr);
            break;
        case '5':
            printMenu();
            break;
        default:
            printf("Sorry, your choice is not recognize.\n");
            break;
    }
}

/*Reads the data csv file line by line, create tokens from each line, and 
 * calls the newNodeData function and InsertElement function to build a binary search tree*/
void read_in() {
    FILE *frp;
    const char *input_file = "IceCreamList_NoIDColumn.csv";
    char *arr[MAX_PRODUCTS];
    register int i = 0;
    char str[MAX_LEN];

    if ((frp = fopen(input_file, "r")) == NULL) {
        perror("Unable to open input file!\n");
        exit(1);
    }

    while (fgets(str, sizeof (str), frp) && i < MAX_PRODUCTS) {

        REMOVERN(str);

        if (strlen(str) > 0) {
            arr[i] = (char*) calloc(strlen(str) + 1, sizeof (char));

            if (arr[i] == NULL) {
                printf("Cannot allocate memory\n");
                exit(1);
            }
            strncpy(arr[i], str, strlen(str));
            i++;
        }
    }

    fclose(frp);

    /*split each line from the csv file into tokens*/

    for (int j = 0; j < i; j++) {

        char* token;
        token = strtok(arr[j], ",");
        char* tokensArray[4] = {0};
        int k = 0;
        while (token != NULL) {
            tokensArray[k++] = token;
            token = strtok(NULL, ",");
        }

        /*convert tokens into correct data type*/

        char* flavour = tokensArray[0];
        char* description = tokensArray[1];
        double price = strtod(tokensArray[2], NULL);
        double rating = strtod(tokensArray[3], NULL);

        /*insert new data into new NodeData struct on the heap and return its pointer*/
        NodeDataPtr dataPtr = newNodeData(flavour, description, price, rating);

        /*insert new NodeData into a new node in the tree and return the new tree*/
        bst.root = insertElement(bst.root, dataPtr);

    }
    /*free heap memory used for array holding the csv input lines*/
    for (int z = 0; z < i; z++) {
        if (arr[z] != NULL) {
            free(arr[z]);
        }
    }
}

NodeDataPtr newNodeData(char flavour[], char description[], double price, double rating) {
    NodeDataPtr dataPtr = (NodeDataPtr) malloc(sizeof (NodeData));

    dataPtr->id = hashCode(flavour);

    dataPtr->flavour = (char*) calloc(strlen(flavour) + 1, sizeof (char));
    if (dataPtr->flavour == NULL) {
        printf("Cannot allocate memory");
        exit(1);
    }
    strcpy(dataPtr->flavour, flavour);

    dataPtr->description = (char*) calloc(strlen(description) + 1, sizeof (char));
    if (dataPtr->description == NULL) {
        printf("Cannot allocate memory");
        exit(1);
    }
    strcpy(dataPtr->description, description);

    dataPtr->price = price;

    dataPtr->rating = rating;

    return dataPtr;
}

//Function to visit a node and print the data

void visit(TreeNodePtr tree) {
    printf("%3d\t%-35s %5.2f\t %6.2f\n", tree->data->id, tree->data->flavour, tree->data->price, tree->data->rating);
}

/*Show/View All ASC records by traversing binary search tree node by node In Order traversal*/
void inorderTraversal(TreeNodePtr tree) {
    if (tree != NULL) {
        inorderTraversal(tree -> left);
        visit(tree);
        inorderTraversal(tree -> right);
    }
}

/*Show/View All DESC records*/
void reverseInOrderTraversal(TreeNodePtr tree) {
    if (tree != NULL) {
        reverseInOrderTraversal(tree -> right);
        visit(tree);
        reverseInOrderTraversal(tree -> left);
    }
}

void preOrderTraversal_writeOut(TreeNodePtr tree, FILE *fwp) {
    if (tree != NULL) {
        fprintf(fwp, "%s,%s,%.2f,%.2f\n", tree->data->flavour, tree->data->description, tree->data->price, tree->data->rating);
        preOrderTraversal_writeOut(tree -> left, fwp);
        preOrderTraversal_writeOut(tree -> right, fwp);
    }
}

TreeNodePtr searchByFlavour(TreeNodePtr tree, char flavour[]) {
    if (tree == NULL || strcasecmp(tree->data->flavour, flavour) == 0)
        return tree;
    if (strcasecmp(tree->data->flavour, flavour) > 0)
        return searchByFlavour(tree->left, flavour);
    else
        return searchByFlavour(tree->right, flavour);
}

TreeNodePtr newTreeNode(NodeDataPtr dataPtr) {

    TreeNodePtr p = (TreeNodePtr) malloc(sizeof (TreeNode));
    p->data = dataPtr;
    p -> left = p -> right = NULL;

    return p;
}

TreeNodePtr insertElement(TreeNodePtr tree, NodeDataPtr dataPtr) {
    if (tree == NULL)
        tree = newTreeNode(dataPtr);
    else if (strcasecmp(tree->data->flavour, dataPtr->flavour) > 0)
        tree->left = insertElement(tree->left, dataPtr);
    else if (strcasecmp(tree->data->flavour, dataPtr->flavour) < 0)
        tree->right = insertElement(tree->right, dataPtr);
    else if (strcasecmp(tree->data->flavour, dataPtr->flavour) == 0) {
        printf("\nCan't insert, flavour already exists in the catalogue.\n\n");
        free(dataPtr->flavour);
        free(dataPtr->description);
        free(dataPtr);
    }
    return tree;
}

/**
 * Remove By Element Method to search the tree by the Flavour attribute
 * @param tree
 */
TreeNodePtr removeByFlavour(TreeNodePtr tree, char flavour[]) {
    if (tree == NULL) return tree;
    if (strcasecmp(tree->data->flavour, flavour) > 0) {
        tree->left = removeByFlavour(tree->left, flavour); //is flavour in left subtree
    } else if (strcasecmp(tree->data->flavour, flavour) < 0) {
        tree->right = removeByFlavour(tree->right, flavour); //is flavour in right subtree
    } else if (strcasecmp(tree->data->flavour, flavour) == 0) { //found
        if (tree->left == NULL) {
            TreeNodePtr temp = tree->right;
            free(tree);
            return temp;
        } else if (tree->right == NULL) {
            TreeNodePtr temp = tree->left;
            free(tree);
            return temp;
        }
        // re organize
        TreeNodePtr temp = tree->right;
        while (temp->left != NULL)
            temp = temp->left;
        tree->data = temp->data; //copy data to the node
        tree->right = removeByFlavour(tree->right, temp->data->flavour); //delete the successor
    }
    return tree;
}

/**
 * A method to read the new parameters for the TreeNode to be "edited"
 * A copy of the original node is being passed in  
 * @param tree
 */
void readEditByFlavour(TreeNodePtr tree) {

    char choice;
    char flavour[LEN_FLAVOUR];

    int validate = -1; //loop exit variable

    while (validate == -1) { //START LOOP: Ask for user input until data is validated
        validate = 0;
        printf("Enter new name of flavour: ");
        fgets(flavour, sizeof (flavour), stdin);
        REMOVERN(flavour);
        FLUSH;
        if (strlen(flavour) < 1) {
            validate = -1;
        }
        if (validate == -1)
            printf("Try Again. Invalid input(s) detected. \n\n");
    }

    //insert data into new NodeData struct
    NodeDataPtr dataPtr = newNodeData(flavour, tree->data->description, tree->data->price, tree->data->rating);
    ptr = NULL;
    printf("Are you sure you would like to edit? (Y/N): ");
    scanf("%c", &choice);
    FLUSH;
    confirmationPrompt(choice, tree, dataPtr);
}

/*Function to edit the description of an ice cream*/
void readEditByDesc(TreeNodePtr tree) {

    char choice;
    char description[251] = {0};

    int validate = -1; //loop exit variable

    while (validate == -1) { //START LOOP: Ask for user input until data is validated
        validate = 0;
        printf("Enter description: ");
        fgets(description, sizeof (description), stdin);
        REMOVERN(description);
        FLUSH;
        if (strlen(description) < 1) {
            validate = -1;
        }
        if (validate == -1)
            printf("Try Again. Invalid input(s) detected. \n\n");
    }

    //insert data into new NodeData struct
    NodeDataPtr dataPtr = newNodeData(tree->data->flavour, description, tree->data->price, tree->data->rating);
    ptr = NULL;
    printf("Are you sure you would like to edit? (Y/N): ");
    scanf("%c", &choice);
    FLUSH;
    confirmationPrompt(choice, tree, dataPtr);
}

/*Function to edit the price of an ice cream*/
void readEditByPrice(TreeNodePtr tree) {

    char choice;
    char priceStr[10] = {0};
    double price;

    int validate = -1; //loop exit variable

    while (validate == -1) { //START LOOP: Ask for user input until data is validated
        validate = 0;
        printf("Enter new price: ");
        fgets(priceStr, sizeof (priceStr), stdin);
        REMOVERN(priceStr);
        FLUSH;
        if (strlen(priceStr) < 1) {
            validate = -1;
        }
        char* cPtr = NULL;
        price = strtod(priceStr, &cPtr); // conversion to double
        if (!cPtr || *cPtr) { // check if conversion to double unsuccessfull 
            validate = -1;
        }
        if (validate == -1)
            printf("Try Again. Invalid input(s) detected. \n\n");
    }

    //insert data into new NodeData struct
    NodeDataPtr dataPtr = newNodeData(tree->data->flavour, tree->data->description, price, tree->data->rating);
    ptr = NULL;
    printf("Are you sure you would like to edit? (Y/N): ");
    scanf("%c", &choice);
    FLUSH;
    confirmationPrompt(choice, tree, dataPtr);
}

/*Function to edit the rating of an ice cream*/
void readEditByRating(TreeNodePtr tree) {

    char choice;
    char ratingStr[10] = {0};
    double rating;

    int validate = -1; //loop exit variable

    while (validate == -1) { //START LOOP: Ask for user input until data is validated
        validate = 0;
        printf("Enter rating between 1.00-10.00:\n");
        fgets(ratingStr, sizeof (ratingStr), stdin);
        REMOVERN(ratingStr);
        FLUSH;
        if (strlen(ratingStr) < 1) {
            validate = -1;
        }
        char* rPtr = NULL;
        rating = strtod(ratingStr, &rPtr); // conversion to double
        if (!rPtr || *rPtr) {
            validate = -1;
        } else {
            if (rating < 0 || rating > 10) { //validate input 1-10
                validate = -1;
            }
        }
        if (validate == -1)
            printf("Try Again. Invalid input(s) detected. \n\n");
    }

    //insert data into new NodeData struct
    NodeDataPtr dataPtr = newNodeData(tree->data->flavour, tree->data->description, tree->data->price, rating);
    ptr = NULL;
    printf("Are you sure you would like to edit? (Y/N): ");
    scanf("%c", &choice);
    FLUSH;
    confirmationPrompt(choice, tree, dataPtr);
}

/*Print confirmation to edit ice cream*/
void confirmationPrompt(char choice, TreeNodePtr tree, NodeDataPtr data) {

    switch (choice) {
        case 'y':
        case 'Y':
            removeByFlavour(bst.root, tree->data->flavour);
            //pass NodeDataPtr to new insert element
            bst.root = insertElement(bst.root, data);
            break;
        case 'n':
        case 'N':
            free(data);
            break;
        default:
            printf("\nInvalid input. Try again.\n\n");
            editFieldSelection(tree);
            break;
    }
}

/*Use Post-Order Traversal to traverse all nodes and delete them one by one*/
void deleteTree(TreeNodePtr tree) {
    if (tree != NULL) {
        deleteTree(tree->left);
        deleteTree(tree->right);
        free(tree->data->flavour);
        free(tree->data->description);
        free(tree->data);
        free(tree);
    }
}

/* I/O method to open existing text file or creates one if does not exist and 
 * calls function to preorder traverse all the nodes the binary tree and print node data line by line 
 * in formatted csv output file and close the stream so changes are saved*/
void write_out() {
    FILE *fwp;
    const char *output_file = "outputTest.csv";
    if ((fwp = fopen(output_file, "w")) == NULL) {
        perror("Could not open output file!\n");
        exit(1);
    }
    preOrderTraversal_writeOut(bst.root, fwp);

    fclose(fwp);
}

/*Add Item function: Get user input and validate, insert data into NodeData struct, and call function insertElement containing NodeData as its argument*/
void readNewData(int option) {

    char flavour[50] = {0};
    char description[251] = {0};
    char priceStr[10] = {0};
    double price;
    char ratingStr[10] = {0};
    double rating;

    int validate = -1; //loop exit variable

    while (validate == -1) { //START LOOP: Ask for user input until data is validated

        validate = 0;

        printf("Enter name of flavour: ");
        fgets(flavour, sizeof (flavour), stdin);
        REMOVERN(flavour);
        FLUSH;
        if (strlen(flavour) < 1) {
            validate = -1;
        }

        printf("Enter Description (less than 250 characters): ");
        fgets(description, sizeof (description), stdin);
        REMOVERN(description);
        FLUSH;
        if (strlen(description) < 1)
            validate = -1;

        printf("Enter Price: ");
        fgets(priceStr, sizeof (priceStr), stdin);
        REMOVERN(priceStr);
        FLUSH;
        if (strlen(priceStr) < 1)
            validate = -1;
        char* cPtr = NULL;
        price = strtod(priceStr, &cPtr); // conversion to double
        if (!cPtr || *cPtr) { // check if conversion to double unsuccessfull 
            validate = -1;
        }

        printf("Enter rating between 1.00-10.00: ");
        fgets(ratingStr, sizeof (ratingStr), stdin);
        REMOVERN(ratingStr);
        FLUSH;
        if (strlen(ratingStr) < 1)
            validate = -1;
        char* rPtr = NULL;
        rating = strtod(ratingStr, &rPtr); // conversion to double
        if (!rPtr || *rPtr) {
            validate = -1;
        } else {
            if (rating < 0 || rating > 10) { //validate input 1-10
                validate = -1;
            }
        }
        if (validate == -1)
            printf("Try Again. Invalid input(s) detected. All fields are required. Price and rating must be a number. Rating must between 0-10 \n\n");

    } //END LOOP of user input

    //insert data into new NodeData struct
    NodeDataPtr dataPtr = newNodeData(flavour, description, price, rating);

    //pass NodeDataPtr to new insert element
    bst.root = insertElement(bst.root, dataPtr);
    printf("\nNew flavour inserted!\n");
    if (option == 1) {
        listSelectionMenu();
    }
}

//This function converts flavour string type into an integer type for Ice Cream ID data field value

unsigned int hashCode(char *key) {
    //preprocessing key string to integer using polynomial hash
    unsigned int product = 0;
    for (int i = 0; i < strlen(key); i++) {
        product = (product << 5) + (int) key[i];
    }
    //get id/index using division hash
    unsigned int h_Id = product % MAX_PRODUCTS;
    return h_Id;
}

void printAsciiArt() {
    printf(R"EOF(
 ___  _  _  _____  _    _    ____    __   _  _    ____  ___  ____     ___  ____  ____    __    __  __  ___ 
/ __)( \( )(  _  )( \/\/ )  (  _ \  /__\ ( \/ )  (_  _)/ __)( ___)   / __)(  _ \( ___)  /__\  (  \/  )/ __)
\__ \ )  (  )(_)(  )    (    )(_) )/(__)\ \  /    _)(_( (__  )__)   ( (__  )   / )__)  /(__)\  )    ( \__ \
(___/(_)\_)(_____)(__/\__)  (____/(__)(__)(__)   (____)\___)(____)   \___)(_)\_)(____)(__)(__)(_/\/\_)(___/
)EOF");
}

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//#define MAX_PRODUCTS 100
//#define MAX_LEN 1000
//#define REMOVERN(str) str[strcspn(str,"\r\n")]=0
//
//typedef struct treeNode {
//    int id;
//    char* flavour;
//    char* description;
//    double price;
//    char* category;
//    struct treeNode*left;
//    struct treeNode*right;
//} TreeNode, *TreeNodePtr;
//
//
//void read_in();
//void inorderTraversal_writeOut(TreeNodePtr tree, FILE *fwp);
//void inorderTraversal(TreeNodePtr tree);
//TreeNodePtr newTreeNode(int id, char flavour[], char description[], double price, char category[]);
//TreeNodePtr insertElement(TreeNodePtr tree, int id, char flavour[], char description[], double price, char category[]);
//void deleteTree(TreeNodePtr tree);
//void write_out();
//
//TreeNodePtr bstree = NULL;
//TreeNodePtr ptr = NULL;
//
//int main(int argc, char** argv) {
//
//
//    read_in();
//    printf("ID\tFlavour\t\t\tDescription\t\t\t\tPrice\t\tCategory\n");
//    inorderTraversal(bstree);
//    write_out();
//    deleteTree(bstree);
//
//    return (EXIT_SUCCESS);
//}
//
////TreeNodePtr read_in(char ** arr, int* sizePtr, TreeNodePtr bstree) {
//
//void read_in() {
//    FILE *frp;
//    const char *input_file = "IceCreamList.csv";
//    char *arr[MAX_PRODUCTS];
//    register int i = 0;
//    char str[MAX_LEN];
//
//    if ((frp = fopen(input_file, "r")) == NULL) {
//        perror("Unable to open input file!\n");
//        exit(1);
//    }
//
//    while (fgets(str, sizeof (str), frp) && i < MAX_PRODUCTS) {
//
//        REMOVERN(str);
//
//        if (strlen(str) > 0) {
//            arr[i] = (char*) calloc(strlen(str) + 1, sizeof (char));
//
//            if (arr[i] == NULL) {
//                printf("Cannot allocate memory\n");
//                exit(1);
//            }
//            strncpy(arr[i], str, strlen(str));
//            i++;
//        }
//    }
//
//    fclose(frp);
//
//    /*split each line from the csv file into tokens*/
//
//    for (int j = 0; j < i; j++) {
//
//        char* token;
//        token = strtok(arr[j], ",");
//        char* tokensArray[5] = {0};
//        int k = 0;
//        while (token != NULL) {
//            tokensArray[k++] = token;
//            token = strtok(NULL, ",");
//        }
//        /*convert tokens into correct data type*/
//        int id = atoi(tokensArray[0]);
//        char* flavour = tokensArray[1];
//        char* description = tokensArray[2];
//        double price = strtod(tokensArray[3], NULL);
//        char* category = tokensArray[4];
//        /*insert into in order tree*/
//        bstree = insertElement(bstree, id, flavour, description, price, category);
//
//    }
//    /*free heap memory used for array holding the csv input lines*/
//    for (int z = 0; z < i; z++) {
//        if (arr[z] != NULL) {
//            free(arr[z]);
//        }
//    }
//}
//
//void inorderTraversal(TreeNodePtr tree) {
//    if (tree != NULL) {
//        inorderTraversal(tree -> left);
//        printf("%d\t%s\t\t\t%s\t %.2f\t %s \n", tree->id, tree->flavour, tree->description, tree->price, tree->category);
//        inorderTraversal(tree -> right);
//    }
//}
//
//void inorderTraversal_writeOut(TreeNodePtr tree, FILE *fwp) {
//    if (tree != NULL) {
//        inorderTraversal_writeOut(tree -> left, fwp);
//        fprintf(fwp, "%d,%s,%s,%.2f,%s\n", tree->id, tree->flavour, tree->description, tree->price, tree->category);
//        inorderTraversal_writeOut(tree -> right, fwp);
//    }
//}
//
//TreeNodePtr newTreeNode(int id, char flavour[], char description[], double price, char category[]) {
//
//    TreeNodePtr p = (TreeNodePtr) malloc(sizeof (TreeNode));
//
//    p -> id = id;
//    p->flavour = (char*) calloc(strlen(flavour) + 1, sizeof (char));
//    if (p->flavour == NULL) {
//        printf("Cannot allocate memory");
//        exit(1);
//    }
//    strcpy(p->flavour, flavour);
//    p->description = (char*) calloc(strlen(description) + 1, sizeof (char));
//    if (p->description == NULL) {
//        printf("Cannot allocate memory");
//        exit(1);
//    }
//    strcpy(p->description, description);
//    p->price = price;
//    p->category = (char*) calloc(strlen(category) + 1, sizeof (char));
//    if (p->category == NULL) {
//        printf("Cannot allocate memory");
//        exit(1);
//    }
//    strcpy(p->category, category);
//    p -> left = p -> right = NULL;
//
//    return p;
//}
//
////TreeNodePtr insertElement(TreeNodePtr tree, int id, char flavour[], char description[], double price, char category[]) {
////    printf("inside insert element");
////    if (tree == NULL)
////        tree = newTreeNode(id, flavour, description, price, category);
////    else if (bstree->id > id)
////        bstree->left = insertElement(id, flavour, description, price, category);
////    else if (bstree->id < id)
////        bstree->right = insertElement(id, flavour, description, price, category);
////    else if (bstree->id = id)
////        printf(" Can't insert, element already exists in the tree. \n");
////    return bstree;
////}
//
//TreeNodePtr insertElement(TreeNodePtr tree, int id, char flavour[], char description[], double price, char category[]) {
//    if (tree == NULL)
//        tree = newTreeNode(id, flavour, description, price, category);
//    else if (tree->id > id)
//        tree->left = insertElement(tree->left, id, flavour, description, price, category);
//    else if (tree->id < id)
//        tree->right = insertElement(tree->right, id, flavour, description, price, category);
//    else if (tree->id = id)
//        printf(" Can't insert, element already exists in the tree. \n");
//    return tree;
//}
//
//void deleteTree(TreeNodePtr tree) {
//    if (tree != NULL) {
//        deleteTree(tree->left);
//        deleteTree(tree->right);
//        free(tree->flavour);
//        free(tree->description);
//        free(tree->category);
//        free(tree);
//    }
//}
//
//void write_out() {
//    FILE *fwp;
//    const char *output_file = "outputTest.txt";
//    if ((fwp = fopen(output_file, "w")) == NULL) {
//        perror("Could not open output file!\n");
//        exit(1);
//    }
//
//    inorderTraversal_writeOut(bstree, fwp);
//
//    fclose(fwp);
//}
//
//
//
//
//
////#include <stdio.h>
////#include <stdlib.h>
////#include <string.h>
////
////#define MAX_PRODUCTS 100
////#define MAX_LEN 1000
////#define REMOVERN(str) str[strcspn(str,"\r\n")]=0
////
////typedef struct treeNode {
////    int id;
////    char* flavour;
////    char* description;
////    double price;
////    char* category;
////    struct treeNode*left;
////    struct treeNode*right;
////} TreeNode, *TreeNodePtr;
////
//////TreeNodePtr read_in(char ** arr, int* ptr, TreeNodePtr bstree);
////TreeNodePtr read_in(TreeNodePtr bstree);
////void inorderTraversal_writeOut(TreeNodePtr tree, FILE *fwp);
////void inorderTraversal(TreeNodePtr);
////TreeNodePtr newTreeNode(int id, char flavour[], char description[], double price, char category[]);
////TreeNodePtr insertElement(TreeNodePtr tree, int id, char flavour[], char description[], double price, char category[]);
////void deleteTree(TreeNodePtr tree);
////void write_out(TreeNodePtr tree);
////
////int main(int argc, char** argv) {
////
////    //    char *arr[MAX_PRODUCTS];
////    //    int size = MAX_PRODUCTS;
////    TreeNodePtr bstree = NULL;
////    TreeNodePtr ptr = NULL;
////
////    // bstree = read_in(arr, &size, bstree);
////    bstree = read_in(bstree);
////    printf("ID\tFlavour\t\t\tDescription\t\t\t\tPrice\t\tCategory\n");
////    inorderTraversal(bstree);
////    // write_out(bstree);
////    deleteTree(bstree);
////
////    return (EXIT_SUCCESS);
////}
////
//////TreeNodePtr read_in(char ** arr, int* sizePtr, TreeNodePtr bstree) {
////
////TreeNodePtr read_in(TreeNodePtr bstree) {
////    FILE *frp;
////    const char *input_file = "IceCreamList.csv";
////    char *arr[MAX_PRODUCTS];
////    register int i = 0;
////    char str[MAX_LEN];
////
////    if ((frp = fopen(input_file, "r")) == NULL) {
////        perror("Unable to open input file!\n");
////        exit(1);
////    }
////
////    while (fgets(str, sizeof (str), frp) && i < MAX_PRODUCTS) {
////
////        REMOVERN(str);
////
////        if (strlen(str) > 0) {
////            arr[i] = (char*) calloc(strlen(str) + 1, sizeof (char));
////
////            if (arr[i] == NULL) {
////                printf("Cannot allocate memory\n");
////                exit(1);
////            }
////            strncpy(arr[i], str, strlen(str));
////            i++;
////        }
////    }
////
////    fclose(frp);
////
////    /*split each line from the csv file into tokens*/
////
////    for (int j = 0; j < i; j++) {
////
////        char* token;
////        token = strtok(arr[j], ",");
////        char* tokensArray[5] = {0};
////        int k = 0;
////        while (token != NULL) {
////            tokensArray[k++] = token;
////            token = strtok(NULL, ",");
////        }
////        /*convert tokens into correct data type*/
////        int id = atoi(tokensArray[0]);
////        char* flavour = tokensArray[1];
////        char* description = tokensArray[2];
////        double price = strtod(tokensArray[3], NULL);
////        char* category = tokensArray[4];
////        /*insert into in order tree*/
////        bstree = insertElement(bstree, id, flavour, description, price, category);
////    }
////    /*free heap memory used for array holding the csv input lines*/
////    for (int z = 0; z < i; z++) {
////        if (arr[z] != NULL) {
////            free(arr[z]);
////        }
////    }
////
////    return bstree;
////
////}
////
////void inorderTraversal(TreeNodePtr tree) {
////    if (tree != NULL) {
////        inorderTraversal(tree -> left);
////        printf("%d\t%s\t\t\t%s\t %.2f\t %s \n", tree->id, tree->flavour, tree->description, tree->price, tree->category);
////        inorderTraversal(tree -> right);
////    }
////}
////
////void inorderTraversal_writeOut(TreeNodePtr tree, FILE *fwp) {
////    if (tree != NULL) {
////        inorderTraversal_writeOut(tree -> left, fwp);
////        fprintf(fwp, "%d,%s,%s,%.2f,%s\n", tree->id, tree->flavour, tree->description, tree->price, tree->category);
////        inorderTraversal_writeOut(tree -> right, fwp);
////    }
////}
////
////TreeNodePtr newTreeNode(int id, char flavour[], char description[], double price, char category[]) {
////
////    TreeNodePtr p = (TreeNodePtr) malloc(sizeof (TreeNode));
////
////    p -> id = id;
////    p->flavour = (char*) calloc(strlen(flavour) + 1, sizeof (char));
////    if (p->flavour == NULL) {
////        printf("Cannot allocate memory");
////        exit(1);
////    }
////    strcpy(p->flavour, flavour);
////    p->description = (char*) calloc(strlen(description) + 1, sizeof (char));
////    if (p->description == NULL) {
////        printf("Cannot allocate memory");
////        exit(1);
////    }
////    strcpy(p->description, description);
////    p->price = price;
////    p->category = (char*) calloc(strlen(category) + 1, sizeof (char));
////    if (p->category == NULL) {
////        printf("Cannot allocate memory");
////        exit(1);
////    }
////    strcpy(p->category, category);
////    p -> left = p -> right = NULL;
////
////    return p;
////}
////
////TreeNodePtr insertElement(TreeNodePtr tree, int id, char flavour[], char description[], double price, char category[]) {
////    if (tree == NULL)
////        tree = newTreeNode(id, flavour, description, price, category);
////    else if (tree->id > id)
////        tree->left = insertElement(tree->left, id, flavour, description, price, category);
////    else if (tree->id < id)
////        tree->right = insertElement(tree->right, id, flavour, description, price, category);
////    else if (tree->id = id)
////        printf(" Can't insert, element already exists in the tree. \n");
////    return tree;
////}
////
////void deleteTree(TreeNodePtr tree) {
////    if (tree != NULL) {
////        deleteTree(tree->left);
////        deleteTree(tree->right);
////        free(tree->flavour);
////        free(tree->description);
////        free(tree->category);
////        free(tree);
////    }
////}
////
////void write_out(TreeNodePtr tree) {
////    FILE *fwp;
////    const char *output_file = "outputTest.txt";
////    if ((fwp = fopen(output_file, "w")) == NULL) {
////        perror("Could not open output file!\n");
////        exit(1);
////    }
////
////    inorderTraversal_writeOut(tree, fwp);
////
////    fclose(fwp);
////}
////
////
