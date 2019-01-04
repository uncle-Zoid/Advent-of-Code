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
    int value = -1;

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

void countValue(sNode *root)
{
    if (root == nullptr)
    {
        return;
    }


    if(root->childs.size() == 0)
    {
        root->value = std::accumulate(root->metadata.begin(), root->metadata.end(), 0);
//        printf("node %d %d - %d\n", root->countChilds, root->countMetadata, root->value);
    }
    else
    {
        int sum = 0;
        for(int i : root->metadata)
        {
            i -= 1;
            if(i < root->childs.size())
            {
                auto it = root->childs.begin();
                while(i--)it++;

                if((*it)->value == -1)
                {
                    countValue(*it);
                }

                sum += (*it)->value;
            }
        }
        root->value = sum;
//        printf("node %d %d - %d\n", root->countChilds, root->countMetadata, root->value);
    }
}


int main()
{
//    FILE *f = fopen("testinput.txt", "r");
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

    printf("PART 1 : Suma: %d\n", sum);

    countValue(root);

    sum = 0;
    for(int i : root->metadata)
    {
        i -= 1;
        if(i < root->childs.size())
        {
            auto it = root->childs.begin();
            while(i--)it++;

            sum += (*it)->value;
//            printf("node %d %d - %d\n", (*it)->countChilds, (*it)->countMetadata, (*it)->value);
        }
    }
    printf("PART 2 : root node value %d\n", sum);
    return 0;
}
