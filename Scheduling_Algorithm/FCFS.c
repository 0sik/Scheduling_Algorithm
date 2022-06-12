#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void initialize_queue();						//큐 초기화 함수
int insert_queue(int process_id, int priority,	//큐에 데이터 삽입하는 함수
	int computing_time, int arrive_time);
void print_queue();								//큐 출력 함수

typedef struct Queue* queue_p;			//Queue 구조체 포인터 변수 queue_p 선언
typedef struct Queue_head* head_p;		//Queue 구조체 포인터 변수 head_p 선언
queue_p queue;		//단일 큐 선언


typedef struct Queue {
	int process_id;				//프로세스 ID
	int priority;				//우선순위
	int computing_time;			//CPU 처리시간 == 서비스 타임
	int turnaround_time;		//응답시간
	int arrive_time;			//프로세스 도착시간
	queue_p left_link;			//Queue의 이전노드 링크
	queue_p right_link;			//Queue의 다음노드 링크
}Queue;


//main함수
int main() {
	int type = 0, arrive_time = 0;
	int process_id, priority, computing_time;
	int file_res;

	initialize_queue();		//큐 초기화
	FILE* file = NULL;
	file = fopen("input_test.txt", "rt");

	if (file != NULL) {
		while (1) {
			file_res = fscanf(file, "%d %d %d %d", &type, &process_id, &priority,
				&computing_time);

			if (type == 1) {
				arrive_time += 20;
			}
			else if (type == 0) {
				insert_queue(process_id, priority, computing_time, arrive_time);
			}
			else if (type == -1) {
				break;
			}
		}
		fclose(file);
	}

	print_queue();

	return 0;
}


//큐 초기화
void initialize_queue() {
	queue = (head_p)malloc(sizeof(struct Queue));
	queue->right_link = NULL;
}

//노드 삽입
int insert_queue(int process_id, int priority, int computing_time, int arrive_time) {
	queue_p p;			//구조체 포인터 변수 p
	queue_p node;		//새로운 노드 생성

	node = (queue_p)malloc(sizeof(struct Queue));	//새로운 노드 동적할당
	node->left_link = NULL;
	node->right_link = NULL;
	node->process_id = process_id;
	node->priority = priority;
	node->computing_time = computing_time;
	node->arrive_time = arrive_time;

	//큐 공백 검사
	if (queue->right_link == NULL) {
		queue->right_link = node;
		return 0;
	}

	p = queue->right_link;

	while (1) {
		if (p->right_link == NULL) {
			p->right_link = node;
			node->left_link = p;
			break;
		}
		else {
			p = p->right_link;
		}
	}

	return 0;
}

//큐 출력
void print_queue() {
	queue_p p;		//구조체 포인터 변수 p 선언
	int arrive_time = 0;
	printf("--------------------------------FCFS---------------------------------\n");
	printf("| procees_id\tcomputing_time\tturnaround_time\t\tarrive_time |\n");

	for (p = queue->right_link; p != NULL; p = p->right_link) {
		arrive_time = arrive_time + p->computing_time;	//이전 프로세스가 종료한 시간 저장 --> 다음 프로세스의 시작시간

		p->turnaround_time = arrive_time - p->arrive_time;	//turnaround_time 계산
		printf("| %d\t\t%d\t\t%d\t\t\t%d\t    |", p->process_id,
			p->computing_time, p->turnaround_time, p->arrive_time);
		printf("\n");
	}

	printf("---------------------------------------------------------------------\n");
}
