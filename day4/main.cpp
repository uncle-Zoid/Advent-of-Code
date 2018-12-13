#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <list>
#include <set>
#include <cstring>
#include <algorithm>
#include <numeric>
#include <map>

int daysToMonth[2][12] =
{
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 },
    { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 },
};
bool isLeapYear( int year )
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}
int dayOfYear(int year, int month, int day)
{
    return daysToMonth[isLeapYear(year)][month-1] + day;
}

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

enum EAction {UNKNOWN, START, SLEEP, WAKE};
static const char *TEXTS[] {"co ja vim", "zacal sluzbu", "usnul", "vzbudil se"};
struct sGuardAction
{
    int hodina;
    int minuta;

    EAction akce;

    bool operator < (const sGuardAction &other) const
    {
        if(hodina < other.hodina)
            return true;

        return minuta < other.minuta;
    }

    void print() const
    {
        printf("\t%02d:%02d - %s\n", hodina, minuta, TEXTS[akce]);
    }
};

struct sGuard
{
    sGuard()
        : id (-1)
    { }
    sGuard(int hodina, int minuta, char *buffer)
    {
        addAction(hodina, minuta, buffer);
    }

    int id = -1;
    std::set<sGuardAction> actions_;

    int sleepInMinute(uint8_t *line = nullptr)
    {
        int sleep = 0;
        int total = 0;
        for (auto &a : actions_)
        {
            if(a.akce == SLEEP)
            {
                sleep = a.minuta;
            }
            if(a.akce == WAKE)
            {
                total += a.minuta - sleep;
                if(line)
                {
                    for(auto *it = line+sleep-1; it != line+a.minuta-1; ++it)
                    {
                        *it = '*';
                    }
                }
            }
        }

        return total;
    }

    void addAction(int hodina, int minuta, char *buffer)
    {
        sGuardAction akce{hodina, minuta, UNKNOWN};
        switch(tolower(*buffer))
        {
        case 'g':
            if(id > 0)
            {
                printf("chyba predikce :), dalsi guard id v jednom dni %d - %s", id, buffer);
                exit(1);
            }
            sscanf(buffer, "%*s #%d", &id);
            akce.akce = START;
            break;
        case 'f':
            akce.akce = SLEEP;
            break;
        case 'w':
            akce.akce = WAKE;
            break;
        default:
            printf("ty kokso, to jsem necekal: %s", buffer);
            exit(1);
        }
//        printf("pridavam akci ID=%d - %d:%d akce=%d\n", id, hodina, minuta, akce.akce);
        actions_.emplace(akce);
    }

    void print() const
    {
        printf("GUARD %d\n", id);
        for(const auto &a : actions_)
        {
            a.print();
        }
    }
};

