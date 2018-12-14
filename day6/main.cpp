#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <vector>
#include <list>
#include <map>
#include <set>

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
    for(size_t i = 0; i < x; ++i)
    {
        for(size_t j = 0; j < y; ++j)
        {
            printf("%c", *((matrix + i*y) + j));
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
    FILE *f = fopen("testinput.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }

    std::vector<sPoint> points;

    const size_t SIZE = 10;
    char matrix[SIZE][SIZE];
    memset(matrix, '.', SIZE*SIZE);

    char buffer[64];
    size_t read = 0;
    int x,y;
    char counter = 'A';
    while((read = readline(f, buffer, sizeof(buffer)/sizeof(char))) > 0)
    {
        printf("%s\n", buffer);
        sscanf(buffer, "%d,%d", &x, &y);
        points.push_back({x,y, counter});
        matrix[y][x] = counter++;

    }

    print(SIZE, SIZE, (char*)matrix);


    for(int i = 0; i < SIZE; ++i)
    {
        for(int j = 0; j < SIZE; ++j)
        {
            if (isCoord(points, j, i))
                continue;

            std::multimap<int, const sPoint *> distances;
            int minDist = 1e9;
            const sPoint *point = nullptr;
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
                matrix[i][j] = tolower(it->second->ch);

//                printf("x=[%d], y=[%d] = %d\n", i,j, it->first);
            }
        }
    }

    print(SIZE, SIZE, (char*)matrix);


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


    int maxArea = 0;
    const sPoint *ppoint = nullptr;
    for(const auto &p : points)
    {
        std::list<sPoint> finder {p};
        int area = 0;
        bool infinite = false;
        while(finder.size() > 0)
        {
            auto &p = finder.front();

            std::list<sPoint> neighborhood;
            neighborhood.push_back(sPoint {p.x-1, p.y-1});
            neighborhood.push_back(sPoint {p.x  , p.y-1});
            neighborhood.push_back(sPoint {p.x+1, p.y-1});
            neighborhood.push_back(sPoint {p.x-1, p.y  });
            neighborhood.push_back(sPoint {p.x  , p.y+1});
            neighborhood.push_back(sPoint {p.x-1, p.y+1});
            neighborhood.push_back(sPoint {p.x  , p.y+1});
            neighborhood.push_back(sPoint {p.x+1, p.y+1});


            for(auto &n : neighborhood)
            {
                if(!isLimit(n, SIZE))
                {
                    infinite = true;
                    break;
                }
                if(tolower(matrix[n.y][n.x]) == tolower(p.ch))
                {
                    ++area;
                    finder.push_back(n);
                }
            }
            if(infinite)
            {
                break;
            }

            finder.pop_front();
        }
        printf("area %c = %d ... %s\n", p.ch, area, infinite ? "infinite" : "finite");
    }




//    printf("PART1: sumarum: %d\n", sumarum);
//    printf("PART2: pocet=%d id=%d\n", ids.size(), *ids.begin());

    return 0;
}
