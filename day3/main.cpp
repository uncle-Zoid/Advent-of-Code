#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <list>
#include <set>
#include <cstring>
#include <algorithm>
#include <numeric>

#include <QImage>

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
        if(id == other.id)
        {
            return {0,0};
        }
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
    void print(uint8_t *matrix, int sizex, int sizey) const
    {

        for(int i = tl.x; i != br.x; ++i)
        {
            for(int j = tl.y; j != br.y; ++j)
            {
                matrix[sizex * j + i] += 1;
            }
        }
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
    std::set<int> ids;

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
            auto size = makePoint(it2 + 1, static_cast<long long>(read) - (it2 - buffer) - 1, 'x');
            rect.construct(tl, size);

            ids.emplace(rect.id);
            //rect.print();

            rects.push_back(rect);
        }
    }

    for (auto &r1 : rects)
    {
        for (auto &r2 : rects)
        {
            if(r1.overlap(r2).area() > 0)
            {
                auto it = ids.find(r1.id);
                if(it != ids.end())
                {
                    ids.erase(it);
                }
                it = ids.find(r2.id);
                if(it != ids.end())
                {
                    ids.erase(it);
                }
            }
        }
    }

    const int W = 1000;
    const int H = 1000;
    uint8_t matrix[W*H] = {0};
    int sumarum = 0;
    for(auto &a : rects)
    {
        a.print(matrix, W, H);
    }

    QImage im(W,H,QImage::Format_ARGB32);

    for(size_t i = 0; i < sizeof(matrix)/sizeof (uint8_t); ++i)
    {
        int r = 0;
        if(matrix[i] == 1)
        {
            r = 80;
        }
        else if(matrix[i] > 1)
        {
            r = 80 + 20*matrix[i];
            matrix[i] = uint8_t(r > 255 ? 255 : r);
            ++ sumarum;
        }
//        std::cout << "x="<<(i % 10) << "  y=" << (i / 10) << std::endl;
        im.setPixelColor(int(i % W), int(i / W), QColor::fromRgb(r,0,0));
    }
    im.scaled(1000,1000).save("obrazek.png");

    printf("PART1: sumarum: %d\n", sumarum);
    printf("PART2: pocet=%d id=%d\n", ids.size(), *ids.begin());

    return 0;
}
