#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <list>
#include <cstring>
struct sPoint
{
    int x = 0;
    int y = 0;

    sPoint()
        : x (0)
        , y (0)
    {}
    sPoint(int xx, int yy)
        : x (xx)
        , y (yy)
    {}
    bool operator==(const sPoint &o)
    {
        return o.x == x && o.y == y;
    }
//    bool operator==(sPoint &o)
//    {
//        return o.x == x && o.y == y;
//    }
    int area() const
    {
        return x * y;
    }
    void print() const
    {
        printf("X=%d, Y=%d", x,y);
    }
};

struct sRect
{
    int id;

    sPoint tl;
    sPoint br;
    sPoint size;

    sRect()
        : id (0)
    {}

    bool operator==(const sRect &o)
    {
        return id == o.id || ((tl == o.tl) && (br == o.br) && (size == o.size));
    }

    sPoint overlap(const sRect &other)
    {
        auto interX = std::max(0, std::min(other.br.x, br.x) - std::max(other.tl.x, tl.x));
        auto interY = std::max(0, std::min(other.br.y, br.y) - std::max(other.tl.y, tl.y));

        return {interX, interY};
    }

    void construct(const sPoint &tl, const sPoint &size)
    {
        this->tl = tl;
        this->size = size;
        br.x = tl.x + size.x;
        br.y = tl.y + size.y;
    }

    void print() const
    {
        printf("ID = %d\n", id);
        printf("\tsize = ");
        size.print();
        printf("\n\tTL = ");
        tl.print();
        printf("\n\tBR = ");
        br.print();
        printf("\n");
    }
};

size_t readline(FILE *f, char buffer[], size_t size, char delim = '\n')
{
    size_t read = 0;
    size_t r    = 0;
    do
    {
        r    = fread(buffer + read, 1, 1, f);
        read += r;
    }
    while(read < size && r > 0 && buffer[read - 1] != delim);

    buffer[read] = '\0';
    return read;
}

int myatoi(char *buffer, long long size)
{
    int val = 0;
    for (char *it = buffer; it != buffer + size; ++it)
    {
        if(isblank(*it))
        {
            continue;
        }
        else if(!isdigit(*it))
        {
            break;
        }

        val = 10*val + (*it - '0');
    }
    return val;
}
sPoint makePoint(char *buffer, long long size, char delim)
{
    char *second = strchr(buffer, delim);
    sPoint p;
    p.x = myatoi(buffer    , second - buffer);
    p.y = myatoi(second + 1, size - (second - buffer) - 1);

    return p;
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }
    std::list<sRect> rects;

    char buffer[64];
    size_t read = 0;
//    #1 @ 1,3: 4x4
    while((read = readline(f, buffer, sizeof(buffer)/sizeof(char))) > 0)
    {
        if(buffer[0] == '#')
        {
            sRect rect;
            char *it = strchr(buffer, '@');
            char *it2 = strchr(it, ':');
            if(it == NULL)
            {
                printf("nenalezen @");
                return -1;
            }
            if(it2 == NULL)
            {
                printf("nenalezen :");
                return -1;
            }

            rect.id   = atoi(buffer+1);
            auto tl   = makePoint(it  + 1, it2-it, ',');
            auto size = makePoint(it2 + 1, read - (it2 - buffer) - 1, 'x');
            rect.construct(tl, size);

            //rect.print();

            rects.push_back(rect);
        }
    }

    int max = 0;
    for (auto &r1 : rects)
    {
        for (auto &r2 : rects)
        {
            if(r1 == r2)
            {
//                printf("same\n");
                continue;
            }
            auto res = r1.overlap(r2);

//            res.print();
            int i = res.x*res.y;
            if(i > max)
            {
                max = i;
                r1.print();
                r2.print();
                res.print();
                goto end;
            }
//            printf(" -> %d \n", i);
        }
//        printf("\n");
    }
end:
    printf("MAX overlap = %d", max);
    std::cout << std::endl;

    return 0;
}
