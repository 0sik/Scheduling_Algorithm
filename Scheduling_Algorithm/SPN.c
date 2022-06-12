#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>


typedef struct SPN* queue_pointer;
typedef struct SPN {
    int type;
    int process_id;
    int priority;
    int computing_time;
    int enter_time;
    queue_pointer left_link;
    queue_pointer right_link;
}NODE;

typedef struct SPN_head* head_pointer;
struct SPN_head {
    queue_pointer right_link;
};


head_pointer head;
int turn_around_time_bucket = 0; // turn around time �� �����ϱ� ���� ��������
void insert_node(int, int, int, int); // process_id, priority, computing_time, enter_time �� �Է¹ޱ� ���� �Լ�
void print_queue(void); // SPN �˰��� ���� ������ִ� �Լ�
void reset_head(void);// Head �ʱ�ȭ
int delete_queue(void); // �����층 �Ϸ� �� �Ϸ� �� ��带 �����ϴ� �Լ�


int main(void) {
    int type, process_id, priority, computing_time, f_result;
    int enter_time = 0;
    int x;


    FILE* file = fopen("data.txt", "rt");
    if (NULL == file) {
        printf("���� ���� ����\n");
    }

    reset_head();

    printf("Process_id\tpriority\tcomputing_time\tturn_around time\n");
    if (NULL != file) {
        while (1) {
            f_result = fscanf(file, "%d %d %d %d", &type, &process_id, &priority, &computing_time);

            if (type == -1) break;

            if (type == 0)
                insert_node(process_id, priority, computing_time, enter_time); // ����
            if (type == 1) {
                enter_time += 20; //  �ð��� 20�� �Ǿ����� ��Ÿ����.
                x = delete_queue(); // �����층 �� ���� ť���� �����ǰ�, ����� ���� ť�� ����Ų��.
                if (x == 1) return 0;
            }
        }
        print_queue();
        fclose(file);
    }
    return 0;
}

void insert_node(int process_id, int priority, int computing_time, int enter_time) {
    queue_pointer a, newNode; //  ��� ������ ���� ����

    newNode = (queue_pointer)malloc(sizeof(NODE)); // �������� ���ο� ��� �Ҵ�
    newNode->left_link = NULL;
    newNode->right_link = NULL;
    newNode->priority = priority;
    newNode->computing_time = computing_time;
    newNode->enter_time = enter_time;
    newNode->process_id = process_id;

    // ť�� ����ִ� ���
    if (head->right_link == NULL) {
        head->right_link = newNode;
        return;
    }

    // ť�� ���� ���� ����
    for (a = head->right_link; a != NULL; a = a->right_link) {
        if (a->computing_time > computing_time) {
            if (a->left_link == NULL && a->computing_time > computing_time) // �� �� 
                break;
            else if (a->right_link == NULL && a->computing_time <= computing_time) // �� ��
                break;
            a = a->left_link; // �߰��� ��带 �����ϱ� ���� ������ ��� �ּҸ� �����Ѵ�.
            break;
        }
        else if (a->right_link == NULL)
            break;
    }

    if (a->computing_time > computing_time && a->left_link == NULL) {
        a->left_link = newNode;
        head->right_link = newNode;
        newNode->right_link = a;
    }
    else if (a->right_link == NULL) { // �� �� ������ ������ ������ ��ũ�� NULL �� �� �� �������� ����
        a->right_link = newNode;
        newNode->left_link = a; // �� �ڿ� ���Ե� ��, newNode �� ���ʸ�ũ�� �����͸� ����Ų��.
        newNode->right_link = NULL; // �������� ������ ��ũ�� NULL�� ó���Ѵ�.
    }
    else { // �߰� ��忡 ������ ���
        newNode->right_link = a->right_link;
        newNode->left_link = a;
        a->right_link->left_link = newNode;
        a->right_link = newNode;
        return;
    }
}

void print_queue(void) { // �����층 �� turn around time�� ����ϰ� ���� �ð��� return�Ѵ�. 
    queue_pointer pointer;
    int turn_around_time;
    for (pointer = head->right_link; pointer != NULL; pointer = head->right_link) {
        turn_around_time_bucket = turn_around_time_bucket + pointer->computing_time; // ���� �����층 �ð� + computing_time;

        turn_around_time = turn_around_time_bucket - pointer->enter_time;
        printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, turn_around_time);

        if (pointer->right_link == NULL) {
            head->right_link = NULL;
            free(pointer);
        }
        else {
            head->right_link = pointer->right_link;
            pointer->right_link->left_link = NULL;
            pointer->right_link = NULL;
            free(pointer);
        }

    }
}

void reset_head(void) { // �ʱ�ȭ
    head = (head_pointer)malloc(sizeof(struct SPN_head));
    head->right_link = NULL;
}


int delete_queue(void) {
    queue_pointer pointer;
    int turn_around_time;

    pointer = head->right_link;
    if (pointer == NULL) { // ť�� ��尡 �������� �ʴµ� DELETE_QUEUE�� �����ϴ� ��� ���� ó��
        printf("ť�� ��尡 �������� �ʴµ� �����ٸ��� �õ��߽��ϴ�.\n");
        return 1;
    }
    turn_around_time_bucket = turn_around_time_bucket + pointer->computing_time;
    turn_around_time = turn_around_time_bucket - pointer->enter_time;
    printf("%d\t\t%d\t\t%d\t\t%d\n", pointer->process_id, pointer->priority, pointer->computing_time, turn_around_time);
    if (pointer->right_link == NULL) {
        head->right_link = NULL;
        free(pointer);
        return 0;
    }
    else {
        head->right_link = pointer->right_link;
        pointer->right_link->left_link = NULL;
        free(pointer);
        return 0;
    }
}