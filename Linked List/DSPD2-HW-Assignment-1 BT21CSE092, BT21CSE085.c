#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define slots_of_course 6

// BT21CSE092 Rohan Meshram
// BT21CSE085 Shakambari Kannade

// course(1)= DSPD2 (1);
// course(2)= LA (2);
// course(3)= IOOM (3);
// course(4)= CPL (4);
// course(5)= CO (5);
// course(6)= DCMP (6);
// course(7)= PTSM (2);
// course(8)= TC (4);

// slot(1) = 10:00 AM - 11:00 AM
// slot(2) = 11:00 AM - 12:00 AM
// slot(3) = 12:00 PM - 01:00 PM
// LUNCH   = 01:00 PM - 02:00 PM
// slot(4) = 02:00 PM - 03:00 PM
// slot(5) = 03:00 PM - 04:00 PM
// slot(6) = 04:00 PM - 05:00 PM

int Num_courses = 0;
int Num_classrooms = 0;

struct student
{
    char name[20];
    int En_num;
    int courses[5];
    int roomNum;
    int RowNum;
    int BenchNum;
    int attribute;
    struct student *next;
};

struct courses
{
    char Cname[20];
    int Cnum;
    int slot;
    int NumStudents;
    int lecture_room;
    int RoomNum;
    struct student *headstudent;
    struct courses *next;
};

struct classrooms
{
    int roomNum;
    int capacity;
    int seatOccupied;
    int seatNum;
    int seating[10][12];
    struct courses *C;
    struct courses *CL;
    struct classrooms *next;
};

void addcourse(struct courses **first, char *Cname, int CNum, int SlotNum) // to update a new course
{
    // Insert new item at start.

    struct courses *newest = (struct courses *)malloc(sizeof(struct courses));
    newest->Cnum = CNum;
    newest->slot = SlotNum;
    strcpy(newest->Cname, Cname);
    newest->NumStudents = 0;
    newest->headstudent = NULL;
    newest->next = *first;
    *first = newest;

    Num_courses++;
}

void addstudent(struct courses *first, int rollno, char *name) // to update student info in newly made linked list
{
    struct student *newest = (struct student *)malloc(sizeof(struct student));
    struct courses *ptr = first;
    // strcpy(newest->name,name);
    newest->En_num = rollno;
    newest->roomNum = 0;
    newest->BenchNum = 0;
    newest->RowNum = 0;
    strcpy(newest->name, name);
    ptr->NumStudents++;

    newest->next = first->headstudent;
    first->headstudent = newest;
}

struct courses *deleteStudent(struct courses *first, int EnNum)
{
    struct courses *p = first;
    while (p != NULL)
    {
        struct student *q = p->headstudent;
        struct student *prev = NULL;
        while (q != NULL)
        {
            if (q->En_num == EnNum)
            {
                if (prev == NULL)
                {
                    // Student to delete is the first student in the list
                    p->headstudent = q->next;
                }
                else
                {
                    // Student to delete is not the first student in the list
                    prev->next = q->next;
                }
                free(q);
                p->NumStudents--; // Decrement course enrollment count
                // return first;
            }
            prev = q;
            q = q->next;
        }
        p = p->next;
    }

    return first;
}

struct student *insertS(struct student *head, char *Sname, int EnNum, int *course, struct courses **firsts) // to update student info in linked list 1// as well as simultaneously updates students name in corresponding course taken by him.It returns the head pointer of linked list 1.
{

    struct student *NewS = (struct student *)malloc(sizeof(struct student));
    struct student *ptr = head;
    int i;
    int slots[slots_of_course] = {0};

    struct courses *ptr2 = *firsts;
    NewS->En_num = EnNum;
    NewS->roomNum = 0;
    NewS->RowNum = 0;
    NewS->BenchNum = 0;
    strcpy(NewS->name, Sname);

    for (i = 0; i < 5; i++)
    {
        int k = course[i];
        if (course[i] == k)
        {
            if (ptr2->Cnum == k)
            {
                addstudent(ptr2, EnNum, Sname);
            }
            else
            {
                while (ptr2->Cnum != k)
                {
                    ptr2 = ptr2->next;
                }
                addstudent(ptr2, EnNum, Sname);
                slots[ptr2->slot - 1]++;
                ptr2 = *firsts;
            }
        }
    }

    for (i = 0; i < slots_of_course; i++)
    {
        if (slots[i] > 1)
        {
            printf("%s cannot take two courses of same slot (%d)\n", Sname, i + 1);
            exit(0);
        }
    }
    for (i = 0; i < 5; i++)
    {
        NewS->courses[i] = course[i]; // updates courses taken by student in News
    }

