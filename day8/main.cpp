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

struct sNode
{
    int countChilds;
    int countMetadata;

    std::list<sNode *> childs;
    std::list<int> metadata;

};

void addChild(sNode *&root, int count)
{
    while(count)
    {
        root->childs.emplace({});
        -- count;
    }
}



int main()
{
    FILE *f = fopen("testinput.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }
    sNode *root = nullptr;

    fseek(f, 0 ,SEEK_END);
    auto size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = new char[size + 1]();
    fread(buffer, 1, size, f);

    printf("'%s'\n", buffer);


    auto *it = buffer;
    auto *tusom = root;
    while (it != (buffer + size))
    {
        sNode *node = new sNode;
        node->countChilds = atoi(it); it += 2;
        node->countMetadata = atoi(it); it += 2;

        if(node->countChilds == 0)
        {
            for (int i = 0; i < node->countMetadata; ++i)
            {
                node->metadata.push_back(atoi(it));
                it += 2;
            }
        }

        if(root == nullptr)
        {
            root = node;
        }
        else
        {

        }
    }


    return 0;
}
