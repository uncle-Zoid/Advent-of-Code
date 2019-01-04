#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>

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

void seekSpace(char *&it)
{
    do{++it;}while(*it != ' ');
}

struct sNode
{
    sNode *parent;
    int countChilds;
    int countMetadata;

    std::list<sNode *>::iterator it;
    std::list<sNode *> childs;
    std::list<int> metadata;

    sNode (sNode *parent)
        : parent        (parent)
        , countChilds   (0)
        , countMetadata (0)
    {
        it = childs.end();
    }

    sNode (sNode *parent, char *&it)
        : parent    (parent)
    {
        countChilds   = atoi(it); seekSpace(it);
        countMetadata = atoi(it); seekSpace(it);

        this->it = childs.end();
    }
};

sNode* addChild(sNode *&root, char *&it)
{
    if(root == nullptr)
    {
        root = new sNode(root, it);
    }
    else
    {
        if(root->countChilds == root->childs.size())
        {
            for (int i = 0; i < root->countMetadata; ++i)
            {
                root->metadata.push_back(atoi(it));
                seekSpace(it);
            }
            return root->parent;
        }
        else
        {
            auto *n = new sNode(root, it);
            root->childs.push_back(n);
            if(root->it == root->childs.end())
            {
                root->it = root->childs.begin();
            }
            return n;
        }
    }
    return root;
}


void sumTree(sNode *root, int &sum)
{
    if (root == nullptr)
    {
        return;
    }

    if(root->it == root->childs.begin())
    {
        sum += std::accumulate(root->metadata.begin(), root->metadata.end(), 0);
    }

    if(root->it == root->childs.end())
    {
        sumTree(root->parent, sum);
    }
    else
    {
        sumTree(*(root->it++), sum);
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
    sNode *root = nullptr;

    fseek(f, 0 ,SEEK_END);
    auto size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = new char[size + 1]();
    fread(buffer, 1, size, f);

//    printf("'%s'\n", buffer);


    auto *it = buffer;
    auto *tusom = root;
    //2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2
//    while (it != (buffer + size))
    do
    {
        tusom = addChild(tusom, it);
        if(root == nullptr)
        {
            root = tusom;
        }
    }
    while(tusom);

    int sum = 0;
    sumTree(root, sum);

    printf("Suma: %d\n", sum);

    return 0;
}