    if (ptr == NULL)
    {
        ptr = NewS;
        NewS->next = NULL;
        head = ptr;
    }
    else
    {
        struct student *lptr = ptr;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = NewS;
        NewS->next = NULL;
        head = lptr;
    }

    return head;
}

struct student *deleteS(struct student *head, int EnNum, struct courses **first)
{
    struct student *p = head;

    struct courses *ptr = *first;
    if (p == NULL)
    {
        printf("No students to delete \n");
        return head;
    }

    if (p->En_num == EnNum)
    {
        head = p->next;
        free(p);
        printf("Student with Enrollment no. %d is deleted \n", EnNum);
    }
    else
    {
        struct student *q = head->next;
        while (q->En_num != EnNum && q->next!=NULL)
        {
            p = p->next;
            q = q->next;
        }
        if(q->En_num!=EnNum)
        {
            printf("No student found with enrollment no. %d \n",EnNum);
            printf("\n");
            return head;
        }
        p->next = q->next;
        free(q);
        printf("Student with Enrollment no. %d is deleted \n", EnNum);
        
    }
    ptr = deleteStudent(ptr, EnNum);
    *first = ptr;
    return head;
}

struct courses *deleteCourse(struct courses *head, int CNUM)
{
    struct courses *p = head;
    struct courses *q = head->next;

    if (p->Cnum == CNUM)
    {
        if (p->headstudent == NULL)
        {
            head = head->next;
            free(p);
            // return head;
        }
        else
        {
            printf("Cannot delete the course %d as some studentsa have opted for it\n", CNUM);
            return head;
        }
    }
    else
    {
        while (q->Cnum != CNUM && q != NULL)
        {
            p = p->next;
            q = q->next;
        }
        if (q->Cnum == CNUM && q->headstudent == NULL)
        {
            p->next = q->next;
            free(q);
            printf("Course %d is deleted \n",q->Cnum);
        }
        else if (q == NULL)
        {
            printf("this course does not exist");
        }
        else
        {
            printf("Cannot delete the course %d as some students have opted for it\n", CNUM);
            return head;
        }
    }

    return head;
}

void addClassroom(struct classrooms **first, int RNum, int BPR)
{
    struct classrooms *newC = (struct classrooms *)malloc(sizeof(struct classrooms));
    newC->roomNum = RNum;
    newC->capacity = (BPR)*12;
    newC->C = NULL;
    newC->seatOccupied = 0;
    newC->seatNum = 0;
    newC->next = *first;
    *first = newC;

    Num_classrooms++;
}

void printRooms(struct classrooms *currClass)
{
    struct classrooms *main = currClass ;
    if (main == NULL)
    {
        printf("no classrooms");
    }
    else
    {
        while (main != NULL)
        {
            printf("Classroom no. %d having capacity %d and alloted courses: \n", main->roomNum, main->capacity);

            struct courses *currCourse = main->CL;
            if (currCourse == NULL)
            {
                printf("No course alloted \n");
            }
            else
            {
                while (currCourse != NULL)
                {
                    printf("Course no. %d - %s \n", currCourse->Cnum, currCourse->Cname);
                    currCourse = currCourse->next;
                }
            }
            main = main->next;
            printf("\n");
        }
    }
}

void printall(struct courses *C)
{
    // For every child.
    struct courses * currcourse = C;
    if (currcourse == NULL)
    {
        printf("No data recorded yet");
    }
    else
    {
        while (currcourse != NULL)
        {
            printf("Course %s of slot %d is taken by:\n", currcourse->Cname, currcourse->slot);

            // For every toy that child has.

            struct student *currstu = currcourse->headstudent;
            if (currstu == NULL)
            {
                printf("   <<nothing>>\n");
            }
            else
            {
                while (currstu != NULL)
                {
                    printf("%d  ", currstu->En_num);
                    printf("%s \n", currstu->name);
                    currstu = currstu->next;
                }
            }
            currcourse = currcourse->next;
            printf("\n");
        }
    }
}

void printStudentList(struct student *ptr)
{
    if (ptr == NULL)
    {
        printf("no student recorded");
    }
    else
    {
        while (ptr != NULL)
        {
            printf("%d  %s \n", ptr->En_num, ptr->name);
            ptr = ptr->next;
        }
    }
}

void allotSeats(struct classrooms *first, struct courses **head)
{
    struct classrooms *ptr = first;
    struct courses *sub = *head;
    struct student *S = sub->headstudent;
    while (ptr != NULL)
    {
        int BPR = (ptr->capacity) / 4;
        int seating[4][BPR];
        while ((ptr->capacity) / 4 > 0 && sub != NULL)
        {
            while (S != NULL)
            {
                S->roomNum = ptr->roomNum;
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < BPR; j++)
                    {
                        S->RowNum = i + 1;
                        S->BenchNum = j + 1;
                        ptr->capacity = ptr->capacity - 4;
                    }
                }
                S = S->next;
            }
            sub = sub->next;
        }
        ptr = ptr->next;
    }
}