int main()
{
    std::map<int, sGuard> records_;
    std::map<int, std::vector<std::map<int, sGuard>::iterator> >guards;

    FILE *f = fopen("input.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }

    char buffer[64];
    size_t read = 0;
    int rok,mesic,den,hodina,minuta;
    while((read = readline(f, buffer, sizeof(buffer)/sizeof(char))) > 0)
    {        
        sscanf(buffer, "[%d-%d-%d %d:%d]", &rok,&mesic,&den,&hodina,&minuta);

        int doy = dayOfYear(rok, mesic, den);
        if(hodina > 22)
            ++doy;
        auto it = records_.find(doy);
        int gid;
        if(it == records_.end())
        {
            records_[doy] = sGuard(hodina, minuta, buffer + 19);
            gid = records_[doy].id;
        }
        else
        {
            (*it).second.addAction(hodina, minuta, buffer + 19);
            gid = (*it).second.id;
        }
//        std::cout << doy << "   " << rok<<" "<<mesic<<" "<<den<<" "<<hodina<<" "<<minuta<<" : ";
//        std::cout << (buffer + 19) << std::endl;
    }

    for(auto r = records_.begin(); r != records_.end(); ++r)
    {
        auto it = guards.find((*r).second.id);
        if(it == guards.end())
        {
            guards[(*r).second.id] = std::vector<std::map<int, sGuard>::iterator> {r};
        }
        else
        {
            (*it).second.push_back(r);//guards.insert(std::make_pair((*(*it).second).second.id, r));
        }
    }

//    printf("\nGuadrs count: %d\n", guards.size());
//    printf("\n\n");
//    for(const auto& a : records_)
//    {
//        printf("\nday %d: ", a.first);
//        a.second.print();
//    }

//    printf("\n\n");
    int maxsleps= 0;
    int maxsleptid = 0;
    for(const auto& a : guards)
    {
        printf("\nGUARD %d: ", a.first);

        int sum = 0;
        for(auto it = a.second.begin(); it != a.second.end(); ++it)
        {
            sum += (*it)->second.sleepInMinute();
            std::cout << (*it)->second.sleepInMinute() << " ";
        }
        if(sum > maxsleps)
        {
            maxsleps = sum;
            maxsleptid = a.first;
        }
//            (*it)->second.print();
    }

    std::cout << "\n\nMAX slept guard: " << maxsleptid << " with " << maxsleps << std::endl<< std::endl<< std::endl;
    // roztridit pro jednotlive hlidace ve kterych dnech meli sluzbu

    // Find the guard that has the most minutes asleep. What minute does that guard spend asleep the most?
    auto itg = guards.find(maxsleptid);
    auto &litg = (*itg).second;

    uint8_t **sleeparray = new uint8_t*[litg.size()];
    for(size_t i = 0; i < litg.size(); ++i)
    {
        sleeparray[i] = new uint8_t[60];
        memset(sleeparray[i], '.', 60);
        litg[i]->second.sleepInMinute(*(sleeparray + i));
//        std::cout << litg[i]->second.sleepInMinute(*(sleeparray + i)) << std::endl;
    }

    for(size_t i = 0; i < litg.size(); ++i)
    {
        printf("%.60s\n", sleeparray[i]);
    }

    int mostsleepminute=0;
    int mostsleepminuteCount=0;

    for(size_t i = 0; i < 60; ++i)
    {
        int s = 0;
        for(size_t j = 0; j < litg.size(); ++j)
        {
            if(sleeparray[j][i] == '*')
            {
                ++s;
            }
        }
        if(s > mostsleepminuteCount)
        {
            mostsleepminuteCount = s;
            mostsleepminute = i;
        }
    }


    printf("PART1: %d most sleep in %d (%d) => %d\n", maxsleptid, mostsleepminute+1, mostsleepminuteCount, maxsleptid * (mostsleepminute+1));
//    printf("PART2: pocet=%d id=%d\n", ids.size(), *ids.begin());






    // part 2
    //In the example above, Guard #99 spent minute 45 asleep more than any other guard or minute - three times in total. (In all other cases, any guard spent any minute asleep at most twice.)

    std::cout << "\n\nPART2\n\n";
    {
        int superhero = 0;
        int superheroMinute = 0;
        int superheroCount = 0;
    for(auto &itg :guards)
    {
//        auto itg = guards.find(maxsleptid);
        auto &litg = (itg).second;

        uint8_t **sleeparray = new uint8_t*[litg.size()];
        for(size_t i = 0; i < litg.size(); ++i)
        {
            sleeparray[i] = new uint8_t[60];
            memset(sleeparray[i], '.', 60);
            litg[i]->second.sleepInMinute(*(sleeparray + i));
//            std::cout << litg[i]->second.sleepInMinute(*(sleeparray + i)) << std::endl;
        }

        for(size_t i = 0; i < litg.size(); ++i)
        {
            printf("%.60s\n", sleeparray[i]);
        }

        int mostsleepminute=0;
        int mostsleepminuteCount=0;

        for(size_t i = 0; i < 60; ++i)
        {
            int s = 0;
            for(size_t j = 0; j < litg.size(); ++j)
            {
                if(sleeparray[j][i] == '*')
                {
                    ++s;
                }
            }
            if(s > mostsleepminuteCount)
            {
                mostsleepminuteCount = s;
                mostsleepminute = i;
            }
        }

        if((mostsleepminuteCount) > superheroCount)
        {
            superheroMinute = mostsleepminute+1;

            superhero = itg.first;
            superheroCount = mostsleepminuteCount;
        }
        printf("%d most sleep in %d (%d) => %d\n\n", itg.first, mostsleepminute+1, mostsleepminuteCount, itg.first * (mostsleepminute+1));
    }

    printf("\n\nPART2 superhero:%d most sleep in %d (%d) => %d\n", superhero, superheroMinute, superheroCount, superhero * superheroMinute);
    }
    return 0;
}
