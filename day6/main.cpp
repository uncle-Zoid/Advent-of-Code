#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <vector>
#include <list>
#include <map>
#include <set>

#include <QImage>

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

struct sPoint
{
    int x;
    int y;
    char ch;

    int manhatan(const sPoint &p) const
    {
        return abs(x - p.x) + abs(y - p.y);
    }

    bool operator==(const sPoint &p) const
    {
        return x==p.x && y==p.y;
    }
};

struct sList
{
    sList *next = nullptr;
    sPoint value;

    sList* add(sList *prev, const sPoint &p)
    {
        if(prev)
        {
            next = (sList*)calloc(1, sizeof(sList));
            next->value = p;
            return next;
        }
        else
        {
            value = p;
            return this;
        }
    }
};

void print(const size_t x, const size_t y, char *matrix)
{
    return;
    for(size_t i = 0; i < x; ++i)
    {
        for(size_t j = 0; j < y; ++j)
        {
            printf("%d", *((matrix + i*y) + j));
        }
        printf("\n");
    }
    printf("\n");
}
bool isCoord(std::vector<sPoint> &v, int x, int y)
{
    for(auto &a : v)
    {
        if(a == sPoint{x,y})
        {
            return true;
        }
    }
    return false;
}
bool isLimit(const sPoint &point, int max)
{
    return (point.x >= 0 && point.x < max) && (point.y >=0 && point.y < max);
}

int main()
{
    FILE *f = fopen("input.txt", "r");
    const size_t SIZE = 400;
//    FILE *f = fopen("testinput.txt", "r");
//    const size_t SIZE = 10;//400;
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }

    std::vector<sPoint> points;

    char matrix[SIZE][SIZE];
    memset(matrix, '\0', SIZE*SIZE);

    char buffer[64];
    size_t read = 0;
    int x,y;
    char counter = 1;//'A';
    while((read = readline(f, buffer, sizeof(buffer)/sizeof(char))) > 0)
    {
        printf("%s\n", buffer);
        sscanf(buffer, "%d,%d", &x, &y);
        points.push_back({x,y, counter});
        matrix[y][x] = counter++;

    }

    print(SIZE, SIZE, (char*)matrix);

    QImage im(SIZE,SIZE,QImage::Format_ARGB32);
    im.fill(Qt::gray);


    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            if (isCoord(points, j, i))
                continue;

            std::multimap<int, const sPoint *> distances;
            for(const auto &p : points)
            {
                int d= p.manhatan({j,i});
                distances.insert(std::make_pair(d, &p));
            }

            int min = 1e9;
            for(auto &a : distances)
            {
                if(a.first < min)
                {
                    min = a.first;
                }
            }
            if(distances.count(min) <= 1)
            {
                auto it = distances.find(min);
                matrix[i][j] = it->second->ch;

                int r = 80+10*it->second->ch;
                int g = 10*it->second->ch;
                int b = 180+10*it->second->ch;
                im.setPixelColor(j, i, QColor::fromRgb(r % 255, g % 255, b % 255));
//                printf("x=[%d], y=[%d] = %d\n", i,j, it->first);
            }
        }
    }
//intptr_t a;
    print(SIZE, SIZE, (char*)matrix);

    im.scaled(1000,1000).save("obrazek_rozdeleni.png");

    /*
aaaaa.cccc
aAaaa.cccc
aaaddecccc
aadddeccCc
..dDdeeccc
bb.deEeecc
bBb.eeee..
bbb.eeefff
bbb.eeffff
bbb.ffffFf
*/

//    char matrix[SIZE][SIZE];
    char *mmatrixx = new char[SIZE*SIZE];
    memcpy(mmatrixx, matrix, SIZE*SIZE);
    print(SIZE, SIZE, mmatrixx);

    int maxArea = 0;
    const sPoint *ppoint = nullptr;

    for(const auto &p : points)
    {
        std::list<sPoint> finder {p};
        int area = 0;
        bool infinite = false;
        while(finder.size() > 0)
        {
            auto poi = finder.front();

            std::list<sPoint> neighborhood;
            neighborhood.push_back(sPoint {poi.x-1, poi.y-1});
            neighborhood.push_back(sPoint {poi.x  , poi.y-1});
            neighborhood.push_back(sPoint {poi.x+1, poi.y-1});
            neighborhood.push_back(sPoint {poi.x-1, poi.y  });
            neighborhood.push_back(sPoint {poi.x+1, poi.y  });
            neighborhood.push_back(sPoint {poi.x-1, poi.y+1});
            neighborhood.push_back(sPoint {poi.x  , poi.y+1});
            neighborhood.push_back(sPoint {poi.x+1, poi.y+1});

            finder.pop_front();

            for(auto &n : neighborhood)
            {
                char chachar = *(mmatrixx + int(SIZE) * n.y +n.x);
                if(!isLimit(n, SIZE))
                {
                    infinite = true;
                    break;
                }
                else if(tolower(chachar) == tolower(p.ch))
                {
//                    printf("x=%d y=%d    %c\n", n.x, n.y, *(mmatrixx + int(SIZE) * n.y +n.x));
//                    print(SIZE, SIZE, mmatrixx);
                    *(mmatrixx + int(SIZE) * n.y +n.x) = 0/*'#'*/;
                    ++area;
                    finder.push_back(n);
                }
            }
            if(infinite)
            {
                break;
            }

        }
//        print(SIZE, SIZE, mmatrixx);
        printf("area %d = %d ... %s\n", p.ch, area, infinite ? "infinite" : "finite");
        if(!infinite)
        {
            if(area > maxArea)
            {
                maxArea = area;
                ppoint = &p;
            }
        }
    }




    printf("PART1: %d plocha %d\n", ppoint->ch, maxArea);

    printf("\n\n\n");

    int sizeRegion = 0;
    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            int sum = 0;
            for(const auto &p : points)
            {
                sum += p.manhatan({j,i});
            }
            if(sum < 10000)
            {
                matrix[i][j] = 255;
                im.setPixelColor(j,i, Qt::magenta);
                ++sizeRegion;
            }
        }
    }

    im.scaled(1000,1000).save("obrazek_region.png");
    print(SIZE, SIZE, (char*)matrix);

    printf("PART2: size region=%d \n", sizeRegion);


    return 0;
}