void printSeatalloted(struct student *S)
{
    do
    {
        printf("%s is alloted in classroom no. %d with bench no. %d and row no. %d \n", S->name, S->roomNum, S->BenchNum, S->RowNum);
        S = S->next;
    } while (S != NULL);
}

void allotRooms(struct courses *courseF, int numCourses, struct classrooms *roomF, int numRooms, int Slot)
{
    struct courses *course = courseF;
    struct classrooms *room = roomF;

    if (course->slot == Slot)
    {
        for (int j = 0; j < course->NumStudents; j++)
        {
            int S_Enum = course->headstudent->En_num;

            int Cindex = -1;

            int k = 0;

            while (k < numRooms && room != NULL)
            {
                if (room->capacity - room->seatOccupied >= course->NumStudents)
                {
                    Cindex = k;
                    break;
                }
                room = room->next;
                k++;
            }

            if (Cindex != -1)
            {
                int seatNum = room->seatOccupied;

                int x = 0;
                while (x < course->NumStudents && room != NULL)
                {
                    if (course->headstudent->En_num == S_Enum)
                    {
                        room->seatNum = S_Enum;
                    }
                    x++;
                    room = room->next;
                }
                room->seatOccupied = room->seatOccupied + course->NumStudents;
            }
            else
            {
                printf("Error: No classroom with enough capacity\n");
            }
        }
    }
    else
    {
        int i = 0;
        while (i < numCourses && course!=NULL)
        {
            for (int j = 0; j < course->NumStudents; j++)
            {
                int S_Enum = course->headstudent->En_num;

                int Cindex = -1;

                int k = 0;

                while (k < numRooms && room != NULL)
                {
                    if (room->capacity - room->seatOccupied >= course->NumStudents)
                    {
                        Cindex = k;
                        break;
                    }
                    room = room->next;
                    k++;
                }

                if (Cindex != -1)
                {
                    int seatNum = room->seatOccupied;

                    int x = 0;
                    while (x < course->NumStudents && room != NULL)
                    {
                        if (course->headstudent->En_num == S_Enum)
                        {
                            room->seatNum = S_Enum;
                        }
                        x++;
                        room = room->next;
                    }
                    room->seatOccupied = room->seatOccupied + course->NumStudents;
                }
                else
                {
                    printf("Error: No classroom with enough capacity\n");
                }
            }
            course=course->next;
        }
    }
}

struct classrooms *Insert_course_in_classroom(struct classrooms *cptr, struct courses *qptr)
{
    // struct classrooms*nptr=cptr;
    struct courses *newest = (struct courses *)malloc(sizeof(struct courses));
    newest->Cnum = qptr->Cnum;
    strcpy(newest->Cname, qptr->Cname);
    newest->NumStudents = qptr->NumStudents;
    // newest->NumStudents = qptr->NumStudents;
    newest->next = cptr->CL;
    cptr->CL = newest;

    return cptr;
}

struct classrooms *Insert_course_in_examroom(struct classrooms *cptr, struct courses *qptr)
{
    // struct classrooms*nptr=cptr;
    struct courses *newest = (struct courses *)malloc(sizeof(struct courses));
    newest->Cnum = qptr->Cnum;
    strcpy(newest->Cname, qptr->Cname);
    newest->NumStudents = qptr->NumStudents;
    // newest->NumStudents = qptr->NumStudents;
    newest->next = cptr->C;
    cptr->C = newest;

    return cptr;
}

void print_classrooms_for_lectures(struct classrooms *lptr)
{
    if (lptr == NULL)
    {
        printf("nothing");
    }
    else
    {
        while (lptr != NULL)
        {
            printf("classroom %d is alloted to :\n", lptr->roomNum);
            struct courses *nptr = lptr->CL;
            if (nptr == NULL)
            {
                printf("no courses alloted to this classroom");
            }
            else
            {
                while (nptr != NULL)
                {
                    printf("course number %d", nptr->Cnum);
                    printf("course name %s\n", nptr->Cname);
                }
            }
            lptr = lptr->next;
            printf("\n");
        }
    }
}

