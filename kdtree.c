#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

struct Node {
    double point[2];
    int userNumber;
    struct Node* left;
    struct Node* right;
    int depth;
};

struct Node* newNode(double arr[], int userNumber, int depth) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    for (int i = 0; i < 2; i++)
        temp->point[i] = arr[i];
    temp->userNumber = userNumber;
    temp->left = temp->right = NULL;
    temp->depth = depth;
    return temp;
}

struct Node* insertRec(struct Node* root, double point[], int userNumber, unsigned depth) {
    if (root == NULL)
        return newNode(point, userNumber, depth);
    unsigned cd = depth % 2;
    if (point[cd] < root->point[cd])
        root->left = insertRec(root->left, point, userNumber, depth + 1);
    else
        root->right = insertRec(root->right, point, userNumber, depth + 1);
    return root;
}

int arePointsSame(double point1[], double point2[]) {
    for (int i = 0; i < 2; ++i)
        if (point1[i] != point2[i])
            return 0;
    return 1;
}

int searchRec(struct Node* root, double point[], unsigned depth) {
    if (root == NULL)
        return 0;
    if (arePointsSame(root->point, point))
        return 1;
    unsigned cd = depth % 2;
    if (point[cd] < root->point[cd])
        return searchRec(root->left, point, depth + 1);
    return searchRec(root->right, point, depth + 1);
}

int search(struct Node* root, double point[]) {
    return searchRec(root, point, 0);
}

void findNearest(struct Node* root, double target[], double* dist, struct Node** nearest) {
    if (root == NULL)
        return;
    double d = sqrt(pow(target[0] - root->point[0], 2) + pow(target[1] - root->point[1], 2));
    if (d < *dist) {
        *dist = d;
        *nearest = root;
    }
    struct Node* nearer = (target[root->depth % 2] < root->point[root->depth % 2]) ? root->left : root->right;
    struct Node* further = (target[root->depth % 2] < root->point[root->depth % 2]) ? root->right : root->left;
    findNearest(nearer, target, dist, nearest);
    findNearest(further, target, dist, nearest);
}

struct Node* insert(struct Node* root, double point[2], int userNumber) {
    if (root == NULL) {
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->point[0] = point[0];
        newNode->point[1] = point[1];
        newNode->userNumber = userNumber;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    if (point[0] < root->point[0] || (point[0] == root->point[0] && point[1] < root->point[1])) {
        root->left = insert(root->left, point, userNumber);
    } else {
        root->right = insert(root->right, point, userNumber);
    }
    return root;
}

int main() {
    struct Node* root = NULL;
    int n;
    printf("Enter the number of users: ");
    scanf("%d", &n);
    printf("Each user is categorised by 2 factors: genre and language.\nX refers to genre and Y refers to language\nFor X: 1-Pop, 2-Hip-hop/Rap, 3-Rock, 4-Dance/Electronic, 5-Indie.\nFor Y: Y corresponds to listening hours.\n\n");
    double** points = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        points[i] = (double*)malloc(2 * sizeof(double));
        printf("Enter point %d (x y): ", i + 1);
        for (int j = 0; j < 2; j++) {
            scanf("%lf", &points[i][j]);
            if (points[i][j] < 1 || points[i][j] > 5) {
                printf("\nInvalid genre or language value. Must be between 1 and 5.\n");
                return 1;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        root = insert(root, points[i], i + 1);
    }
    double target[2];
    printf("\nEnter the target point (genre language): ");
    for (int i = 0; i < 2; i++) {
        scanf("%lf", &target[i]);
        if (target[i] < 1 || target[i] > 5) {
            printf("\nInvalid genre or language value. Must be between 1 and 5.\n");
            return 1;
        }
    }
    double dist = INT_MAX;
    struct Node* nearest = NULL;
    findNearest(root, target, &dist, &nearest);
    printf("Nearest user to target is: User %d with coordinates %.1lf, %.1lf\n", nearest->userNumber, nearest->point[0], nearest->point[1]);
    for (int i = 0; i < n; i++) {
        free(points[i]);
    }
    free(points);
    return 0;
}