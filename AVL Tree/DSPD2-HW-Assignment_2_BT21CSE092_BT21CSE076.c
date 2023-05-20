#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BT21CSE092 Rohan Meshram
// BT21CSE076 Azizullah Choudhary

// course(1) = DSPD2 (1);
// course(2) = LA (2);
// course(3) = IOOM (3);
// course(4) = CPL (4);
// course(5) = CO (5);
// course(6) = DCMP (6);
// course(7) = PTSM (2);
// course(8) = TC (4);

// slot(1) = 10:00 AM - 11:00 AM
// slot(2) = 11:00 AM - 12:00 AM
// slot(3) = 12:00 PM - 01:00 PM
// LUNCH   = 01:00 PM - 02:00 PM
// slot(4) = 02:00 PM - 03:00 PM
// slot(5) = 03:00 PM - 04:00 PM
// slot(6) = 04:00 PM - 05:00 PM

int Num_courses = 0;
int Num_classrooms = 0;
int Num_Students = 0;
#define slots_of_course 6

struct Courses
{
    int Cnum;
    int Slot;
    int numStudents;
    char Cname[20];
    struct Courses *left;
    struct Courses *right;
    struct Students *Student;
    int height;
};

struct Courses *rootCourse = NULL;

struct Students
{
    int Sid;
    char Sname[20];
    int roomNum;
    int row;
    int column;
    int L_R;
    struct Courses *courses;
    struct Students *left;
    struct Students *right;
    int height;
};
struct Students *HeadStudents = NULL;

struct Classrooms
{
    int roomNum;
    int capacity;
    int SeatOccupied;
    struct Courses *Course;
    struct Classrooms *left;
    struct Classrooms *right;
    int height;
};
struct Classrooms *firstClass = NULL;