struct courses *merge_course(struct courses *lptr, struct courses *nptr)
{
    struct courses *result, *ptr1, *ptr2, *tail;
    ptr1 = lptr;
    ptr2 = nptr;
    if (ptr1->NumStudents > ptr2->NumStudents)
    {
        result = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        result = ptr2;
        ptr2 = ptr2->next;
    }
    tail = result;
    while (ptr1 != NULL && ptr2 != NULL)
    {
        if (ptr1->NumStudents > ptr2->NumStudents)
        {
            tail = tail->next = ptr1;
            ptr1 = ptr1->next;
        }
        else
        {
            tail = tail->next = ptr2;
            ptr2 = ptr2->next;
        }
    }
    if (ptr1 != NULL)
    {
        tail->next = ptr1;
    }
    else
    {
        tail->next = ptr2;
    }
    return result;
}

struct courses *divide_course(struct courses *lptr)
{
    struct courses *fast, *slow, *nptr;
    slow = lptr;
    fast = lptr->next->next;
    while (fast != NULL)
    {
        fast = fast->next;
        slow = slow->next;
        if (fast != NULL)
        {
            fast = fast->next;
        }
    }
    nptr = slow->next;
    slow->next = NULL;
    return nptr;
}

struct classrooms *merge(struct classrooms *lptr, struct classrooms *nptr)
{
    struct classrooms *result, *ptr1, *ptr2, *tail;
    ptr1 = lptr;
    ptr2 = nptr;
    if (ptr1->capacity > ptr2->capacity)
    {
        result = ptr1;
        ptr1 = ptr1->next;
    }
    else
    {
        result = ptr2;
        ptr2 = ptr2->next;
    }
    tail = result;
    while (ptr1 != NULL && ptr2 != NULL)
    {
        if (ptr1->capacity > ptr2->capacity)
        {
            tail = tail->next = ptr1;
            ptr1 = ptr1->next;
        }
        else
        {
            tail = tail->next = ptr2;
            ptr2 = ptr2->next;
        }
    }
    if (ptr1 != NULL)
    {
        tail->next = ptr1;
    }
    else
    {
        tail->next = ptr2;
    }
    return result;
}

struct classrooms *divide(struct classrooms *lptr)
{
    struct classrooms *fast, *slow, *nptr;
    slow = lptr;
    fast = lptr->next->next;
    while (fast != NULL)
    {
        fast = fast->next;
        slow = slow->next;
        if (fast != NULL)
        {
            fast = fast->next;
        }
    }
    nptr = slow->next;
    slow->next = NULL;
    return nptr;
}

struct classrooms *merge_sort(struct classrooms *list_ptr) // sorts in descending order.
{
    struct classrooms *nptr, *lptr;
    lptr = list_ptr;
    if ((lptr != NULL) && (lptr->next != NULL))
    {
        nptr = divide(lptr);
        lptr = merge_sort(lptr);
        nptr = merge_sort(nptr);
        lptr = merge(lptr, nptr);
    }
    return lptr;
}

struct courses *merge_sort_course(struct courses *list_ptr) // sorts in descending order.
{
    struct courses *nptr, *lptr;
    lptr = list_ptr;
    if ((lptr != NULL) && (lptr->next != NULL))
    {
        nptr = divide_course(lptr);
        lptr = merge_sort_course(lptr);
        nptr = merge_sort_course(nptr);
        lptr = merge_course(lptr, nptr);
    }
    return lptr;
}

