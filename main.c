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

// Функции.......................................................
int rand_int(int l, int r){
    int *b = (int*)malloc(1);
    srand(*b + rand() + rand()%1636 + rand() % 2 + rand()% 10000 + rand()%938679183746 * rand());
    free(b);
    return (rand() % (r - l + 1) + l);
} // Рандомное число
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
} // Создание очереди ожидания
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
} // Печать очереди
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
} // Вставка в элемент списка
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
} // Удаления элемента из списка

int main(void) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");
    srand(time(NULL));

    // Массив с настройками, количеством рабочих касс и временем
    int set[5], count = 0, time = 0;

    // Чтение настроек из файла
    FILE *fin = fopen("settings.txt", "rt");
    char str[100];
    for (int i = 0; i < 10; i++) {
        fscanf(fin, "%s", str);
        if (i % 2 == 1) {
            set[count] = atoi(str);
            count++;
        }
    }

    int MAX_CUSTOMER_TIME = set[0]; // Максимальное время покупателя
    int MAX_CASHIER_QUEUE = set[1]; // Максимальная очередь у КАССЫ
    int MAX_CASHIERS = set[2]; // Максимальное количество касс
    int MAX_NEXT_CUSTOMERS = set[3]; // Максимальное количество новых посетителей
    int MAX_CUSTOMER_CHECK = set[4]; // Максимальный чек покупателей
    count = 0;
    fclose(fin);

    // Массив всех касс
    cashier **all_cashiers = (cashier**)malloc(sizeof(cashier*)*MAX_CASHIERS);
    for (int i = 0; i < MAX_CASHIERS; i++){
        all_cashiers[i] = (cashier*)malloc(sizeof(cashier));
        all_cashiers[i]->check_sum = 0;
        all_cashiers[i]->flag = '-';
        all_cashiers[i]->count = 0;
        all_cashiers[i]->queue = *create_queue(0, 0, 0);
    }

    // Массив итераторов для вывода, каждый для своей кассы
    queue_element **iterators = (queue_element**)malloc(sizeof(queue_element*)*MAX_CASHIERS);
    for (int i = 0; i < MAX_CASHIERS; i++){
        iterators[i] = all_cashiers[i]->queue.first;
    }

    // Вывод
    int customers_sum = 0, customers_gone = 0, check_sum = 0;
    while (TRUE) {
        printf("Remi queue model.");

        // Массив ожидающей очереди
        queue *waiters_queue = create_queue(MAX_CUSTOMER_TIME, MAX_CUSTOMER_CHECK, rand_int(0, MAX_NEXT_CUSTOMERS));
        for (int i = 0; i < MAX_CASHIERS; i++){
            iterators[i] = all_cashiers[i]->queue.first;
        }

        // Вывод в консоль
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

        // Пробег по всем кассам
        for (int i = 0; i < MAX_CASHIERS; i++){

            // Вычитание из первого элемента кассы
            if (all_cashiers[i]->queue.first != NULL) {
                all_cashiers[i]->queue.first->customer.time--;

                // Если время первого == 0
                if (all_cashiers[i]->queue.first->customer.time == 0) {

                    // Счётчики
                    check_sum += all_cashiers[i]->queue.first->customer.check;
                    customers_gone++;
                    customers_sum--;
                    all_cashiers[i]->count += all_cashiers[i]->queue.first->customer.check;

                    // Удаление первого элемента
                    delete_element(&all_cashiers[i]->queue, 0);

                    // Если очередь нулевая, сменить флаг
                    if (all_cashiers[i]->queue.first == NULL){
                        all_cashiers[i]->queue.last = NULL;
                        all_cashiers[i]->flag = '-';
                        count--;
                    }
                }
            }

            // Проверка на заполненность очереди кассы
            if (all_cashiers[i]->queue.len == MAX_CASHIER_QUEUE){
                continue;
            }

            // Заполнение очереди кассы из очереди ожидания

            // Итератор последнего элемента очереди кассы
            queue_element *iter_wait;
            iter_wait = all_cashiers[i]->queue.last;

            // Если очередь нулевая
            if (iter_wait == NULL && waiters_queue->len != 0){

                // Счётчики
                all_cashiers[i]->flag = '+';
                count++;
                customers_sum++;

                // Добавление первого элемента очереди кассы
                insert_element(&all_cashiers[i]->queue, waiters_queue->first, 0);
                delete_element(waiters_queue, 0);
            }

            // Добавление элементов оставшихся в очереди ожидания
            while (waiters_queue->len != 0 && all_cashiers[i]->queue.len != MAX_CASHIER_QUEUE){

                // Счётчики
                customers_sum++;

                // Добавление в конец очереди кассы из очереди ожидания
                insert_element(&all_cashiers[i]->queue, waiters_queue->first, all_cashiers[i]->queue.len);
                delete_element(waiters_queue, 0);
            }
        }

        // Если кассы заполенны, а очередь ожидания не нулевая
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