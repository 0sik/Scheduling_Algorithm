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
int turn_around_time_bucket = 0; // turn around time 을 측정하기 위한 전역변수
void insert_node(int, int, int, int); // process_id, priority, computing_time, enter_time 을 입력받기 위한 함수
void print_queue(void); // SPN 알고리즘에 따라 출력해주는 함수
void reset_head(void);// Head 초기화
int delete_queue(void); // 스케쥴링 완료 후 완료 된 노드를 삭제하는 함수


int main(void) {
    int type, process_id, priority, computing_time, f_result;
    int enter_time = 0;
    int x;


    FILE* file = fopen("data.txt", "rt");
    if (NULL == file) {
        printf("파일 열기 실패\n");
    }

    reset_head();

    printf("Process_id\tpriority\tcomputing_time\tturn_around time\n");
    if (NULL != file) {
        while (1) {
            f_result = fscanf(file, "%d %d %d %d", &type, &process_id, &priority, &computing_time);

            if (type == -1) break;

            if (type == 0)
                insert_node(process_id, priority, computing_time, enter_time); // 삽입
            if (type == 1) {
                enter_time += 20; //  시간이 20이 되었음을 나타낸다.
                x = delete_queue(); // 스케쥴링 된 노드는 큐에서 삭제되고, 헤더는 다음 큐를 가리킨다.
                if (x == 1) return 0;
            }
        }
        print_queue();
        fclose(file);
    }
    return 0;
}

void insert_node(int process_id, int priority, int computing_time, int enter_time) {
    queue_pointer a, newNode; //  노드 삽입을 위한 변수

    newNode = (queue_pointer)malloc(sizeof(NODE)); // 동적으로 새로운 노드 할당
    newNode->left_link = NULL;
    newNode->right_link = NULL;
    newNode->priority = priority;
    newNode->computing_time = computing_time;
    newNode->enter_time = enter_time;
    newNode->process_id = process_id;

    // 큐가 비어있는 경우
    if (head->right_link == NULL) {
        head->right_link = newNode;
        return;
    }

    // 큐의 가장 끝에 삽입
    for (a = head->right_link; a != NULL; a = a->right_link) {
        if (a->computing_time > computing_time) {
            if (a->left_link == NULL && a->computing_time > computing_time) // 맨 앞 
                break;
            else if (a->right_link == NULL && a->computing_time <= computing_time) // 맨 뒤
                break;
            a = a->left_link; // 중간에 노드를 삽입하기 위해 이전의 노드 주소를 지정한다.
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
    else if (a->right_link == NULL) { // 맨 뒤 삽입의 조건은 오른쪽 링크가 NULL 일 때 맨 마지막에 삽입
        a->right_link = newNode;
        newNode->left_link = a; // 맨 뒤에 삽입될 시, newNode 의 왼쪽링크는 포인터를 가리킨다.
        newNode->right_link = NULL; // 마지막의 오른쪽 링크는 NULL로 처리한다.
    }
    else { // 중간 노드에 삽입할 경우
        newNode->right_link = a->right_link;
        newNode->left_link = a;
        a->right_link->left_link = newNode;
        a->right_link = newNode;
        return;
    }
}

void print_queue(void) { // 스케쥴링 후 turn around time을 출력하고 끝난 시간을 return한다. 
    queue_pointer pointer;
    int turn_around_time;
    for (pointer = head->right_link; pointer != NULL; pointer = head->right_link) {
        turn_around_time_bucket = turn_around_time_bucket + pointer->computing_time; // 현재 스케쥴링 시간 + computing_time;

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

void reset_head(void) { // 초기화
    head = (head_pointer)malloc(sizeof(struct SPN_head));
    head->right_link = NULL;
}


int delete_queue(void) {
    queue_pointer pointer;
    int turn_around_time;

    pointer = head->right_link;
    if (pointer == NULL) { // 큐에 노드가 존재하지 않는데 DELETE_QUEUE를 실행하는 경우 종료 처리
        printf("큐에 노드가 존재하지 않는데 스케줄링을 시도했습니다.\n");
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