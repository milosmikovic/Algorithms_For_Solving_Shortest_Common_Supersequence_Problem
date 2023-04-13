#include <iostream>
#include <vector>
#include <set>



std::string *shortestCommonSupersequence = nullptr;
std::size_t shortestCommonSupersequenceLen = 0;

int scsMaxDepth(std::set<std::string> &setOfStrings)
{
    std::size_t maxDepth = 0;
    for(auto &s : setOfStrings)
    {
        maxDepth += s.length();
    }

    // we do not want to process a depth greater than the sum of the sizes of all input strings
    return maxDepth;
}

bool isCommonSupersequence(const std::string &scs, const std::set<std::string> &setOfStrings)
{
    for(const std::string &s : setOfStrings)
    {
        auto scsTmpPos = scs.cbegin();
        auto scsEnd = scs.cend();
        auto sTmpPos = s.cbegin();
        auto sEnd = s.cend();

        while(sTmpPos != sEnd)
        {
            if(scsTmpPos == scsEnd)
            {
                return false;
            }
            else if(*sTmpPos == *scsTmpPos)
            {
                ++sTmpPos;
                ++scsTmpPos;
            }
            else
            {
                ++scsTmpPos;
            }
        }
    }

    return true;
}

template <typename T>
void printCollection(T &alphabet)
{
    std::cout << "Collection printing..." << std::endl;
    auto end = alphabet.cend();
    for(auto it = alphabet.cbegin(); it != end; ++it)
    {
        std::cout << *it << std::endl;
    }
}

// std::pair<std::string,int> 
void Shortest_Common_Supersequence(const std::vector<char> &alphabet, 
                                   const std::set<std::string> &setOfStrings,
                                   const int maxDepth,
                                   const std::string &scs)
{

    std::size_t scsLen = scs.length();

    if(scsLen == (maxDepth + 1))
    {
        return;
    }

    if(shortestCommonSupersequenceLen <= scsLen)
    {
        return;
    }

    if(isCommonSupersequence(scs,setOfStrings))
    {
        // we know that the size of scsLen is certainly smaller than shortestCommonSupersequenceLen, so we don't need to check
        shortestCommonSupersequenceLen = scsLen;
        shortestCommonSupersequence->assign(scs);
        
        std::cout << scs << std::endl;

        return;
    }


    for(auto letter : alphabet)
    {
        Shortest_Common_Supersequence(alphabet, setOfStrings, maxDepth, scs + letter);
    }

    
}

int main(int argc, char **argv)
{

    shortestCommonSupersequence = new std::string("");

    std::vector<char> alphabet;
    std::set<std::string> setOfStrings;

    std::cout << "Enter the letters of the alphabet: " << std::endl;
    char inputC;
    while((inputC = std::getchar()) != '\n')
    {
        alphabet.push_back(inputC);
    }
    printCollection(alphabet);


    std::cout << "Enter the set of strings: " << std::endl;
    std::string inputS;
    while(std::getline(std::cin,inputS))
    {
        setOfStrings.emplace(inputS);
    }
    printCollection(setOfStrings);


    std::cout << "#####Shortest_Common_Supersequence#####" << std::endl;

    int maxDepth = scsMaxDepth(setOfStrings);

    // set shortest common supersequence length to the sum of the lengths of all strings
    shortestCommonSupersequenceLen = maxDepth;

    Shortest_Common_Supersequence(alphabet, setOfStrings, maxDepth, std::string(""));


    std::cout << "#####Shortest_Common_Supersequence Solution#####" << std::endl;

    std::cout << "SCS:" + *shortestCommonSupersequence << std::endl;
    std::cout << "Length:" + shortestCommonSupersequenceLen << std::endl;

    if(shortestCommonSupersequence)
    {
        delete shortestCommonSupersequence;
    }

    return 0;
}