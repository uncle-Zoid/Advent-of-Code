#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstring>


int doPolymorf(char *polymorf, const long size)
{
    for(char *it = polymorf; it != (polymorf + size - 1); )
    {
        char *nextone = it + 1;
        while(it != (polymorf + size - 1) && *nextone == '.')++nextone;

//        printf("%c %c\n", *it, *nextone);
        if(tolower(*nextone) == tolower(*it))
        {
            if((isupper(*nextone) && islower(*it)) || (isupper(*it) && islower(*nextone)))
            {
                *nextone = '.';
                *it = '.';

                while(it != polymorf && *--it == '.');
                continue;
            }
        }
        while(it != (polymorf + size - 1) && *++it == '.');
    }

    int remains = 0;
    for(char *it = polymorf; it != (polymorf + size); ++it)
    {
        if(*it != '.')
        {
//            printf("%c", *it);
            ++remains;
        }

    }
    return remains;
}
void print(char *polymorf, const long size)
{
    for(char *it = polymorf; it != (polymorf + size); ++it)
    {
        if(*it != '.')
        {
            printf("%c", *it);
        }
    }
    printf("\n\n");
}
void removeUnit(char *polymorf, const long size, char unit)
{
    unit = tolower(unit);
    for(char *it = polymorf; it != (polymorf + size); ++it)
    {
        if(tolower(*it) == unit)
        {
            *it = '.';
        }
    }
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }


    fseek(f, 0, SEEK_END);
    const auto size = ftell(f);
    rewind(f);


    char *polymorf = (char*)calloc(size+1, sizeof(char));
    fread(polymorf, 1, size_t(size), f);

//    printf("%s\n", polymorf);
    char *polymorf2 = (char*)calloc(size+1, sizeof(char));
    memcpy(polymorf2, polymorf, size_t(size));

    int remains = doPolymorf(polymorf2, size);
//    print(polymorf2, size);
    printf("Part1: Result: %d \n", remains);

    int minlen = 1e6;
    char minelement;

    for(char i = 'a'; i < 'z'; ++i)
    {
        char *polymorf2 = (char*)calloc(size+1, sizeof(char));
        memcpy(polymorf2, polymorf, size_t(size));

        removeUnit(polymorf2, size, i);

        int res = doPolymorf(polymorf2, size);
//        print(polymorf2, size);
        if(res < minlen)
        {
            minlen = res;
            minelement = i;
        }
        free(polymorf2);
    }


    printf("PART2: removed unit=%c - final size %d\n", minelement, minlen);


    free(polymorf2);
    free(polymorf);


//    printf("PART1: %d most sleep in %d (%d) => %d\n", maxsleptid, mostsleepminute+1, mostsleepminuteCount, maxsleptid * (mostsleepminute+1));

    return 0;
}