void allot_for_lectures(struct classrooms *list_ptr, struct courses **head)
{
    // printf("printing initially\n");
    // printRooms(list_ptr);
    // printf("printing courses\n");
    // printall(*head);
    struct classrooms *lptr = list_ptr;
    lptr = merge_sort(lptr); // classrooms available in descending order
    struct courses *mptr = *head;

    if (mptr == NULL)
    {
        printf("no courses to allot seats to\n");
    }
    else
    {
        for (int k = 1; k <= slots_of_course; k++)
        {
            mptr = *head;
            struct courses *nptr = NULL; // linked list of courses of k slot
            struct courses *qptr = NULL;
            int flag = 1; // will point to the head of nptr waala ll
           
            while (mptr != NULL)
            {
                
                if (mptr->slot == k) // insert at end
                {
                    if (nptr == NULL)
                    {
                        struct courses *newptr = (struct courses *)malloc(sizeof(struct courses));
                        newptr->Cnum = mptr->Cnum;
                        strcpy(newptr->Cname, mptr->Cname);
                        newptr->slot = mptr->slot;
                        newptr->NumStudents = mptr->NumStudents;
                        newptr->lecture_room = -1;
                        nptr = qptr = newptr;
                        mptr = mptr->next;
                    }
                    else
                    {
                        struct courses *newptr = (struct courses *)malloc(sizeof(struct courses));
                        newptr->Cnum = mptr->Cnum;
                        strcpy(newptr->Cname, mptr->Cname);
                        newptr->slot = mptr->slot;
                        newptr->NumStudents = mptr->NumStudents;
                        newptr->lecture_room = -1;
                        nptr->next = newptr;
                        nptr = nptr->next;
                        mptr = mptr->next;
                    }
                }
                else
                {
                    mptr = mptr->next;
                }
            }
            if (nptr != NULL)
            {
                nptr->next = NULL; // code crashes here for k=3
            }

            if (qptr == NULL)
            {
                printf("\n");
                printf("no courses to allot in %d slot\n", k);
            }
            else
            {
                qptr = merge_sort_course(qptr); //  sorted linkes list of couses of k slot with respect to maximum numstudents.
               // printf("printing sorted linked list of courses\n");
                // printall(qptr);
                if (qptr == NULL)
                {
                    printf("No subjects of this slot\n");
                }
                struct classrooms *cptr = lptr;
                struct courses *temp = qptr;
                while (qptr != NULL && cptr != NULL)
                {
                    if (cptr->capacity < qptr->NumStudents)
                    {
                        printf("no of students in this course are more than largest free classroom hence cant allot\n");
                        flag = 0;
                        qptr = qptr->next;
                    }
                    else
                    {
                        //printf("HI \n");
                        qptr->lecture_room = cptr->roomNum;
                        cptr = Insert_course_in_classroom(cptr, qptr);
                        qptr = qptr->next;
                        cptr = cptr->next;
                    }
                }
                if (cptr == NULL)
                {
                    printf("no classrooms available to allot\n");
                }
                else if (qptr == NULL)
                {
                    // printf("allotment completed\n");
                    printf("\n");
                    printf("allotment of courses of slot %d : \n", k);
                    printf("\n");
                    while (temp != NULL)
                    {
                        printf("course %s of course number %d alloted to roomnumber %d\n", temp->Cname, temp->Cnum, temp->lecture_room); // flag condition to be incorporated.
                        temp = temp->next;
                    }
                }
            }
        }
    }
    // printf("\n");
    // printRooms(lptr);
}

