#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <fstream>

#include <QImage>

using namespace std;

struct sRule
{
    vector<char> rules;
    char result;

    sRule(char *data, size_t size)
    {
        if(size < 6)
            return;

        for (int i = 0; i < 5; ++i)
        {
            rules.push_back(data[i]);
        }

        auto *it = data + size - 1;
        while(isspace(*it)) --it;
        result = *it;
    }

    bool applyRule(list<char> &pots, list<char>::iterator it)
    {
        auto frombeg = std::distance(pots.begin(), it);
        auto fromend = std::distance(pots.end(), it);
        if(frombeg < 2 || fromend < 3)
        {
            return false;
        }
        --it;
        --it;
        for(auto r : rules)
        {
            if(r != *it)
            {
                return false;
            }
            ++it;
        }
        return true;
    }
};
static int64_t zero = 0;
static int64_t growStep = 0;


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

void print(list<char> &pots)
{
//    ofstream f("file", ios::app);
    int64_t suma = 0;
    for (auto it = pots.begin(); it != pots.end(); ++it)
    {
//        printf("%c ", *it);
//        f << *it << " ";
        if(*it == '#')
        {
            auto d = distance(pots.begin(), it);
//            printf("[%d %d]", d, d-zero);
            suma += d - zero;
        }
    }
//    f << "\n";
//    f.close();
    printf("\tzero=%-15lld\tsum=%-15lld, size=%-5lld growstrep=%-5lld\n", zero, suma, pots.size(), growStep);
}

void grow(list<char> &pots)
{

    auto itbeg = pots.begin();
    auto itend = pots.rbegin();
    growStep = -3;
    while(*itbeg == '.')
    {
        itbeg = pots.erase(itbeg);
        -- zero;
        ++ growStep;
    }

//    while(*itend++ == '.') ++em;
//    {
//        std::advance(itend, 1);
//        pots.erase( itend.base() );
//    }
    int em = 0;
    while(*itend++ == '.') ++em;
    if(em < 3)
    {
        for(int i = 0; i < 3 - em; ++i)
        {
            pots.push_back('.');
        }
    }
    for(int i = 0; i < 3; ++i)
    {
        pots.push_front('.');
//        pots.push_back('.');
    }
    zero += 3;
}

void applyRule(list<char> &pots, list<sRule> &rules, list<char> &outpots)
{
    for(auto it = pots.begin(); it != pots.end(); ++it)
    {
//        printf("pot:%d  %c\t", distance(pots.begin(), it), *it);
        bool match = false;
        for(auto &r : rules)
        {
            if((match=r.applyRule(pots, it)) == true)
            {
//                printf("match rule ");
//                for(auto rr : r.rules) printf("%c", rr);
//                printf("\n");
                outpots.push_back(r.result); //*it = r.result;
                break;
            }
        }
        if(!match)
        {
            outpots.push_back('.');
        }
    }
}
bool comparer(list<char> &potsold, list<char> &potsnew)
{
    if(potsold.size() != potsnew.size())
    {
        return false;
    }

    auto ito = potsold.begin(), itn = potsnew.begin();

    while(*ito++ == '.');
    while(*itn++ == '.');

    for(; ito != potsold.end() || itn != potsnew.end() ; ++itn, ++ito)
    {
        if(*ito != *itn)
            return false;
    }
    return true;
}
int main()
{
//    ofstream ff("file");
//    ff.close();
//    FILE *f = fopen("testinput.txt", "r");
    FILE *f = fopen("input.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }


    list<sRule> rules;
    list<char> pots;


    char buffer[128];
    size_t read = 0;
    read = readline(f, buffer, sizeof(buffer)/sizeof(char));

    auto it = buffer + 15;
    while(!isspace(*it))
    {
        pots.push_back(*it);
        ++it;
    }

    while((read = readline(f, buffer, sizeof(buffer)/sizeof(char))) > 0)
    {
        auto r = sRule(buffer, read);
        if(r.rules.size() > 0)
        {
            rules.push_back(r);
        }
    }

    grow(pots);
    print(pots);
    uint64_t MAX = 50000000000;
    list<char> old;
    for(uint64_t i = 1; i < MAX; ++i)
    {
        list<char> newgenpots;

        grow(pots);
        applyRule(pots, rules, newgenpots);
        pots = newgenpots;

        printf("%d: ", i);
        print(pots);


        if(comparer(pots, old) && i > 10)
        {
            zero -= (MAX - i) * growStep;
            print(pots);
            break;
        }
        old = pots;
    }

    return 0;
}
