#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

int main()
{
    const int serialNumber = 1309;
    const int SIZE = 300;
    int matrix[SIZE][SIZE];
    char buffer[32];

    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            auto rackId = x + 10;
            int powerLevel = rackId * y;
            powerLevel += serialNumber;
            powerLevel *= rackId;
            if(powerLevel >= 100)
            {
                itoa(powerLevel, buffer, 10);
                powerLevel = buffer[strlen(buffer) - 3] - '0';
            }
            else
            {
                powerLevel = 0;
            }
            powerLevel -= 5;

            matrix[x][y] = powerLevel;
//            printf("%d ... %d\n", powerLevel, (powerLevel+5) * 255/19);
        }
    }

    int maxsum = -1e9;
    int cx = 0,cy = 0, sq = 1;
    for (int k = 1; k <= 20; ++k)
    {
        printf("k=%d\n", k);
        for (int y = k; y < SIZE - k; ++y)
        {
            for (int x = k; x < SIZE - k; ++x)
            {
                int sum = 0;
                for (int kk = 0; kk < k; ++kk)
                    for (int jj = 0; jj < k; ++jj)
                {
                    sum += matrix[x+jj][y+kk];

                    /*sum += matrix[jj][y-kk];
                    sum += matrix[x][y-kk];
                    sum += matrix[x+kk][y-kk];

                    sum += matrix[x-kk][y];
                    sum += matrix[x][y];
                    sum += matrix[x+kk][y];

                    sum += matrix[x-kk][y+kk];
                    sum += matrix[x][y+kk];
                    sum += matrix[x+kk][y+kk];*/
                }
                if(sum > maxsum)
                {
                    maxsum = sum;
                    cx = x;
                    cy = y;
                    sq = k;
                }
            }
        }
    }
    printf("maxsum %d, cx=%d, cy=%d, sq=%d\n", maxsum, cx, cy, sq);

    return 0;
}
