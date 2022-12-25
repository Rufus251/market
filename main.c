#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <unistd.h>
#include <locale.h>
#include <windows.h>
#include <time.h>
typedef struct customer{
    char name;
    int time, check;
}customer;

typedef struct queue_element{
    customer customer;
    struct queue_element *next_element;
}queue_element;

typedef struct queue{
    int len;
    queue_element *first, *last;
}queue;

typedef struct cashier{
    int count, check_sum;
    char flag;
    queue queue;
}cashier;

// �������.......................................................
int rand_int(int l, int r){
    int *b = (int*)malloc(1);
    srand(*b + rand() + rand()%1636 + rand() % 2 + rand()% 10000 + rand()%938679183746 * rand());
    free(b);
    return (rand() % (r - l + 1) + l);
} // ��������� �����
queue *create_queue(int max_time, int max_check, int count){
    queue *waiters_queue = (queue*)malloc(sizeof(queue));
    waiters_queue->len = 0;
    waiters_queue->first = NULL;
    waiters_queue->last = NULL;
    queue_element *iterator;
    for (int i = 0; i < count; i++){
        queue_element *a = (queue_element*)malloc(sizeof(queue_element));
        customer cus = {rand_int(97, 122), rand_int(1, max_time), rand_int(1, max_check)};
        a->customer = cus;
        a->next_element = NULL;

        if (i == 0 && count != 0){
            waiters_queue->first = a;
            waiters_queue->len = i + 1;
        }
        else if (i == count - 1){
            waiters_queue->last = a;
            waiters_queue->len = i + 1;
            iterator->next_element = a;
        }
        else{
            iterator->next_element = a;
        }

        iterator = a;
    }
    return waiters_queue;
} // �������� ������� ��������
int print_queue(queue *queue){
    queue_element *iterator = queue->first;
    if (iterator == NULL){
        printf("\n");
        return 0;
    }
    while (iterator->next_element != NULL){
        printf("%c%d ", iterator->customer.name, iterator->customer.time);
        iterator = iterator->next_element;
    }
    printf("%c%d\n", iterator->customer.name, iterator->customer.time);
} // ������ �������
void insert_element(queue *queue, queue_element *insert, int pozition){
    queue_element *iterator;
    if (pozition == 0){
        queue->first = insert;
        queue->last = insert;
        queue->len++;
    }
    else if (pozition == queue->len){
        iterator = queue->first;
        for (int i = 0; i < pozition - 1; i++) {
            iterator = iterator->next_element;
        }
        iterator->next_element = insert;
        queue->len++;
    }
    else {
        iterator = queue->first;
        for (int i = 0; i < pozition - 1; i++) {
            iterator = iterator->next_element;
        }
        queue_element *a = iterator->next_element;
        iterator->next_element = insert;
        insert->next_element = a;
        queue->len++;
    }
} // ������� � ������� ������
void delete_element(queue *queue, int pozition){
    queue_element *iterator;
    if (pozition == 0){
        if (queue->len == 1) {
            queue->len--;
            queue_element *a = queue->first;
            queue->first = NULL;
            queue->last = NULL;
            a = NULL;
            free(a);
        }
        else{
            queue->len--;
            queue_element *a = queue->first;
            queue->first = queue->first->next_element;
            a = NULL;
            free(a);
        }
    }
    else if (pozition == queue->len - 1){
        queue->len--;
        iterator = queue->first;
        for (int i = 0; i < pozition - 2; i++) {
            iterator = iterator->next_element;
        }
        queue->last = iterator;
        queue_element *a = iterator->next_element;
        iterator->next_element = NULL;
        a = NULL;
        free(a);
    }
    else {
        queue->len--;
        iterator = queue->first;
        for (int i = 0; i < pozition - 1; i++) {
            iterator = iterator->next_element;
        }
        queue_element *a = iterator->next_element;
        iterator->next_element = iterator->next_element->next_element;
        a = NULL;
        free(a);
    }
} // �������� �������� �� ������

