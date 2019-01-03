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
    char id;
    char visited;
    char nodeCompleted;

    std::vector<sNode *> reqNext;
    std::vector<sNode *> reqPrev;

    sNode(char id)
        : id            (id)
        , visited       (false)
        , nodeCompleted (false)
    {}

};

sNode *find(sNode *root, char id);

sNode *findIn(sNode *root, char id)
{
//    if(root->reqNext.size() == 0)
//    {
//        return root->id id
//    }
    return find(root, id);
}

sNode *find(sNode *root, char id)
{
    if(root->id == id)
        return root;

    sNode *p = nullptr;
    for(const auto &a : root->reqNext)
    {
        if((p = find(a, id)) != nullptr)
        {
            return p;
        }
    }

    return p;
}

void add(sNode *&root, char id1, char id2)
{
    if(root == nullptr)
    {
        root = new sNode(id1);
    }

    sNode *f = nullptr;
    sNode *s = nullptr;
    f = find(root, id1);
    s = find(root, id2);

    printf("%c before %c\n", id1, id2);
    if(f)
    {
        if (s == nullptr)
            s = new sNode(id2);

        auto it = f->reqNext.begin();
        while ((it != f->reqNext.end()) && (id2 > (*it)->id))
        {
            ++it;
        }
        f->reqNext.insert(it, s);
//        f->reqNext.push_back(s);
        s->reqPrev.push_back(f);
    }
    else
    {
        f = new sNode(id1);
        f->reqNext.push_back(root);

        if(s == nullptr)
        {
            s = new sNode(id2);
            f->reqNext.push_back(s);
            s->reqPrev.push_back(f);
        }
        root->reqPrev.push_back(f);
        root = f;
    }



}

void print(sNode *root, sNode *from, std::string &res)
{
    if (!root->visited)
    {
        bool canMark = true;
        for(const auto &a : root->reqPrev)
        {
            if(a->visited == false)
            {
                canMark = false;
                break;
            }
        }
        root->visited = canMark;
        if(root->visited)
        {
            res.push_back(root->id);
        }
        std::cout << root->id << "  " << res << std::endl;
    }



    if(root->reqNext.size() > 0)
    {
        auto f = root->reqNext.front();
        root->reqNext.erase(root->reqNext.begin());
        print(f, root, res);
    }
    else if (from->reqPrev.size() > 0)
    {
//        print(from, from, res);
        print(from, from->reqPrev.front(), res);

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

    char buffer[64];
    size_t read = 0;
    char id1, id2;
    int i = 0;
    while((read = readline(f, buffer, sizeof(buffer)/sizeof(char))) > 0)
    {
        sscanf(buffer, "Step %c must be finished before step %c can begin.", &id1, &id2);
        add(root, id1, id2);
        if (++i > 26) break;
    }

    std::string res;
    print(root, root, res);
    std::cout << "\n\n\nres=" << res << std::endl;

    return 0;
}
