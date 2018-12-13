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
const char *TEXTS[] {"co ja vim", "zacal sluzbu", "usnul", "vzbudil se"};
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

    bool wasSleepInMinute(int minute)
    {

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
        printf("pridavam akci ID=%d - %d:%d akce=%d\n", id, hodina, minuta, akce.akce);
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

    FILE *f = fopen("testinput.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }
    std::set<int> guards;

    char buffer[64];
    size_t read = 0;
//    #1 @ 1,3: 4x4
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
        guards.insert(gid);


//        std::cout << doy << "   " << rok<<" "<<mesic<<" "<<den<<" "<<hodina<<" "<<minuta<<" : ";
//        std::cout << (buffer + 19) << std::endl;
    }

    printf("\nGuadrs count: %d\n", guards.size());
    printf("\n\n");
    for(const auto& a : records_)
    {
        printf("\nday %d: ", a.first);
        a.second.print();
    }

    // roztridit pro jednotlive hlidace ve kterych dnech meli sluzbu

    // Find the guard that has the most minutes asleep. What minute does that guard spend asleep the most?
//    printf("PART1: sumarum: %d\n", sumarum);
//    printf("PART2: pocet=%d id=%d\n", ids.size(), *ids.begin());

    return 0;
}
