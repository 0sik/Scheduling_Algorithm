#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void initialize_queue();						//ť �ʱ�ȭ �Լ�
int insert_queue(int process_id, int priority,	//ť�� ������ �����ϴ� �Լ�
	int computing_time, int arrive_time);
void print_queue();								//ť ��� �Լ�

typedef struct Queue* queue_p;			//Queue ����ü ������ ���� queue_p ����
typedef struct Queue_head* head_p;		//Queue ����ü ������ ���� head_p ����
queue_p queue;		//���� ť ����


typedef struct Queue {
	int process_id;				//���μ��� ID
	int priority;				//�켱����
	int computing_time;			//CPU ó���ð� == ���� Ÿ��
	int turnaround_time;		//����ð�
	int arrive_time;			//���μ��� �����ð�
	queue_p left_link;			//Queue�� ������� ��ũ
	queue_p right_link;			//Queue�� ������� ��ũ
}Queue;


//main�Լ�
int main() {
	int type = 0, arrive_time = 0;
	int process_id, priority, computing_time;
	int file_res;

	initialize_queue();		//ť �ʱ�ȭ
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


//ť �ʱ�ȭ
void initialize_queue() {
	queue = (head_p)malloc(sizeof(struct Queue));
	queue->right_link = NULL;
}

//��� ����
int insert_queue(int process_id, int priority, int computing_time, int arrive_time) {
	queue_p p;			//����ü ������ ���� p
	queue_p node;		//���ο� ��� ����

	node = (queue_p)malloc(sizeof(struct Queue));	//���ο� ��� �����Ҵ�
	node->left_link = NULL;
	node->right_link = NULL;
	node->process_id = process_id;
	node->priority = priority;
	node->computing_time = computing_time;
	node->arrive_time = arrive_time;

	//ť ���� �˻�
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

//ť ���
void print_queue() {
	queue_p p;		//����ü ������ ���� p ����
	int arrive_time = 0;
	printf("--------------------------------FCFS---------------------------------\n");
	printf("| procees_id\tcomputing_time\tturnaround_time\t\tarrive_time |\n");

	for (p = queue->right_link; p != NULL; p = p->right_link) {
		arrive_time = arrive_time + p->computing_time;	//���� ���μ����� ������ �ð� ���� --> ���� ���μ����� ���۽ð�

		p->turnaround_time = arrive_time - p->arrive_time;	//turnaround_time ���
		printf("| %d\t\t%d\t\t%d\t\t\t%d\t    |", p->process_id,
			p->computing_time, p->turnaround_time, p->arrive_time);
		printf("\n");
	}

	printf("---------------------------------------------------------------------\n");
}