void allot_for_exams(struct classrooms *list_ptr, struct courses **head)
{
    struct classrooms *lptr = list_ptr;
    lptr = merge_sort(lptr); // classrooms available in descending order
    // printRooms(lptr);
    int flag = 0, complete = 0, tag = 0, alloted = 0, stu = 1, nostu = 0, remaining = 1, remaining1 = 1,remaining2=1;
    struct courses *mptr = *head;
    int b1 = 0, r1 = 0, b2 = 0, r2 = 0, k;

    if (mptr == NULL)
    {
        printf("No courses to allot seats to\n");
    }
    else
    {
        for (k = 1; k <= slots_of_course; k++)
        {

            stu = 1;
            // if (k == 5)
            // {
            //     printf("k is 5\n");
            // }
            // if (k == 6)
            // {
            //     printf("k is 6\n");
            // }
            // if (k == 7)
            // {
            //     printf("k is 7\n");
            // }
            mptr = *head;
            struct courses *nptr = NULL; // linked list of courses of k slots
            struct courses *qptr = NULL;
            while (mptr != NULL)
            {
                if (mptr->slot == k)
                {
                    if (nptr == NULL)
                    {
                        struct courses *newptr = (struct courses *)malloc(sizeof(struct courses));
                        newptr->Cnum = mptr->Cnum;
                        strcpy(newptr->Cname, mptr->Cname);
                        newptr->slot = mptr->slot;
                        newptr->NumStudents = mptr->NumStudents;
                        newptr->RoomNum = -1;
                        newptr->headstudent = mptr->headstudent;
                        nptr = qptr = newptr;
                        mptr = mptr->next;
                    }
                    else
                    {
                        struct courses *newptr = (struct courses *)malloc(sizeof(struct courses));
                        newptr->Cnum = mptr->Cnum;
                        strcpy(newptr->Cname, mptr->Cname);
                        newptr->slot = mptr->slot;
                        newptr->NumStudents = mptr->NumStudents;
                        newptr->RoomNum = -1;
                        newptr->headstudent = mptr->headstudent;
                        nptr->next = newptr;
                        nptr = nptr->next;
                        mptr = mptr->next;
                    }
                }
                else
                {
                    mptr = mptr->next;
                }
            }
            if (nptr != NULL)
            {
                nptr->next = NULL; // code crashes here for k=3
            }

            if (qptr == NULL)
            {
                printf("No courses to allot in %d slot\n", k);
            }
            else
            {
                qptr = merge_sort_course(qptr); //  sorted linked list of courses of k slot with respect to maximum numstudents.
                // if(k==6){
                //     printf("here again\n");
                // }
                if (qptr == NULL)
                {
                    printf("No subjects of %d slot\n", k);
                    flag = 1;
                }
                struct classrooms *cptr = lptr; // points towards classroom in which seat is being alloted
                struct courses *temp = qptr;
                struct courses *ptr1 = NULL, *ptr2 = NULL; // to point towars course being alloted
                struct student *p1, *hp1, *p2, *hp2;       // to point towars the student being alloted the seat
                if (flag == 0)
                {
                    // if (k == 6)
                    // {
                    //     printf("reached for k 6\n");
                    //     printf("stu value %d",stu);
                    // }
                    // end conditions to be checked
                    if (ptr1 == NULL && ptr2 == NULL)
                    {
                        ptr1 = qptr;
                        ptr2 = qptr->next;
                        tag = 1;
                    }
                    if (ptr1 != NULL && ptr2 == NULL && stu == 1)
                    {
                        printf("%s\n", ptr1->Cname);
                        p1 = hp1 = ptr1->headstudent;
                        ptr1->RoomNum = cptr->roomNum;
                        cptr = Insert_course_in_examroom(cptr, ptr1);
                        int bpr = (cptr->capacity) / 12;
                        int t = bpr;
                        int tempCap = cptr->capacity;
                        b1 = 1;
                        r1 = 1;
                        if (p1 == NULL)
                        {
                            printf("No students in this course\n");
                            stu = 0;
                        }
                        while (p1 != NULL && tempCap > 0 && remaining == 1)
                        {

                            // while (p1 != NULL )
                            // {
                            printf("Printing seats for slot %d\n", k);
                            // printf("hellloooooo\n");
                            while (t > 0 && p1 != NULL && remaining == 1)
                            {
                                p1->attribute = 1; // since we r assigning for subject1
                                p1->roomNum = cptr->roomNum;
                                p1->BenchNum = b1;
                                b1++;
                                p1->RowNum = r1;
                                t--;
                                // printf("%d examroom %d benchnumber %d rownumber and %d attribute seat assigned to %s \n", p1->roomNum, p1->BenchNum, p1->RowNum, p1->attribute, p1->name);
                                printf("%d %s is assigned classroom no. %d with bench no. %d, row no. %d and attirbute %d\n ", p1->En_num, p1->name, p1->roomNum, p1->BenchNum, p1->RowNum, p1->attribute);
                                tempCap = tempCap - 3;
                                // printf("p1 printing\n");
                                p1 = p1->next;
                                // if (p1 == NULL)
                                // {
                                //     printf("NULL");
                                // }
                                // if (t == 0)
                                // {
                                //     printf("t is 0");
                                // }
                                if (t == 0 && r1 < 4 && p1 != NULL)
                                {
                                    // printf("entered");
                                    r1 = r1 + 1;
                                    t = bpr;
                                    b1 = 1;
                                }
                                else if (r1 >= 4 && p1 != NULL && b1 > bpr)
                                {
                                    // printf("%d\n",b1);
                                    struct student *s1 = p1;
                                    printf("Remaining Students: \n");
                                    while (s1 != NULL)
                                    {

                                        printf("%s    %d\n", s1->name, s1->En_num);
                                        s1 = s1->next;
                                        remaining = 0;
                                    }
                                }
                            }
                            // printf("whats the problem\n");
                            // }
                            // printf("bye\n");
                        }
                    }
                    while (ptr2 != NULL && nostu == 0) // latest change
                    {
                        // printf("into while\n");

                        if (ptr2 != NULL)
                        {
                            if (tag == 0)
                            {
                                ptr1 = ptr1->next;
                                ptr2 = ptr2->next;

                                if (ptr2 == NULL)
                                {
                                    printf("Allotment completed for %d slot courses", k);
                                    printf("\n");
                                    complete = 1;
                                }
                                if (ptr2 != NULL)
                                {
                                    // printf("reached till here");
                                    ptr2 = ptr2->next;
                                    ptr1 = ptr1->next;
                                    cptr = cptr->next;
                                    if (cptr == NULL)
                                    {
                                        printf("No classrooms left\n");
                                        complete = 1;
                                    }
                                }
                            }
                        }
                        if (complete == 0)
                        {
                            // printf("%s\n",ptr2->Cname);
                            p1 = hp1 = ptr1->headstudent;
                            ptr1->RoomNum = cptr->roomNum;
                            cptr = Insert_course_in_examroom(cptr, ptr1);
                            if (ptr2 != NULL)
                            {
                                // printf("ptr2!=NULL\n");
                                printf("%s", ptr2->Cname);
                                p2 = ptr2->headstudent;
                                // printf("%s\n", p2->name);
                                alloted = 1;
                                ptr2->RoomNum = cptr->roomNum;

                                cptr = Insert_course_in_examroom(cptr, ptr2);
                            }
                            // a1 = 1;
                            int bpr = (cptr->capacity) / 12;
                            int t = bpr;
                            // printf("\n capacity of classroom : %d\n",cptr->capacity);
                            // printf("\n%d\n",bpr);
                            int tempCap = cptr->capacity;
                            b1 = 1;
                            r1 = 1;
                            while (p1 != NULL && tempCap > 0 && remaining1 == 1)
                            {

                                // while (p1 != NULL)
                                // {

                                printf("printing seats for slot %d\n", k);
                                // printf("hii\n");
                                while (t > 0 && p1 != NULL && remaining1 == 1)
                                {
                                    p1->attribute = 1; // since we r assigning for subject1
                                    p1->roomNum = cptr->roomNum;
                                    p1->BenchNum = b1;
                                    b1++;
                                    p1->RowNum = r1;
                                    t--;
                                    // printf("%d examroom %d benchnumber %d rownumber and %d attribute seat assigned to %s \n", p1->roomNum, p1->BenchNum, p1->RowNum, p1->attribute, p1->name);
                                    printf("%d %s is assigned classroom no. %d with bench no. %d, row no. %d and attirbute %d\n ", p1->En_num, p1->name, p1->roomNum, p1->BenchNum, p1->RowNum, p1->attribute);
                                    tempCap = tempCap - 3;
                                    // printf("p1 printing\n");
                                    p1 = p1->next;
                                    // if (p1 == NULL)
                                    // {
                                    //     printf("NULL");
                                    // }
                                    // if (t == 0)
                                    // {
                                    //     printf("t is 0");
                                    // }
                                    if (t == 0 && r1 < 4 && p1 != NULL)
                                    {
                                        // printf("entered");
                                        r1 = r1 + 1;
                                        t = bpr;
                                        b1 = 1;
                                    }
                                    else if (r1 >= 4 && p1 != NULL && b1 > bpr)
                                    {
                                        // printf("%d\n",b1);
                                        struct student *s1 = p1;
                                        printf("Remaining Students: \n");
                                        while (s1 != NULL)
                                        {

                                            printf("%s    %d\n", s1->name, s1->En_num);
                                            s1 = s1->next;
                                            remaining1 = 0;
                                        }
                                    }
                                }
                                // printf("whats the problem\n");
                                // }
                                // printf("bye\n");
                            }
                            // printf("excuse me\n");
                            b2 = 1, r2 = 1;
                            tempCap = cptr->capacity;
                            t = bpr;
                            if (p2 == NULL && ptr2 != NULL)
                            {
                                printf("No students in %s to allot seats to\n", ptr2->Cname);
                                nostu = 1;
                            }
                            while (tempCap > 0 && p2 != NULL && alloted == 1 && remaining2==1)
                            {
                                // printf("hi");

                                while (p2 != NULL && remaining2==1)
                                {
                                
                                    
                                        
                                    
                                    // printf("hello");
                                    while (t > 0 && p2 != NULL && remaining2==1)
                                    {
                                        // printf("shakambari here");
                                        p2->attribute = 2; // since we r assigning for subject2
                                        p2->roomNum = cptr->roomNum;
                                        p2->BenchNum = b2;
                                        b2++;
                                        p2->RowNum = r2;
                                        printf("%d %s is assigned classroom no. %d with bench no. %d, row no. %d and attirbute %d\n ", p2->En_num, p2->name, p2->roomNum, p2->BenchNum, p2->RowNum, p2->attribute);
                                        // printf("%d examroom %d benchnumber %d rownumber and %d attribute seat assigned to %s\n ", p2->roomNum, p2->BenchNum, p2->RowNum, p2->attribute, p2->name);
                                        t--;
                                        // printf("p2 printing\n");
                                        tag = 0;
                                        tempCap = tempCap - 3;
                                        p2 = p2->next;
                                        if (t == 0 && r2 < 4 && p2 != NULL)
                                        {
                                            r2 = r2 + 1;
                                            t = bpr;
                                            b2 = 1;
                                        }
                                        else if (r2>=4 && p2 != NULL && b2>bpr) // remaining children to be placed
                                        {
                                            struct student *s2 = p2;
                                            printf("Remaining Students: \n");
                                            while (s2 != NULL)
                                            {

                                                printf("%s    %d\n", s2->name, s2->En_num);
                                                s2 = s2->next;
                                                remaining2=0;
                                            }
                                        }
                                    }
                                }
                            }
                            // printf("i am here\n");
                            // if (ptr2 != NULL)
                            // {
                            //     printf("i am not null\n");
                            // }

                            if (tempCap == 0)
                            {
                                if (p1 != NULL) // remaining children to be placed
                                {
                                    struct student *s1 = p1->next;
                                    printf("Remaining Students: \n");
                                    while (s1 != NULL)
                                    {

                                        printf("%s    %d\n", s1->name, s1->En_num);
                                        s1 = s1->next;
                                    }
                                }
                                if (p2 != NULL) // remaining children to be placed
                                {
                                    struct student *s2 = p2->next;
                                    printf("Remaining Students: \n");
                                    while (s2 != NULL)
                                    {

                                        printf("%s    %d\n", s2->name, s2->En_num);
                                        s2 = s2->next;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
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

    scanf("%d", &q);

    struct courses *first_course = NULL;
    struct student *head = NULL;
    struct classrooms *head_class = NULL;

    FILE *fptr;
    char line[100];
     
    FILE *course;
    char *Cname;
    int Cnum,slot;
    char Cline[100];

    FILE *stu;
    int c[5],c1,c2,c3,c4,c5,EnNum;
    char *Sname;
    char Sline[100];

    FILE *class;
    int bpr,roomNum;
    char Rline[100];

    switch (q)
    {
    case 1:


        course = fopen("courses.txt","r");
        while(fgets(Cline,100,course)!=NULL)
        {
            Cname = (char *)malloc(50*sizeof(char));
            sscanf(Cline,"%s %d %d",Cname,&Cnum,&slot);
            addcourse(&first_course,Cname,Cnum,slot);
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
            head = insertS(head, Sname, EnNum, c, &first_course);
        }
        fclose(stu);

        printall(first_course);

        printStudentList(head);
        break;

    case 2:

        course = fopen("courses.txt","r");
        while(fgets(Cline,100,course)!=NULL)
        {
            Cname = (char *)malloc(50*sizeof(char));
            sscanf(Cline,"%s %d %d",Cname,&Cnum,&slot);
            addcourse(&first_course,Cname,Cnum,slot);
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
            head = insertS(head, Sname, EnNum, c, &first_course);
        }
        fclose(stu);

        // printall(first_course);
        printStudentList(head);
        fptr = fopen("question2.txt", "r");
        while (fgets(line, 100, fptr) != NULL)
        {
            sscanf(line, "%d", &EnNum);
            head = deleteS(head, EnNum, &first_course);
        }
        printStudentList(head);
        fclose(fptr);

        // printall(first_course);

        break;

    case 3:

        course = fopen("courses.txt","r");
        while(fgets(Cline,100,course)!=NULL)
        {
            Cname = (char *)malloc(50*sizeof(char));
            sscanf(Cline,"%s %d %d",Cname,&Cnum,&slot);
            addcourse(&first_course,Cname,Cnum,slot);
        }
        fclose(course);

        printall(first_course);
        break;

    case 4:

        course = fopen("courses.txt","r");
        while(fgets(Cline,100,course)!=NULL)
        {
            Cname = (char *)malloc(50*sizeof(char));
            sscanf(Cline,"%s %d %d",Cname,&Cnum,&slot);
            addcourse(&first_course,Cname,Cnum,slot);
        }
        fclose(course);

        fptr = fopen("question4.txt", "r");
        while (fgets(line, 100, fptr) != NULL)
        {
            sscanf(line, "%d", &Cnum);
            first_course = deleteCourse(first_course, Cnum);
        }
        fclose(fptr);

        printall(first_course);
        break;

    case 5:

        course = fopen("courses.txt","r");
        while(fgets(Cline,100,course)!=NULL)
        {
            Cname = (char *)malloc(50*sizeof(char));
            sscanf(Cline,"%s %d %d",Cname,&Cnum,&slot);
            addcourse(&first_course,Cname,Cnum,slot);
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
            head = insertS(head, Sname, EnNum, c, &first_course);
        }
        fclose(stu);

        class = fopen("classrooms.txt","r");
        while (fgets(Rline, 100, class) != NULL)
        {
            sscanf(Rline, "%d %d", &roomNum, &bpr);
            addClassroom(&head_class,roomNum,bpr);
        }
        fclose(class);

        allot_for_lectures(head_class,&first_course);
        printf("\n");
        printRooms(head_class);

        break;    

    case 6:

        course = fopen("courses.txt","r");
        while(fgets(Cline,100,course)!=NULL)
        {
            Cname = (char *)malloc(50*sizeof(char));
            sscanf(Cline,"%s %d %d",Cname,&Cnum,&slot);
            addcourse(&first_course,Cname,Cnum,slot);
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
            head = insertS(head, Sname, EnNum, c, &first_course);
        }
        fclose(stu);


        class = fopen("classrooms.txt", "r");
        while (fgets(line, 100, class) != NULL)
        {
            sscanf(line, "%d %d", &roomNum, &bpr);
            addClassroom(&head_class,roomNum,bpr);
        }
        fclose(class);
        
        allot_for_exams(head_class,&first_course);
        printRooms(head_class);

        break;
    }

    return 0;
}
