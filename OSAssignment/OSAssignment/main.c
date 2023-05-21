//
//  main.c
//  OSAssignment
//
//  Created by Robin Han on 2023/05/14.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process{
    int type;
    int priority;
    int process_id;
    int computing_time;
    int startTime;
    int endTime;
    struct Process* next;
    struct Process* prev;
}process;


//Finished  Process
typedef struct {
    process* head;
}List;//Insert process

//해당레디큐로 실제 사용되는 리스트
typedef struct{
    process* start;
    process* end;
}Queue;//ReadyQueue


//CreateList
List * createLinkedList(void);
//CreateQueue
Queue * createQueue(void);
//CreateProcessnode
process * createProcess(Queue*Q, int type,  int process_id, int priority,int computing_time, int startTime);
//Connect prev next
void connector(Queue*Q,process* node);

//Print
void printProcess(Queue* Q);
void printFinish(List* L);
//Free malloc
void freeList(List* L);
void freeQueue(Queue*L);

//Send done proccess to last of the queue
void sendBack(Queue *Q);

//processing
int processActive(Queue*q,List*L, int timeQ, int currentTime);

//mode the finished to List
void finishedProcess(Queue * Q, List *L,int currentTime);


int main(void) {
// 입력되는 값
// type  // 0: Process 생성 프로세스 입력//  1:스케줄링 시작// -1:스케줄링 완료
    int type, priority, process_id, computing_time = 0 ;//input values
    int robin = 20; // timeQuantum
    int currentTime = 0; // to track time
    List *L = createLinkedList();
    Queue * Q = createQueue();
//    printf("time_quantum\n");
//    scanf(" %d", &robin);
    while(1){
        printf("type\n");
        scanf(" %d", &type);
        switch(type){
            case 0:
          //Starting time
                printf("priority\n");
                scanf(" %d", &priority);
                printf("process_id\n");
                scanf(" %d", &process_id);
                printf("computing_time\n");
                scanf(" %d", &computing_time);
                createProcess(Q, type, process_id,priority,computing_time,currentTime);
                break;
            case 1:
                printf("\ntime to Schedule\n");
                processActive(Q,L, robin,currentTime);
                break;
            case -1:
                printf("end of Process\n");
                freeList(L);
                freeQueue(Q);
                break;
            default:
                printf("Error\n");
                break;
        }
        currentTime += robin;
    }
    return 0;
}


List * createLinkedList(void){
  List * L ;
  L  = (List*)malloc(sizeof(List));
  L->head=NULL;
  return L ;
}

Queue * createQueue(void){
    Queue* Q =(Queue*)malloc(sizeof(Queue));
    Q->start=NULL;
    Q->end=NULL;
    return Q;
}



//Proess 생성만 해주고 넘긴다.
process * createProcess(Queue* Q, int type,  int process_id,int priority, int computing_time, int startTime){
    process* node =(process*)malloc(sizeof(process));
    node->type = type;
    node->priority=priority;
    node->process_id=process_id;
    node->computing_time=computing_time;
    node->startTime = startTime;
    node->endTime=0;
    node->prev = NULL;
    node->next = NULL;
    //맨 처음 노드
    connector(Q, node);
    return node;
}

//Connect Next and prev
void connector(Queue*Q,process* node){
    process* temp = Q->start;
    process* previous = Q->start;
    if(Q->start == NULL){
        Q->start =node;
    }
    else{
        //Connect prev and next
        while( temp != NULL){
            if( temp == Q->start){
                temp = temp->next;
            }
            else{
                temp = temp->next;
                previous = previous->next;
            }
        }
        previous->next= node;
        node->prev = previous;
        temp = node;
        Q->end = node;
    }
}


void freeList(List* L){
  process* p ;
  while (L->head != NULL){
    p = L->head;
    L -> head = L-> head->next;
    free(p);
    p = NULL;
  }
}

void freeQueue(Queue*L){
    process* p;
    while (L->start!= NULL){
      p = L->start;
      L -> start = L-> start->next;
      free(p);
      p = NULL;
    }
}

int processActive(Queue*Q,List*L, int timeQ, int currentTime){
    //맨 청므 노드 남아있는 시간
    int remain  =  Q->start->computing_time;
    process * temp =Q->start;
    
    //끝나는 시간
    int resultTime = 0;
    
    while(timeQ != 0){
        // 남아있는 시간이 더 크면 //노드는 맨뒤로 간다.
        if(remain > timeQ){
            temp->computing_time = remain - timeQ;
            resultTime = currentTime + timeQ;
            printProcess(Q);
            sendBack(Q);
            timeQ = 0;
        }
        //남아있는 시간이 똑같으면
        else if(remain == timeQ){
            resultTime = currentTime+timeQ;
            printProcess(Q);
            sendBack(Q);
            timeQ =0;
        }
        // 남아있는 시간이 부족할시 다음 프로세스 실행시
        else {
            timeQ -= remain;
            printProcess(Q);
            finishedProcess(Q, L, currentTime);
        }
    }
    printFinish(L);
    return resultTime;
}



void printProcess(Queue* Q){
    process* temp = Q->start;
    printf("Process id:");
    while(temp != NULL){
        printf(" %d,", temp->process_id);
        temp = temp->next;
    }
    printf("\n");
}
void printFinish(List* L){
    process* temp = L->head;
    printf("Finished id:");
    while(temp != NULL){
        printf(" %d,", temp->process_id);
        temp = temp->next;
    }
    printf("\n");
}

void finishedProcess(Queue * Q, List *L,int currentTime){
    process * temp = L->head;
    Q->start->endTime = currentTime;
    if(L->head == NULL){
        L->head = Q->start;
    }
    else{
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = Q->start;
        temp = temp->next;
    }
    Q->start =Q->start->next;
    temp->next = NULL;
}

//Use everytime Process is done
void sendBack(Queue *Q){
    //끝 다음께 Start
    Q->end->next= Q->start;
    //시작 교체
    Q->start = Q->start->next;
    //마지막 교체
    Q->end =Q->end->next;
    //마지막 다음것은 NULL
    Q->end = NULL;
}

