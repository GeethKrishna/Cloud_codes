#include <stdio.h>
#include <stdlib.h>

int main() {
    int ns, ncs, timestamp, site;
    printf("Enter number of sites: ");
    scanf("%d", &ns);
    printf("Enter number of sites attempting to enter the critical section: ");
    scanf("%d", &ncs);

    int ts[ns];
    int* request = malloc(ncs * sizeof(int));
    int* mp = calloc(ncs * 2, sizeof(int));  // Assuming a maximum of ncs distinct timestamps

    for (int i = 0; i < ns; i++) {
        ts[i] = 0;
    }

    for (int i = 0; i < ncs; i++) {
        printf("\nEnter timestamp for request %d: ", i + 1);
        scanf("%d", &timestamp);
        printf("Enter site number for request %d: ", i + 1);
        scanf("%d", &site);

        ts[site - 1] = timestamp;
        request[i] = site;
        mp[i * 2] = timestamp;
        mp[i * 2 + 1] = site;
    }

    printf("\nTimestamps for each site:\n");
    for (int i = 0; i < ns; i++) {
        printf("Site %d: %d\n", i + 1, ts[i]);
    }

    for (int i = 0; i < ncs; i++) {
        printf("\nResponse for request from Site %d:\n", request[i]);
        for (int j = 0; j < ns; j++) {
            if (request[i] != (j + 1)) {
                if (ts[j] > ts[request[i] - 1] || ts[j] == 0)
                    printf("Site %d: Replied\n", j + 1);
                else
                    printf("Site %d: Deferred\n", j + 1);
            }
        }
    }

    printf("\n");
    int c = 0;
    for (int i = 0; i < ncs * 2; i += 2) {
        if (mp[i] != 0) {
            printf("Site %d entered the Critical Section\n", mp[i + 1]);
            if (c == 0)
                printf("\nSimilarly,\n");
            c++;
        }
    }

    free(request);
    free(mp);
    return 0;
}
