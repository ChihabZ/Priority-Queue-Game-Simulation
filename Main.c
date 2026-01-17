include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ===================== DATA STRUCTURES ===================== */

typedef struct Player {
int id;
char name[30];
int age;

int wins;
int losses;
int consecutiveWins;
int totalScore;

int part1Wins;
int part1Losses;
int part2Wins;
int part2Losses;
} Player;

/* ---------- Queue Node ---------- */

typedef struct QNode {
Player data;
struct QNode* next;
} QNode;

/* ---------- Queue ---------- */

typedef struct Queue {
QNode* front;
QNode* rear;
int size;
} Queue;

/* ---------- List Node ---------- */

typedef struct LNode {
Player data;
struct LNode* next;
} LNode;

/* ===================== QUEUE OPERATIONS ===================== */

void initQueue(Queue* q) {
q->front = q->rear = NULL;
q->size = 0;
}

int isQueueEmpty(Queue* q) {
return q->front == NULL;
}

void enqueue(Queue* q, Player p) {
QNode* n = (QNode*)malloc(sizeof(QNode));
n->data = p;
n->next = NULL;

if (isQueueEmpty(q)) {
q->front = q->rear = n;
} else {
q->rear->next = n;
q->rear = n;
}
q->size++;
}


Player dequeue(Queue* q) {
QNode* tmp = q->front;
Player p = tmp->data;

q->front = tmp->next;
if (q->front == NULL)
q->rear = NULL;

free(tmp);
q->size--;
return p;
}

/* ===================== LIST OPERATIONS ===================== */

void insertEndList(LNode** head, Player p) {
LNode* n = (LNode*)malloc(sizeof(LNode));
n->data = p;
n->next = NULL;

if (*head == NULL) {
*head = n;
return;
}

LNode* cur = *head;
while (cur->next != NULL)
cur = cur->next;
cur->next = n;
}

void insertSortedLG(LNode** head, Player p) {
LNode* n = (LNode*)malloc(sizeof(LNode));
n->data = p;

if (*head == NULL || (*head)->data.totalScore < p.totalScore) {
n->next = *head;
*head = n;
return;
}

LNode* cur = *head;
while (cur->next != NULL && cur->next->data.totalScore >= p.totalScore)
cur = cur->next;

n->next = cur->next;
cur->next = n;
}
/* ===================== UTILITY FUNCTIONS ===================== */

int sumDigits(int x) {
int s = 0;
while (x > 0) {
s += x % 10;
x /= 10;
}
return s;
}

int gcd(int a, int b) {
while (b != 0) {
int r = a % b;
a = b;
b = r;
}
return a;
}

int digitExists(int num, int digit) {
if (num == 0 && digit == 0) return 1;
while (num > 0) {
if (num % 10 == digit) return 1;
num /= 10;
}
return 0;
}

int gcdRuleSatisfied(int a, int b) {
int g = gcd(a, b);
int temp = g;
while (temp > 0) {
int d = temp % 10;
if (digitExists(a, d) || digitExists(b, d))
return 1;
temp /= 10;
}
return 0;
}
/* ===================== DISPLAY FUNCTIONS ===================== */

void displayQueue(char* name, Queue* q) {
printf("%s: ", name);
QNode* cur = q->front;
while (cur != NULL) {
printf("[%s(%d)] -> ", cur->data.name, cur->data.id);
cur = cur->next;
}
printf("NULL");
}

void displayList(char* name, LNode* head) {
printf("%s: ", name);
while (head != NULL) {
printf("[%s(%d), score=%d] -> ", head->data.name, head->data.id, head->data.totalScore);
head = head->next;
}
printf("NULL");
}

/* ===================== PLAYER SELECTION ===================== */

Player selectPlayer(Queue* F1, Queue* F, Queue* F3) {
if (!isQueueEmpty(F1)) return dequeue(F1);
if (!isQueueEmpty(F)) return dequeue(F);
return dequeue(F3);
}

/* ===================== PART I – ROUND ===================== */

