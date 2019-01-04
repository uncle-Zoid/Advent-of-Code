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
    struct
    {
        int x = 0;
        int y = 0;
    } initial, pos, velocity;

    void print() const
    {
//        printf("position [%5d, %5d] -> [%5d, %5d], velocity [%5d,%5d]\n", initial.x, initial.y, pos.x, pos.y, velocity.x, velocity.y);
        printf("position [%5d, %5d]\n", pos.x, pos.y);
    }

    void move()
    {
        pos.x += velocity.x;
        pos.y += velocity.y;
    }
};

int main()
{
    FILE *f = fopen("input.txt", "r");
//    FILE *f = fopen("testinput.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }

    std::vector<sPoint> points;

    char buffer[64];
    size_t read = 0;
    std::multiset<int> mapx, mapy;
    while((read = readline(f, buffer, sizeof(buffer)/sizeof(char))) > 0)
    {
        sPoint point;
        sscanf(buffer, "position=<%d,%d> velocity=<%d,%d>", &point.initial.x, &point.initial.y, &point.velocity.x, &point.velocity.y);
        point.pos = point.initial;
        points.push_back(point);
    }

    for (int i = 1; i < 5e4; ++i)
    {
        int minx = 1e9, miny = 1e9;
        int maxx = -1e9, maxy = -1e9;
        for (auto &a : points)
        {
            a.move();

            minx = std::min(minx, a.pos.x);
            maxx = std::max(maxx, a.pos.x);
            miny = std::min(miny, a.pos.y);
            maxy = std::max(maxy, a.pos.y);
        }
        if ((minx + 100) > maxx && (miny + 100) > maxy)
        {
            printf("sec: %d\n", i);
            printf("mixx = %d, maxx %d  miny=%d  maxy=%d\n", minx, maxx, miny, maxy);
            printf("w=%d, h=%d\n", maxx-minx, maxy-miny);
            QImage im(maxx-minx+1, maxy-miny+1, QImage::Format_ARGB32);
            im.fill(Qt::gray);
            for (auto &a : points)
            {
//                a.print();
                im.setPixelColor(a.pos.x - minx, a.pos.y - miny, Qt::red);
            }
            printf("\n\n");
            im.save(QString("img/obr%1.png").arg(i));
        }
    }

    return 0;
}
