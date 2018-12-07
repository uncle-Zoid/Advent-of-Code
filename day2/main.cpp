#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *f = fopen("input.txt", "r");
    if(f == NULL)
    {
        return 1;
    }
    char buffer[32];

    int counter['z' - 'a' + 1] = {};

    int twos = 0;
    int tres = 0;

    size_t read = 0;
    size_t r = 0;
    do
    {
        read = 0;
        do
        {
            r = fread(buffer + read, 1, 1, f);
            read += r;
        }
        while(r > 0 && buffer[read-1] != '\n');

        if(buffer[read-1] == '\n')
        {
            --read;
        }

//        printf("r=%d read=%d ", r, read);
        if(read == 0)
        {
            break;
        }
        for (char *it = buffer; it != buffer + read; ++it)
        {
            ++counter[int(*it) - 'a'];
//            printf("%c", *it);
        }
        int tmptwos = 0, tmptres = 0;
        for(auto it = counter; it != (counter + sizeof(counter)/sizeof(int)); ++it)
        {
            if (*it == 3)
            {
                ++tmptres;
            }
            else if(*it == 2)
            {
                ++tmptwos;
            }

            *it = 0;
        }
//        printf("  tmptwos='%d' tmptres='%d'\n", tmptwos, tmptres);
        twos += (tmptwos > 0) ? 1 : 0;
        tres += (tmptres > 0) ? 1 : 0;
    }
    while(r > 0);

    printf("PART1=%d*%d=%d\n", twos, tres, twos * tres);

    return 0;
}
