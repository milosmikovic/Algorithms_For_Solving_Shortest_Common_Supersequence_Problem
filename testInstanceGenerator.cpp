#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <regex>
#include <random>


const double removeProb = 0.1;

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

void generateTestInstances(const std::size_t &t)
{

    std::string fName = generateFName();
    std::ofstream os(fName);

    if(t == 1)
    {
        std::size_t n,m;
        std::cout << "N(length of sequences) M(number of sequences):";
        std::cin >> n >> m;
        
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

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(0, alphabet.size() - 1);

        while(cnt < m)
        {
            std::string sequence("");
            for(int j = 0;j < n;++j)
            {
                int rElem = distribution(generator);
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
    else if(t == 2)
    {
        std::cout << "Insert path to file with solution:";
        std::string path;
        std::getline(std::cin,path);
        std::cout << "M(number of sequences):";
        std::size_t numSeq;
        std::cin >> numSeq;

        std::cout << "Insert alphabet:";
        char c;
        std::vector<char> alphabet;

        while(std::cin >> c)
        {
            alphabet.emplace_back(c);
            os << c;
        }
        os << std::endl;

        std::string solution("");
        std::ifstream solutionStream(path);
        std::getline(solutionStream,solution);

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        double randomNum;
        std::size_t cnt = 0;
        while(cnt < numSeq)
        {
            std::string sls("");

            for(auto &c : solution)
            {
                randomNum = distribution(generator);
                if(randomNum > removeProb)
                {
                    sls += c;
                }
            }

            os << sls;
            ++cnt;
            if(cnt != numSeq)
            {
                os << std::endl;
            }
        }

    }
    else
    {
        std::cout << "Test instance generator error!" << std::endl;
    }
}

int main(int argc, char **argv)
{

    if(argc != 2)
    {
        std::cout << "Run: ./testInstanceGenerator T(gen mode 1 or 2)" << std::endl;
        return EXIT_SUCCESS;
    }

    std::size_t t = std::atoi(argv[1]);

    if(t != 1 && t != 2)
    {
        std::cout << "Run: ./testInstanceGenerator T(gen mode 1 or 2)" << std::endl;
        return EXIT_SUCCESS;
    }

    generateTestInstances(t);

    return EXIT_SUCCESS;
}