int main(void) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    srand(time(NULL));

    // ������ � �����������, ����������� ������� ���� � ��������
    int set[5], count = 0, time = 0;

    // ������ �������� �� �����
    FILE *fin = fopen("settings.txt", "rt");
    char str[100];
    for (int i = 0; i < 10; i++) {
        fscanf(fin, "%s", str);
        if (i % 2 == 1) {
            set[count] = atoi(str);
            count++;
        }
    }

    int MAX_CUSTOMER_TIME = set[0]; // ������������ ����� ����������
    int MAX_CASHIER_QUEUE = set[1]; // ������������ ������� � �����
    int MAX_CASHIERS = set[2]; // ������������ ���������� ����
    int MAX_NEXT_CUSTOMERS = set[3]; // ������������ ���������� ����� �����������
    int MAX_CUSTOMER_CHECK = set[4]; // ������������ ��� �����������
    count = 0;
    fclose(fin);

    // ������ ���� ����
    cashier **all_cashiers = (cashier**)malloc(sizeof(cashier*)*MAX_CASHIERS);
    for (int i = 0; i < MAX_CASHIERS; i++){
        all_cashiers[i] = (cashier*)malloc(sizeof(cashier));
        all_cashiers[i]->check_sum = 0;
        all_cashiers[i]->flag = '-';
        all_cashiers[i]->count = 0;
        all_cashiers[i]->queue = *create_queue(0, 0, 0);
    }

    // ������ ���������� ��� ������, ������ ��� ����� �����
    queue_element **iterators = (queue_element**)malloc(sizeof(queue_element*)*MAX_CASHIERS);
    for (int i = 0; i < MAX_CASHIERS; i++){
        iterators[i] = all_cashiers[i]->queue.first;
    }

    // �����
    int customers_sum = 0, customers_gone = 0, check_sum = 0;
    while (TRUE) {
        printf("Remi queue model.");

        // ������ ��������� �������
        queue *waiters_queue = create_queue(MAX_CUSTOMER_TIME, MAX_CUSTOMER_CHECK, rand_int(0, MAX_NEXT_CUSTOMERS));
        for (int i = 0; i < MAX_CASHIERS; i++){
            iterators[i] = all_cashiers[i]->queue.first;
        }

        // ����� � �������
        for (int i = 0; i < MAX_CASHIER_QUEUE + 2; i++) {
            printf("\n");
            for (int j = 0; j < MAX_CASHIERS; j++) {
                if (i == 0) {
                    printf("%d\t", j + 1);
                    continue;
                }
                if (i == 1) {
                    printf("%c\t", all_cashiers[j]->flag);
                    continue;
                }
                if (all_cashiers[j]->queue.len > i - 2){
                    printf("%c%d\t", iterators[j]->customer.name, iterators[j]->customer.time);
                    if (iterators[j]->next_element != 0){
                        iterators[j] = iterators[j]->next_element;
                    }
                }
                else{
                    printf("||\t");
                }
            }
        }
        printf("\nTime: %d\n", time);
        printf("Next customers: ");
        print_queue(waiters_queue);
        printf("Customers now: %d\n", customers_sum);
        printf("Working cashiers %d from %d: \n", count, MAX_CASHIERS);
        printf("Customers gone: %d\n", customers_gone);
        printf("Check sum: %d\n", check_sum);
        printf("Max queue: %d\n", MAX_CASHIER_QUEUE);

        // ������ �� ���� ������
        for (int i = 0; i < MAX_CASHIERS; i++){

            // ��������� �� ������� �������� �����
            if (all_cashiers[i]->queue.first != NULL) {
                all_cashiers[i]->queue.first->customer.time--;

                // ���� ����� ������� == 0
                if (all_cashiers[i]->queue.first->customer.time == 0) {

                    // ��������
                    check_sum += all_cashiers[i]->queue.first->customer.check;
                    customers_gone++;
                    customers_sum--;
                    all_cashiers[i]->count += all_cashiers[i]->queue.first->customer.check;

                    // �������� ������� ��������
                    delete_element(&all_cashiers[i]->queue, 0);

                    // ���� ������� �������, ������� ����
                    if (all_cashiers[i]->queue.first == NULL){
                        all_cashiers[i]->queue.last = NULL;
                        all_cashiers[i]->flag = '-';
                        count--;
                    }
                }
            }

            // �������� �� ������������� ������� �����
            if (all_cashiers[i]->queue.len == MAX_CASHIER_QUEUE){
                continue;
            }

            // ���������� ������� ����� �� ������� ��������

            // �������� ���������� �������� ������� �����
            queue_element *iter_wait;
            iter_wait = all_cashiers[i]->queue.last;

            // ���� ������� �������
            if (iter_wait == NULL && waiters_queue->len != 0){

                // ��������
                all_cashiers[i]->flag = '+';
                count++;
                customers_sum++;

                // ���������� ������� �������� ������� �����
                insert_element(&all_cashiers[i]->queue, waiters_queue->first, 0);
                delete_element(waiters_queue, 0);
            }

            // ���������� ��������� ���������� � ������� ��������
            while (waiters_queue->len != 0 && all_cashiers[i]->queue.len != MAX_CASHIER_QUEUE){

                // ��������
                customers_sum++;

                // ���������� � ����� ������� ����� �� ������� ��������
                insert_element(&all_cashiers[i]->queue, waiters_queue->first, all_cashiers[i]->queue.len);
                delete_element(waiters_queue, 0);
            }
        }

        // ���� ����� ���������, � ������� �������� �� �������
        if (waiters_queue->len > 0){
            break;
        }

        sleep(1);
        time++;
        system("cls");
    }
    system("cls");
    printf("Game Over! Your time is: %d", time);
    sleep(10);
}