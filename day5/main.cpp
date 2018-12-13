#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

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


    char *polymorf = new char[size+1]();
    fread(polymorf, 1, size_t(size), f);

//    printf("%s\n", polymorf);

    for(char *it = polymorf; it != (polymorf + size - 1); )
    {
        char *nextone = it + 1;
        while(*nextone == '.')++nextone;

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
        while(*++it == '.');
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

    printf("Part1: Result: %d \n", remains);
    delete [] polymorf;


//    printf("PART1: %d most sleep in %d (%d) => %d\n", maxsleptid, mostsleepminute+1, mostsleepminuteCount, maxsleptid * (mostsleepminute+1));
//    printf("PART2: pocet=%d id=%d\n", ids.size(), *ids.begin());

    return 0;
}