void playRoundPart1(Player* p1, Player* p2) {
int s1 = 0, s2 = 0;
int turns = 0;

while (abs(s1 - s2) < 3 && turns < 12) {
int val1 = rand() % 900000 + 100000;
if (sumDigits(val1) % 5 == 0) s1++;
turns++;
if (abs(s1 - s2) >= 3 || turns >= 12) break;

int val2 = rand() % 900000 + 100000;
if (sumDigits(val2) % 5 == 0) s2++;
turns++;
}

p1->totalScore += s1;
p2->totalScore += s2;

if (s1 > s2) {
p1->wins++; p1->consecutiveWins++;
p2->losses++; p2->consecutiveWins = 0;
p1->part1Wins++; p2->part1Losses++;
} else if (s2 > s1) {
p2->wins++; p2->consecutiveWins++;
p1->losses++; p1->consecutiveWins = 0;
p2->part1Wins++; p1->part1Losses++;
}
}
/* ===================== PART II – ROUND ===================== */

void playRoundPart2(Player* p1, Player* p2) {
int s1 = 0, s2 = 0;
int turns = 0;

while (abs(s1 - s2) < 3 && turns < 16) {
int a = rand() % 900 + 100;
int b = rand() % 900 + 100;
if (gcdRuleSatisfied(a, b)) s1++;
turns++;
if (abs(s1 - s2) >= 3 || turns >= 16) break;

a = rand() % 900 + 100;
b = rand() % 900 + 100;
if (gcdRuleSatisfied(a, b)) s2++;
turns++;
}

p1->totalScore += s1;
p2->totalScore += s2;

if (s1 > s2) {
p1->wins++; p1->consecutiveWins++;
p2->losses++; p2->consecutiveWins = 0;
p1->part2Wins++; p2->part2Losses++;
} else if (s2 > s1) {
p2->wins++; p2->consecutiveWins++;
p1->losses++; p1->consecutiveWins = 0;
p2->part2Wins++; p1->part2Losses++;
}
}
/* ===================== MAIN PROGRAM ===================== */

int main() {
srand(time(NULL));

Queue F, F1, F3;
initQueue(&F); initQueue(&F1); initQueue(&F3);

LNode *LG = NULL, *LP = NULL;

int n = 6;
for (int i = 1; i <= n; i++) {
Player p = {i, "Player", 20+i, 0, 0, 0, 0, 0, 0, 0, 0};
sprintf(p.name, "P%d", i);
enqueue(&F, p);
}

int rounds = 0;

while (!isQueueEmpty(&F) || !isQueueEmpty(&F1) || !isQueueEmpty(&F3)) {
if ((F.size + F1.size + F3.size) < 2) break;

Player p1 = selectPlayer(&F1, &F, &F3);
Player p2 = selectPlayer(&F1, &F, &F3);

playRoundPart1(&p1, &p2);
rounds++;

if (p1.wins >= 5) insertSortedLG(&LG, p1);
else if (p1.losses >= 5) insertEndList(&LP, p1);
else if (p1.consecutiveWins >= 3) enqueue(&F1, p1);
else if (p1.losses >= 3) enqueue(&F3, p1);
else enqueue(&F, p1);

if (p2.wins >= 5) insertSortedLG(&LG, p2);
else if (p2.losses >= 5) insertEndList(&LP, p2);
else if (p2.consecutiveWins >= 3) enqueue(&F1, p2);
else if (p2.losses >= 3) enqueue(&F3, p2);
else enqueue(&F, p2);

printf("--- ROUND %d ---", rounds);
displayQueue("F1", &F1);
displayQueue("F", &F);
displayQueue("F3", &F3);
displayList("LG", LG);
displayList("LP", LP);
}

printf("=== GAME OVER ===");
displayList("Winners (LG)", LG);
displayList("Losers (LP)", LP);

return 0;
}
typedef struct QNode {
Player data;
struct QNode* next;
} QNode;
typedef struct Queue {
QNode* front;
QNode* rear;
int size;
} Queue;
typedef struct LNode {
Player data;
struct LNode* next;
} LNode;
