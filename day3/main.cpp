#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>
int main()
{
    FILE *f = fopen("input.txt", "r");
    if(f == NULL)
    {
        printf("CANNOT OPEN");
        return 1;
    }
    std::vector<std::string> lines;
    std::vector<const std::string*> matches;
    size_t matchpos = 0;

    char buffer[32];

    int counter['z' - 'a' + 1] = {};

    int twos = 0;
    int tres = 0;

    size_t read = 0;
    size_t r = 0;
    do
    {
        read = 0;
        do
        {
            r = fread(buffer + read, 1, 1, f);
            read += r;
        }
        while(r > 0 && buffer[read-1] != '\n');

        if(buffer[read-1] == '\n')
        {
            --read;
        }

//        printf("r=%d read=%d ", r, read);
        if(read == 0)
        {
            break;
        }
        lines.push_back(std::string(buffer, read));
        for (char *it = buffer; it != buffer + read; ++it)
        {
            ++counter[int(*it) - 'a'];
//            printf("%c", *it);
        }
        int tmptwos = 0, tmptres = 0;
        for(auto it = counter; it != (counter + sizeof(counter)/sizeof(int)); ++it)
        {
            if (*it == 3)
            {
                ++tmptres;
            }
            else if(*it == 2)
            {
                ++tmptwos;
            }

            *it = 0;
        }
//        printf("  tmptwos='%d' tmptres='%d'\n", tmptwos, tmptres);
        twos += (tmptwos > 0) ? 1 : 0;
        tres += (tmptres > 0) ? 1 : 0;
    }
    while(r > 0);

    printf("PART1=%d*%d=%d\n", twos, tres, twos * tres);


    for(const auto &line : lines)
    {
        for(auto it = lines.cbegin(); it != lines.cend(); ++it)
        {
            int diffcount = 0;
            size_t mp = 0;
            if((*it).size() == line.size())
            {
                for(size_t i = 0; i < line.size(); ++i)
                {
                    if(line[i] != (*it)[i])
                    {
                        ++ diffcount;
                        mp = i;
                    }
                    if(diffcount > 1)
                    {
                        break;
                    }
                }
                if(diffcount == 1)
                {
                    matches.push_back(&line);
                    matchpos = mp;
                }
            }
        }
    }

    std::cout << "matches: " << matches.size() << " " << matchpos << std::endl;
    for(auto &m : matches)
    {
        std::cout << *m << std::endl;
    }

    std::cout << "\n\nPART2=" << matches.size() << " : " << matchpos << " - ";
    auto &mf = matches.front();
    for(size_t i = 0; i < mf->size(); ++i)
    {
        if(i != matchpos)
        {
            std::cout << (*mf)[i];
        }
    }

    std::cout << std::endl;




    return 0;
}