int getHeight_Course(struct Courses *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

int getHeight_Student(struct Students *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return n->height;
}

struct Courses *createCourse(int Cnum, char *Cname, int Slot)
{
    struct Courses *newCourse = (struct Courses *)malloc(sizeof(struct Courses));
    newCourse->Cnum = Cnum;
    newCourse->Slot = Slot;
    newCourse->numStudents = 0;
    strcpy(newCourse->Cname, Cname);
    newCourse->left = NULL;
    newCourse->right = NULL;
    newCourse->Student = NULL;
    newCourse->height = 1;

    Num_courses++;

    return newCourse;
}

struct Students *createStudent(int Sid, char *Sname)
{
    struct Students *newStudent = (struct Students *)malloc(sizeof(struct Students));
    newStudent->Sid = Sid;
    strcpy(newStudent->Sname, Sname);
    newStudent->roomNum = 0;
    newStudent->column = 0;
    newStudent->row = 0;
    newStudent->L_R = 0;
    newStudent->left = NULL;
    newStudent->right = NULL;
    newStudent->height = 1;

    Num_Students++;

    return newStudent;
}

int getBalancedFactor_Course(struct Courses *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return getHeight_Course(n->left) - getHeight_Course(n->right);
}

int getBalancedFactor_Student(struct Students *n)
{
    if (n == NULL)
    {
        return 0;
    }
    return getHeight_Student(n->left) - getHeight_Student(n->right);
}

int max(int x, int y)
{
    if (x >= y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

struct Students *insert_Student(struct Students *Student, int Sid, char *Sname);

void addStudent(struct Courses *first, int Sid, char *Sname)
{
    struct Students *newS = (struct Students *)malloc(sizeof(struct Students));
    struct Courses *ptr = first;
    newS->Sid = Sid;
    strcpy(newS->Sname, Sname);
    newS->left = NULL;
    newS->right = NULL;
    ptr->numStudents++;
    newS->height = 1;
    ptr->Student = insert_Student(ptr->Student, Sid, Sname);
}

struct Courses *RightRotate_Course(struct Courses *y)
{
    if (y == NULL)
    {
        return NULL;
    }

    struct Courses *x = y->left;
    if (x == NULL)
    {
        return y;
    }

    struct Courses *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(getHeight_Course(y->right), getHeight_Course(y->left)) + 1;
    x->height = max(getHeight_Course(x->right), getHeight_Course(x->left)) + 1;

    return x;
}

struct Students *RightRotate_Student(struct Students *y)
{
    if (y == NULL)
    {
        return NULL;
    }

    struct Students *x = y->left;
    if (x == NULL)
    {
        return y;
    }

    struct Students *T2 = x->right;

    x->right = y;

    y->left = T2;

    y->height = max(getHeight_Student(y->right), getHeight_Student(y->left)) + 1;
    x->height = max(getHeight_Student(x->right), getHeight_Student(x->left)) + 1;

    return x;
}

struct Courses *LeftRotate_Course(struct Courses *x)
{
    if (x == NULL)
    {
        return NULL;
    }

    struct Courses *y = x->right;
    if (y == NULL)
    {
        return x;
    }

    struct Courses *T2 = y->left;

    y->left = x;
    x->right = T2;

    y->height = max(getHeight_Course(y->right), getHeight_Course(y->left)) + 1;
    x->height = max(getHeight_Course(x->right), getHeight_Course(x->left)) + 1;

    return y;
}

struct Students *LeftRotate_Student(struct Students *x)
{
    if (x == NULL)
    {
        return NULL;
    }

    struct Students *y = x->right;
    if (y == NULL)
    {
        return x;
    }

    struct Students *T2 = y->left;

    y->left = x;
    x->right = T2;

    y->height = max(getHeight_Student(y->right), getHeight_Student(y->left)) + 1;
    x->height = max(getHeight_Student(x->right), getHeight_Student(x->left)) + 1;

    return y;
}

struct Courses *insert_Course(struct Courses *Course, int Cnum, char *Cname, int Slot)
{
    if (Course == NULL)
    {
        return createCourse(Cnum, Cname, Slot);
    }

    if (Cnum < Course->Cnum)
    {
        Course->left = insert_Course(Course->left, Cnum, Cname, Slot);
    }
    else if (Cnum > Course->Cnum)
    {
        Course->right = insert_Course(Course->right, Cnum, Cname, Slot);
    }

    Course->height = 1 + max(getHeight_Course(Course->left), getHeight_Course(Course->right));
    int bf = getBalancedFactor_Course(Course);

    if (bf > 1 && Cnum < Course->left->Cnum)
    {
        return RightRotate_Course(Course);
    }

    if (bf < -1 && Cnum > Course->right->Cnum)
    {
        return LeftRotate_Course(Course);
    }

    if (bf > 1 && Cnum > Course->left->Cnum)
    {
        Course->left = LeftRotate_Course(Course->left);
        return RightRotate_Course(Course);
    }

    if (bf < -1 && Cnum < Course->right->Cnum)
    {
        Course->right = RightRotate_Course(Course->right);
        return LeftRotate_Course(Course);
    }

    return Course;
}

struct Students *insert_Student(struct Students *Student, int Sid, char *Sname)
{
    if (Student == NULL)
    {
        return createStudent(Sid, Sname);
    }

    if (Sid < Student->Sid)
    {
        Student->left = insert_Student(Student->left, Sid, Sname);
    }
    else if (Sid > Student->Sid)
    {
        Student->right = insert_Student(Student->right, Sid, Sname);
    }

    Student->height = 1 + max(getHeight_Student(Student->left), getHeight_Student(Student->right));
    int bf = getBalancedFactor_Student(Student);

    if (bf > 1 && getBalancedFactor_Student(Student->left) >= 0)
    {
        return RightRotate_Student(Student);
    }

    if (bf > 1 && getBalancedFactor_Student(Student->left) < 0)
    {
        Student->left = LeftRotate_Student(Student->left);
        return RightRotate_Student(Student);
    }

    if (bf < -1 && getBalancedFactor_Student(Student->right) <= 0)
    {
        return LeftRotate_Student(Student);
    }

    if (bf < -1 && getBalancedFactor_Student(Student->right) > 0)
    {
        Student->right = RightRotate_Student(Student->right);
        return LeftRotate_Student(Student);
    }

    return Student;
}

void printAVLTree_Course(struct Courses *root, int space)
{
    if (root == NULL)
    {
        return;
    }

    space += 4;

    printAVLTree_Course(root->right, space);

    printf("\n");
    for (int i = 4; i < space; i++)
    {
        printf(" ");
    }
    printf("%d(%s) \n", root->Cnum, root->Cname);

    printAVLTree_Course(root->left, space);
}

void printAVLTree_Student(struct Students *root, int space)
{
    if (root == NULL)
    {
        return;
    }

    space += 4;

    printAVLTree_Student(root->right, space);

    printf("\n");
    for (int i = 4; i < space; i++)
    {
        printf(" ");
    }
    printf("%d(%s) \n", root->Sid, root->Sname);

    printAVLTree_Student(root->left, space);
}

void printAVLTree_Classroom(struct Classrooms *root, int space)
{
    if (root == NULL)
    {
        return;
    }

    space += 4;

    printAVLTree_Classroom(root->right, space);

    printf("\n");
    for (int i = 4; i < space; i++)
    {
        printf(" ");
    }
    printf("%d(%d) \n", root->roomNum, root->capacity);

    printAVLTree_Classroom(root->left, space);
}

void inOrder_Student(struct Students *root)
{
    if (root != NULL)
    {
        inOrder_Student(root->left);
        printf("     %d %s\n", root->Sid, root->Sname);

        inOrder_Student(root->right);
    }
}

void inOrder_Course(struct Courses *root)
{
    if (root != NULL)
    {
        inOrder_Course(root->left);
        printf("%d %s\n", root->Cnum, root->Cname);
        inOrder_Student(root->Student);
        inOrder_Course(root->right);
    }
}

void inOrder_Classroom(struct Classrooms *root)
{
    if (root != NULL)
    {
        inOrder_Classroom(root->left);
        printf("%d - %d\n", root->roomNum, root->capacity);

        inOrder_Classroom(root->right);
    }
}

struct Courses *findCourse(struct Courses *ptr, int k)
{
    while (ptr != NULL)
    {
        if (ptr->Cnum == k)
        {
            return ptr;
        }
        else if (ptr->Cnum > k)
        {
            ptr = ptr->left;
        }
        else
        {
            ptr = ptr->right;
        }
    }

    return NULL;
}

struct Students *insertS(struct Students *Student, int Sid, char *Sname, int *C, struct Courses **first)
{
    struct Courses *ptr = *first;
    int slot[slots_of_course] = {0};

    for (int i = 0; i < 5; i++)
    {
        int k = C[i];
        struct Courses *found = findCourse(ptr, k);

        if (found == NULL)
        {
            printf("Course %d not found\n", k);
            continue;
        }

        addStudent(found, Sid, Sname);
        slot[found->Slot - 1]++;
    }

    for (int i = 0; i < slots_of_course; i++)
    {
        if (slot[i] > 1)
        {
            printf("%s cannot take two courses of same slot (%d)\n", Sname, i + 1);
            exit(0);
        }
    }

    Student = insert_Student(Student, Sid, Sname);
    return Student;
}

struct Students *STreeBalance(struct Students *Student)
{

    Student->height = 1 + max(getHeight_Student(Student->left), getHeight_Student(Student->right));
    int bf = getBalancedFactor_Student(Student);

    if (bf > 1 && getBalancedFactor_Student(Student->left) >= 0)
    {
        return RightRotate_Student(Student);
    }

    if (bf > 1 && getBalancedFactor_Student(Student->left) < 0)
    {
        Student->left = LeftRotate_Student(Student->left);
        return RightRotate_Student(Student);
    }

    if (bf < -1 && getBalancedFactor_Student(Student->right) <= 0)
    {
        return LeftRotate_Student(Student);
    }

    if (bf < -1 && getBalancedFactor_Student(Student->right) > 0)
    {
        Student->right = RightRotate_Student(Student->right);
        return LeftRotate_Student(Student);
    }

    return Student;
}

struct Courses *CTreeBalance(struct Courses *Course)
{

    Course->height = 1 + max(getHeight_Course(Course->left), getHeight_Course(Course->right));
    int bf = getBalancedFactor_Course(Course);

    if (bf > 1 && getBalancedFactor_Course(Course->left) >= 0)
    {
        return RightRotate_Course(Course);
    }

    if (bf > 1 && getBalancedFactor_Course(Course->left) < 0)
    {
        Course->left = LeftRotate_Course(Course->left);
        return RightRotate_Course(Course);
    }

    if (bf < -1 && getBalancedFactor_Course(Course->right) <= 0)
    {
        return LeftRotate_Course(Course);
    }

    if (bf < -1 && getBalancedFactor_Course(Course->right) > 0)
    {
        Course->right = RightRotate_Course(Course->right);
        return LeftRotate_Course(Course);
    }

    return Course;
}

void deleteStudent(struct Courses *rootCourse, int Sid)
{
    struct Courses *currentCourse = rootCourse;

    // Traverse through all the courses
    while (currentCourse != NULL)
    {
        struct Students *currentStudent = currentCourse->Student;
        struct Students *prevStudent = NULL;

        // Traverse through all the students in the current course
        while (currentStudent != NULL)
        {
            // If the student to be deleted is found
            if (currentStudent->Sid == Sid)
            {
                // Remove the student from the student list of the current course
                if (prevStudent == NULL)
                {
                    currentCourse->Student = currentStudent->right;
                }
                else
                {
                    prevStudent->right = currentStudent->right;
                }

                free(currentStudent);
                return;
            }

            prevStudent = currentStudent;
            currentStudent = currentStudent->right;
        }

        currentCourse = currentCourse->right;
    }
}

struct Students *remove_student(struct Students *root, int id)
{
    // Base case: empty tree
    if (root == NULL)
    {
        return root;
    }
    // If the ID to be deleted is smaller than the root's ID,
    // then it lies in left subtree
    if (id < root->Sid)
    {
        root->left = remove_student(root->left, id);
    }
    // If the ID to be deleted is greater than the root's ID,
    // then it lies in right subtree
    else if (id > root->Sid)
    {
        root->right = remove_student(root->right, id);
    }
    // If ID is same as root's ID, then this is the node
    // to be deleted
    else
    {
        // If the node has only one child or no child
        if ((root->left == NULL) || (root->right == NULL))
        {
            struct Students *temp = root->left ? root->left : root->right;
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            // One child case
            else
            {
                *root = *temp;
            }
            free(temp->Sname);
            free(temp);
        }
        // If the node has two children, get the inorder successor
        else
        {
            struct Students *temp = root->right;
            while (temp->left != NULL)
            {
                temp = temp->left;
            }
            // Copy the inorder successor's data to this node
            strcpy(root->Sname, temp->Sname);
            root->Sid = temp->Sid;
            root->courses = temp->courses;
            // Delete the inorder successor
            root->right = remove_student(root->right, temp->Sid);
        }
    }
    // If the tree had only one node then return
    if (root == NULL)
    {
        return root;
    }
    STreeBalance(root);
}

struct Courses *delete_student_from_course(struct Courses *rootc, int id)
{
    if (rootc == NULL)
    {
        return rootc;
    }

    // Delete the student from the left subtree
    rootc->left = delete_student_from_course(rootc->left, id);

    // Delete the student from the right subtree
    rootc->right = delete_student_from_course(rootc->right, id);

    // Check if this course contains the student
    if (rootc->Student->Sid, id)
    {
        // Remove the student from the course
        rootc->Student = remove_student(rootc->Student, id);

        // Rebalance the tree
        CTreeBalance(rootc);
    }

    return rootc;
}

struct Students *delete_student(struct Students *root, int id, struct Courses *rootc)
{
    // Base case: empty tree
    if (root == NULL)
    {
        return root;
    }
    // If the ID to be deleted is smaller than the root's ID,
    // then it lies in left subtree
    if (id < root->Sid)
    {
        root->left = delete_student(root->left, id, rootc);
    }
    // If the ID to be deleted is greater than the root's ID,
    // then it lies in right subtree
    else if (id > root->Sid)
    {
        root->right = delete_student(root->right, id, rootc);
    }
    // If ID is same as root's ID, then this is the node
    // to be deleted
    else
    {
        // If the node has only one child or no child
        if ((root->left == NULL) || (root->right == NULL))
        {
            struct Students *temp = root->left ? root->left : root->right;
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            // One child case
            else
            {
                *root = *temp;
            }
            free(temp->Sname);
            free(temp);

            rootc = delete_student_from_course(rootc, id);
        }
        // If the node has two children, get the inorder successor
        else
        {
            struct Students *temp = root->right;
            while (temp->left != NULL)
            {
                temp = temp->left;
            }
            // Copy the inorder successor's data to this node
            strcpy(root->Sname, temp->Sname);
            root->Sid = temp->Sid;
            root->courses = temp->courses;
            // Delete the inorder successor
            root->right = delete_student(root->right, temp->Sid, rootc);

            rootc = delete_student_from_course(rootc, id);
        }
    }
    // If the tree had only one node then return
    if (root == NULL)
    {
        return root;
    }

    STreeBalance(root);
}

struct Courses *find_min(struct Courses *root)
{
    if (root == NULL)
    {
        // Empty tree
        return NULL;
    }
    else if (root->left == NULL)
    {
        // Found the smallest node
        return root;
    }
    else
    {
        // Traverse left subtree
        return find_min(root->left);
    }
}

struct Courses *delete_Course(struct Courses *root, int Cnum)
{
    if (root == NULL)
    {
        return NULL;
    }
    else if (Cnum < root->Cnum)
    {
        root->left = delete_Course(root->left, Cnum);
    }
    else if (Cnum > root->Cnum)
    {
        root->right = delete_Course(root->right, Cnum);
    }
    else
    {
        // Found the node to delete

        if (root != NULL && root->numStudents != 0 || root->Student != NULL)
        {
            // Restore the deleted course and return the original tree
            // root = insert_Course(root, root->Cnum, root->Cname, root->Slot);
            printf("Cannot delete course with id %d because students have taken it.\n", Cnum);
            return root;
        }
        // Case 1: Node has no children
        if (root->left == NULL && root->right == NULL)
        {
            if (root != NULL && root->numStudents != 0 || root->Student != NULL)
            {
                // Restore the deleted course and return the original tree
                // root = insert_Course(root, root->Cnum, root->Cname, root->Slot);
                printf("Cannot delete course with id %d because students have taken it.\n", Cnum);
                return root;
            }
            free(root);
            root = NULL;
        }
        // Case 2: Node has one child
        else if (root->left == NULL)
        {
            if (root != NULL && root->numStudents != 0 || root->Student != NULL)
            {
                // Restore the deleted course and return the original tree
                // root = insert_Course(root, root->Cnum, root->Cname, root->Slot);
                printf("Cannot delete course with id %d because students have taken it.\n", Cnum);
                return root;
            }
            struct Courses *temp = root;
            root = root->right;
            free(temp);
        }
        else if (root->right == NULL)
        {
            if (root != NULL && root->numStudents != 0 || root->Student != NULL)
            {
                // Restore the deleted course and return the original tree
                // root = insert_Course(root, root->Cnum, root->Cname, root->Slot);
                printf("Cannot delete course with id %d because students have taken it.\n", Cnum);
                return root;
            }
            struct Courses *temp = root;
            root = root->left;
            free(temp);
        }
        // Case 3: Node has two children
        else
        {
            struct Courses *temp = find_min(root->right);
            root->Cnum = temp->Cnum;
            strcpy(root->Cname, temp->Cname);
            root->Slot = temp->Slot;
            root->right = delete_Course(root->right, temp->Cnum);
        }
    }

    if (root == NULL)
    {
        return NULL;
    }

    root->height = 1 + max(getHeight_Course(root->left), getHeight_Course(root->right));
    int bf = getBalancedFactor_Course(root);

    if (bf > 1 && getBalancedFactor_Course(root->left) >= 0)
    {
        return RightRotate_Course(root);
    }

    if (bf > 1 && getBalancedFactor_Course(root->left) < 0)
    {
        root->left = LeftRotate_Course(root->left);
        return RightRotate_Course(root);
    }

    if (bf < -1 && getBalancedFactor_Course(root->right) <= 0)
    {
        return LeftRotate_Course(root);
    }

    if (bf < -1 && getBalancedFactor_Course(root->right) > 0)
    {
        root->right = RightRotate_Course(root->right);
        return LeftRotate_Course(root);
    }

    return root;
}

int height(struct Classrooms *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int getBalance(struct Classrooms *node)
{
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

struct Classrooms *newClassroom(int roomNum, int bpr)
{
    struct Classrooms *classroom = (struct Classrooms *)malloc(sizeof(struct Classrooms));
    classroom->roomNum = roomNum;
    classroom->capacity = 12 * bpr;
    classroom->SeatOccupied = 0;
    classroom->left = NULL;
    classroom->right = NULL;
    classroom->Course = NULL;
    classroom->height = 1;
    return classroom;
}

struct Classrooms *rightRotate(struct Classrooms *y)
{
    struct Classrooms *x = y->left;
    struct Classrooms *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

struct Classrooms *leftRotate(struct Classrooms *x)
{
    struct Classrooms *y = x->right;
    struct Classrooms *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

struct Classrooms *insert(struct Classrooms *node, int roomNum, int capacity)
{
    if (node == NULL)
        return newClassroom(roomNum, capacity);

    if (roomNum < node->roomNum)
        node->left = insert(node->left, roomNum, capacity);
    else if (roomNum > node->roomNum)
        node->right = insert(node->right, roomNum, capacity);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && roomNum < node->left->roomNum)
        return rightRotate(node);

    if (balance < -1 && roomNum > node->right->roomNum)
        return leftRotate(node);

    if (balance > 1 && roomNum > node->left->roomNum)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && roomNum < node->right->roomNum)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

void printClassrooms(struct Classrooms *node)
{
    if (node == NULL)
        return;

    printClassrooms(node->left);
    printf("Room Number: %d, Capacity: %d\n", node->roomNum,
           node->capacity);
    printClassrooms(node->right);
}

// void RangeSearch_Course(struct Courses *root, int course_id_1, int course_id_2)
// {
//     if (root == NULL)
//     {
//         return;
//     }
//     // printf("print the value of %d\n", root->Cnum);

//     if (root->left != NULL && root->left->Cnum >= course_id_1)
//     {
//         RangeSearch_Course(root->left, course_id_1, course_id_2);
//         if (root->left->right != NULL && root->left->right->Cnum >= course_id_1)
//         {
//             RangeSearch_Course(root->left->right, course_id_1, course_id_2);
//         }
//     }

//     if (root->Cnum >= course_id_1 && root->Cnum <= course_id_2)
//     {
//         printf("  %d %s\n", root->Cnum, root->Cname);
//     }

//     if (root->right != NULL && root->right->Cnum <= course_id_2)
//     {
//         RangeSearch_Course(root->right, course_id_1, course_id_2);

//         if (root->right->left != NULL && root->right->left->Cnum <= course_id_1)
//         {
//             RangeSearch_Course(root->right->left, course_id_1, course_id_2);
//         }
//     }
// }

void RangeSearch_Course(struct Courses *root, int course_id_1, int course_id_2)
{
    if (root == NULL)
    {
        return;
    }

    if (root->Cnum >= course_id_1 && root->Cnum <= course_id_2)
    {
        RangeSearch_Course(root->left, course_id_1, course_id_2);
        printf("  %d %s\n", root->Cnum, root->Cname);
        RangeSearch_Course(root->right, course_id_1, course_id_2);
    }
    else if (root->Cnum < course_id_1)
    {
        RangeSearch_Course(root->right, course_id_1, course_id_2);
    }
    else if (root->Cnum > course_id_2)
    {
        RangeSearch_Course(root->left, course_id_1, course_id_2);
    }
}

static int slot[slots_of_course + 1] = {0};

void Allot_for_Exams(struct Classrooms *room, struct Courses *Course)
{
    if (room == NULL)
    {
        return;
    }
    if (Course == NULL)
    {
        return;
    }
    while (room->right != NULL)
    {
        room = room->right;
    }

    slot[Course->Slot]++;

    if (slot[Course->Slot] == 1)
    {
        struct Students *Student1 = Course->Student;

        int tempCap = room->capacity;
        int bpr = tempCap / 12;
        int ro = 1;
        int col = 1;

        printf("Printing for Slot %d: \n", Course->Slot);
        if (Student1 == NULL)
        {
            printf("No student to allot from Course no. %s", Course->Cname);
            return;
        }
        else
        {
            struct Students *Student2 = Course->Student->left;
            while (Student1 != NULL)
            {
                Student1->roomNum = room->roomNum;
                Student1->column = col;
                Student1->row = ro;
                Student1->L_R = 01;

                printf("%d %s is alloted with room number %d with row %d column %d and attribute %d \n", Student1->Sid, Student1->Sname, room->roomNum, Student1->row, Student1->column, Student1->L_R);

                ro++;

                if (ro == bpr)
                {
                    ro = 1;
                    col++;
                }

                Student1 = Student1->right;
            }
            while (Student2 != NULL)
            {
                Student2->roomNum = room->roomNum;
                Student2->column = col;
                Student2->row = ro;
                Student2->L_R = 01;

                printf("%d %s is alloted with room number %d with row %d column %d and attribute %d \n", Student2->Sid, Student2->Sname, room->roomNum, Student2->row, Student2->column, Student2->L_R);

                ro++;

                if (ro == bpr)
                {
                    ro = 1;
                    col++;
                }

                Student2 = Student2->left;
            }
        }
    }
    else if (slot[Course->Slot] == 2)
    {
        struct Students *Student1 = Course->Student;

        int tempCap = room->capacity;
        int bpr = tempCap / 12;
        int ro = 1;
        int col = 1;

        printf("Printing for Slot %d: \n", Course->Slot);
        if (Student1 == NULL)
        {
            printf("No student to allot from Course no. %s", Course->Cname);
            return;
        }
        else
        {
            struct Students *Student2 = Course->Student->left;
            while (Student1 != NULL)
            {
                Student1->roomNum = room->roomNum;
                Student1->column = col;
                Student1->row = ro;
                Student1->L_R = 02;

                printf("%d %s is alloted with room number %d with row %d column %d and attribute %d \n", Student1->Sid, Student1->Sname, room->roomNum, Student1->row, Student1->column, Student1->L_R);

                ro++;

                if (ro == bpr)
                {
                    ro = 1;
                    col++;
                }

                Student1 = Student1->right;
            }
            while (Student2 != NULL)
            {
                Student2->roomNum = room->roomNum;
                Student2->column = col;
                Student2->row = ro;
                Student2->L_R = 02;

                printf("%d %s is alloted with room number %d with row %d column %d and attribute %d \n", Student2->Sid, Student2->Sname, room->roomNum, Student2->row, Student2->column, Student2->L_R);

                ro++;

                if (ro == bpr)
                {
                    ro = 1;
                    col++;
                }

                Student2 = Student2->left;
            }
        }
    }
    printf("\n\n");
    Allot_for_Exams(room, Course->left);
    Allot_for_Exams(room, Course->right);
}

static int slot2[slots_of_course + 1] = {0};

void Allot_for_lectures(struct Classrooms *ptr, struct Courses *Course)
{
    struct Classrooms *room = ptr;
    if (room == NULL)
    {
        return;
    }
    if (Course == NULL)
    {
        return;
    }
    struct Classrooms *prev = room;
    room = room->right;
    if (room->right != NULL)
    {
        while (room->right != NULL)
        {
            room = room->right;
            prev = prev->right;
        }
    }

    slot2[Course->Slot]++;

    printf("Slot %d :", Course->Slot);

    if (slot2[Course->Slot] == 1)
    {
        printf("%s is alloted to classroom number %d\n\n", Course->Cname, room->roomNum);
        room->Course = Course;
    }
    else if (slot2[Course->Slot] == 2)
    {
        prev->Course = Course;
        printf("%s is alloted to classroom number %d\n\n", Course->Cname, prev->roomNum);
    }

    Allot_for_lectures(ptr, Course->left);
    Allot_for_lectures(ptr, Course->right);
}

int main()
{
    int q;
    printf("Enter 1 if you wish to add a student to the student list \n");
    printf("Enter 2 if you wish to delete a student from the student list \n");
    printf("Enter 3 if you wish to add course to the course list \n");
    printf("Enter 4 if you wish to delete a course from the course list \n");
    printf("Enter 5 if you wish to allot classrooms for lectures \n");
    printf("Enter 6 if you wish to allot the exam seats to the students and print the student list \n");
    printf("Enter 7 if you wish to do Range Search between two course numbers \n");

    scanf("%d", &q);

    struct Courses *rootCourse = NULL;
    struct Students *HeadStudents = NULL;
    // struct classrooms *head_class = NULL;

    FILE *fptr;
    char line[100];

    FILE *course;
    char *Cname;
    int Cnum, slot;
    char Cline[100];

    FILE *stu;
    int c[5], c1, c2, c3, c4, c5, EnNum;
    char *Sname;
    char Sline[100];

    FILE *class;
    int bpr, roomNum;
    char Rline[100];

    switch (q)
    {
    case 1:

        course = fopen("courses.txt", "r");
        while (fgets(Cline, 100, course) != NULL)
        {
            Cname = (char *)malloc(50 * sizeof(char));
            sscanf(Cline, "%s %d %d", Cname, &Cnum, &slot);
            rootCourse = insert_Course(rootCourse, Cnum, Cname, slot);
        }
        fclose(course);

        stu = fopen("students.txt", "r");
        while (fgets(Sline, 100, stu) != NULL)
        {
            Sname = (char *)malloc(50 * sizeof(char));
            sscanf(Sline, "%s %d %d %d %d %d %d ", Sname, &EnNum, &c1, &c2, &c3, &c4, &c5);
            c[0] = c1;
            c[1] = c2;
            c[2] = c3;
            c[3] = c4;
            c[4] = c5;
            HeadStudents = insertS(HeadStudents, EnNum, Sname, c, &rootCourse);
        }
        fclose(stu);

        printAVLTree_Course(rootCourse, 0);
        inOrder_Course(rootCourse);
        printAVLTree_Student(HeadStudents, 0);

        break;

    case 2:
        course = fopen("courses.txt", "r");
        while (fgets(Cline, 100, course) != NULL)
        {
            Cname = (char *)malloc(50 * sizeof(char));
            sscanf(Cline, "%s %d %d", Cname, &Cnum, &slot);
            rootCourse = insert_Course(rootCourse, Cnum, Cname, slot);
        }
        fclose(course);

        // printAVLTree_Course(rootCourse, 0);

        stu = fopen("students.txt", "r");
        while (fgets(Sline, 100, stu) != NULL)
        {
            Sname = (char *)malloc(50 * sizeof(char));
            sscanf(Sline, "%s %d %d %d %d %d %d ", Sname, &EnNum, &c1, &c2, &c3, &c4, &c5);
            c[0] = c1;
            c[1] = c2;
            c[2] = c3;
            c[3] = c4;
            c[4] = c5;
            HeadStudents = insertS(HeadStudents, EnNum, Sname, c, &rootCourse);
        }
        fclose(stu);

        inOrder_Student(HeadStudents);
        inOrder_Course(rootCourse);
        printf("\n");
        fptr = fopen("question2.txt", "r");
        while (fgets(line, 100, fptr) != NULL)
        {
            sscanf(line, "%d", &EnNum);
            HeadStudents = delete_student(HeadStudents, EnNum, rootCourse);
        }
        fclose(fptr);

        printf("After deleting the students: \n\n");

        inOrder_Student(HeadStudents);
        inOrder_Course(rootCourse);

        break;

    case 3:

        course = fopen("courses.txt", "r");
        while (fgets(Cline, 100, course) != NULL)
        {
            Cname = (char *)malloc(50 * sizeof(char));
            sscanf(Cline, "%s %d %d", Cname, &Cnum, &slot);
            rootCourse = insert_Course(rootCourse, Cnum, Cname, slot);
        }
        fclose(course);

        printAVLTree_Course(rootCourse, 0);
        inOrder_Course(rootCourse);
        break;

    case 4:

        course = fopen("courses.txt", "r");
        while (fgets(Cline, 100, course) != NULL)
        {
            Cname = (char *)malloc(50 * sizeof(char));
            sscanf(Cline, "%s %d %d", Cname, &Cnum, &slot);
            rootCourse = insert_Course(rootCourse, Cnum, Cname, slot);
        }
        fclose(course);

        addStudent(rootCourse, 101, "Rohan");
        addStudent(rootCourse->left, 102, "Rohan");
        addStudent(rootCourse->left->left, 103, "Aziz");

        printAVLTree_Course(rootCourse, 0);
        inOrder_Course(rootCourse);

        fptr = fopen("question4.txt", "r");
        while (fgets(line, 100, fptr) != NULL)
        {
            sscanf(line, "%d", &Cnum);
            rootCourse = delete_Course(rootCourse, Cnum);
        }
        fclose(fptr);

        printf("After deleting the Courses: \n\n");
        printAVLTree_Course(rootCourse, 0);
        inOrder_Course(rootCourse);
        break;

    case 5:

        course = fopen("courses.txt", "r");
        while (fgets(Cline, 100, course) != NULL)
        {
            Cname = (char *)malloc(50 * sizeof(char));
            sscanf(Cline, "%s %d %d", Cname, &Cnum, &slot);
            rootCourse = insert_Course(rootCourse, Cnum, Cname, slot);
        }
        fclose(course);

        printAVLTree_Course(rootCourse, 0);

        stu = fopen("students.txt", "r");
        while (fgets(Sline, 100, stu) != NULL)
        {
            Sname = (char *)malloc(50 * sizeof(char));
            sscanf(Sline, "%s %d %d %d %d %d %d ", Sname, &EnNum, &c1, &c2, &c3, &c4, &c5);
            c[0] = c1;
            c[1] = c2;
            c[2] = c3;
            c[3] = c4;
            c[4] = c5;
            HeadStudents = insertS(HeadStudents, EnNum, Sname, c, &rootCourse);
        }
        fclose(stu);

        class = fopen("classroom.txt", "r");
        while (fgets(Rline, 100, class) != NULL)
        {
            sscanf(Rline, "%d %d", &roomNum, &bpr);
            firstClass = insert(firstClass, roomNum, bpr);
        }
        fclose(class);

        printf("\n");
        printf("\n");
        printAVLTree_Classroom(firstClass, 0);
        printf("\n");
        printf("\n");
        // inOrder_Classroom(firstClass);

        Allot_for_lectures(firstClass, rootCourse);

        printf("\n");

        break;

    case 6:

        course = fopen("courses.txt", "r");
        while (fgets(Cline, 100, course) != NULL)
        {
            Cname = (char *)malloc(50 * sizeof(char));
            sscanf(Cline, "%s %d %d", Cname, &Cnum, &slot);
            rootCourse = insert_Course(rootCourse, Cnum, Cname, slot);
        }
        fclose(course);

        printAVLTree_Course(rootCourse, 0);

        stu = fopen("students.txt", "r");
        while (fgets(Sline, 100, stu) != NULL)
        {
            Sname = (char *)malloc(50 * sizeof(char));
            sscanf(Sline, "%s %d %d %d %d %d %d ", Sname, &EnNum, &c1, &c2, &c3, &c4, &c5);
            c[0] = c1;
            c[1] = c2;
            c[2] = c3;
            c[3] = c4;
            c[4] = c5;
            HeadStudents = insertS(HeadStudents, EnNum, Sname, c, &rootCourse);
        }
        fclose(stu);

        class = fopen("classroom.txt", "r");
        while (fgets(Rline, 100, class) != NULL)
        {
            sscanf(Rline, "%d %d", &roomNum, &bpr);
            firstClass = insert(firstClass, roomNum, bpr);
        }
        fclose(class);

        Allot_for_Exams(firstClass, rootCourse);
        printAVLTree_Classroom(firstClass, 0);

        break;

    case 7:
        course = fopen("courses.txt", "r");
        while (fgets(Cline, 100, course) != NULL)
        {
            Cname = (char *)malloc(50 * sizeof(char));
            sscanf(Cline, "%s %d %d", Cname, &Cnum, &slot);
            rootCourse = insert_Course(rootCourse, Cnum, Cname, slot);
        }
        fclose(course);

        int id1 ;
        int id2 ;

        printf("Enter Course id 1: \n");
        scanf("%d",&id1);

        printf("Enter Course id 2: \n");
        scanf("%d",&id2);

        printf("Range search between Course %d and %d \n", id1, id2);
        RangeSearch_Course(rootCourse, id1, id2);
    }

    return 0;
}