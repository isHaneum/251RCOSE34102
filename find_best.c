#include "find_best.h"

int argmin(const float arr[], int n) {
    int idx = 0;
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[idx]) idx = i;
    return idx;
}

int argmax(const float arr[], int n) {
    int idx = 0;
    for (int i = 1; i < n; i++)
        if (arr[i] > arr[idx]) idx = i;
    return idx;
}