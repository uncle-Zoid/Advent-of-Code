#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <algorithm>

int main(int argc, char *argv[])
{
    const int size = 16;
    char buffer[size]={0};
    int read = 0;

    size_t r = 0;
    int val;
    int sum = 0;
    std::set<int> twice;
    bool found = false;
    int i = 0;
    do
    {
        FILE *f = fopen("input.txt", "r");
        if(f == NULL)
        {
            return 1;
        }
        printf("round %d", i++);
        while((r = fread(buffer + read, 1, 1, f)) > 0)
        {
            read += r;
            if(read >= size)
            {
                return 2;
            }


            if(buffer[read-1] == '\n')
            {
                buffer[read] = 0;
                val = atoi(buffer);
                sum += val;

                found = false;
                auto it = twice.find(sum);

                if(it != twice.end())
                {
                    found = true;
                    break;
                }
                twice.insert(sum);

//                printf("%d \t...\t%d\n", val, sum);
                read = 0;
            }
        }
        fclose(f);
        printf(" posible matches: %d\n", twice.size());
    }while(found == false);

    printf("TOTAL %d\n", sum);
}
