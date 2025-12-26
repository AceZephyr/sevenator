#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

const int PERCENTS[] = { 50, 40, 30, 20, 10, -2, -5, -10 };
const int PERCENTS_MP[] = { 50, 40, 30, 20, 15, 10, 5, 2 };
#define NUM_PERCENTS 8

typedef struct StateQueue {
    struct StateQueue* next;
    int materiaCounts[NUM_PERCENTS];
    int percent;
    int equippedCount;
} StateQueue;

void sq_add(StateQueue** start, StateQueue** end, int percent, int equippedCount, int* materiaCounts)
{
    StateQueue* add = (StateQueue*) malloc(sizeof(StateQueue));
    add->percent = percent;
    add->equippedCount = equippedCount;
    for (int i = 0; i < NUM_PERCENTS; i++) {
        add->materiaCounts[i] = materiaCounts[i];
    }
    add->next = NULL;
    if ((*start) == NULL) {
        *start = add;
    } else {
        (*end)->next = add;
    }
    *end = add;
}

void sq_remove(StateQueue** start)
{
    StateQueue* oldStart = *start;
    *start = (*start)->next;
    free(oldStart);
}

int map_get(int* map, int percent)
{
    return map[percent + 100];
}

void map_set(int* map, int percent, int val)
{
    map[percent + 100] = val;
}

// BFS graph search to search paths to each percentage value
void possiblePercents(int* prevMap, int* matCt, int slotCt, int is_mp)
{
    int ctMap[201];
    StateQueue* qStart = NULL;
    StateQueue* qEnd = NULL;
    int newPct;
    int newEquippedCt;
    int newMatCt[NUM_PERCENTS];

    const int* percents = is_mp != 0 ? PERCENTS_MP : PERCENTS;

    for (int i = -100; i <= 100; i++)
        map_set(ctMap, i, INT_MAX);
    map_set(ctMap, 0, 0);

    sq_add(&qStart, &qEnd, 0, 0, matCt);

    while (qStart != NULL) {
        if (qStart->equippedCount < slotCt) {
            for (int i = 0; i < NUM_PERCENTS; i++) {
                if (qStart->materiaCounts[i] == 0)
                    continue;
                newPct = qStart->percent + percents[i];
                if (newPct > 100 || newPct < -100)
                    continue;
                newEquippedCt = qStart->equippedCount + 1;

                if (map_get(ctMap, newPct) > newEquippedCt) {
                    map_set(ctMap, newPct, newEquippedCt);
                    map_set(prevMap, newPct, qStart->percent);

                    if (slotCt > newEquippedCt) {
                        for (int j = 0; j < NUM_PERCENTS; j++)
                            newMatCt[j] = qStart->materiaCounts[j];
                        newMatCt[i]--;
                        sq_add(&qStart, &qEnd, newPct, newEquippedCt, newMatCt);
                    }
                }
            }
        }
        sq_remove(&qStart);
    }
}

size_t computeValidPercents(int* out, size_t outSize, int initHP) {
    int hp;
    size_t i = 0;
    for (int pct = -100; pct <= 100; pct++) {
        hp = initHP + (pct * initHP / 100);
        if (hp < 10) hp = 10;
        if (hp > 9999) hp = 9999;
        if (hp <= 7777 && hp % 100 == 77) {
            out[i++] = pct;
            printf("pct: %d %d\n", pct, out[i - 1]);
        }
        if (i >= outSize) return i;
    }
    return i;
}

size_t computeValidPercentsMP(int* out, size_t outSize, int initMP) {
    int mp;
    size_t i = 0;
    for (int pct = -100; pct <= 100; pct++) {
        mp = initMP + (pct * initMP / 100);
        if (mp < 10) mp = 10;
        if (mp > 999) mp = 999;
        if (mp % 100 == 77) {
            out[i++] = pct;
            printf("pct: %d %d\n", pct, out[i - 1]);
        }
        if (i >= outSize) return i;
    }
    return i;
}
