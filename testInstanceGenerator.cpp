#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <regex>
#include <time.h>

std::string generateFName()
{
    std::string path = "./";
    std::vector<std::string> dirF;
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        std::regex str_expr ("(./TestInstances)[0-9]+(.txt)");
        if (std::regex_match(std::string(entry.path()),str_expr))
        {
            dirF.emplace_back(entry.path());
        }
    }

    if(dirF.empty())
    {
        return std::string("TestInstances1.txt");
    }
    else
    {
        std::regex regexp("[0-9]+");
        std::smatch m;
        std::size_t num = 0;
        for(auto &s : dirF)
        {
            std::regex_search(s, m, regexp);
            for(auto s : m)
            {
                std::size_t tmpNum = std::stoi(s);
                if(tmpNum > num)
                {
                    num = tmpNum;
                }
            }
        }
        return std::string("TestInstances" + std::to_string(num + 1) + ".txt");
    }
}

void generateTestInstances(const std::size_t &n, const std::size_t &m)
{

    std::string fName = generateFName();
    std::ofstream os(fName);

    std::cout << "Insert alphabet:";
    char c;
    std::vector<char> alphabet;

    while(std::cin >> c)
    {
        alphabet.emplace_back(c);
        os << c;
    }
    os << std::endl;

    std::vector<std::string> sqs;
    std::size_t cnt = 0;

    while(cnt < m)
    {
        std::string sequence("");
        for(int j = 0;j < n;++j)
        {
            std::size_t rElem = std::rand() % alphabet.size();
            char c = alphabet[rElem];
            sequence += std::string(1,c);
        }

        // if we need to have all different sequences in set of sequences
        if(std::find(sqs.begin(),sqs.end(),sequence) == sqs.end())
        {
            os << sequence;
            sqs.emplace_back(sequence);
            ++cnt;
            if(cnt != m)
            {
                os << std::endl;
            }
        }
    }
}

int main(int argc, char **argv)
{

    std::srand(std::time(NULL));

    if(argc != 3)
    {
        std::cout << "Run: ./testInstanceGenerator N(length of sequences) M(number of sequences)" << std::endl;
        return EXIT_SUCCESS;
    }

    std::size_t n = std::atoi(argv[1]);
    std::size_t m = std::atoi(argv[2]);

    generateTestInstances(n, m);

    return EXIT_SUCCESS;
}