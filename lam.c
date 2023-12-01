#include <stdio.h>

int max(int a, int b) {
    return (a > b) ? a : b;
}

void display(int e1, int e2, int p1[e1], int p2[e2]) {
    int i;

    printf("\nThe time stamps of events in P1:\n");
    for (i = 0; i < e1; i++)
        printf("%d ", p1[i]);

    printf("\nThe time stamps of events in P2:\n");
    for (i = 0; i < e2; i++)
        printf("%d ", p2[i]);
}

void lamport_logical_clock(int e1, int e2, int m[e1][e2]) {
    int i, j, k, p1[e1], p2[e2];

    for (i = 0; i < e1; i++)
        p1[i] = i + 1;

    for (i = 0; i < e2; i++)
        p2[i] = i + 1;

    for (i = 0; i < e1; i++) {
        for (j = 0; j < e2; j++) {
            if (m[i][j] == 1) {
                p2[j] = max(p2[j], p1[i] + 1);
                for (k = j + 1; k < e2; k++)
                    p2[k] = p2[k - 1] + 1;
            }
            if (m[i][j] == -1) {
                p1[i] = max(p1[i], p2[j] + 1);
                for (k = i + 1; k < e1; k++)
                    p1[k] = p1[k - 1] + 1;
            }
        }
    }

    display(e1, e2, p1, p2);
}

int main() {
    int e1 = 3, e2 = 2;
    int m[3][2] = { {-1, 0}, {0, 1}, {0, -1} };

    lamport_logical_clock(e1, e2, m);

    return 0;
}
