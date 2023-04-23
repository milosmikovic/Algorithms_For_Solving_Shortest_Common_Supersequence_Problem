#include <iostream>
#include <fstream>
#include <vector>
#include <set>


// the sequence that represents the solution to the problem and its size 
std::string *shortestCommonSupersequence = nullptr;
std::size_t shortestCommonSupersequenceLen = 0;

// maximal and minimal depth to consider in algorithm
std::size_t maxDepth = 0;
std::size_t minDepth = 0;

// we do not want to process a depth greater than the sum of the sizes of all input strings
std::size_t scsMaxDepth(std::set<std::string> &setOfStrings)
{
    std::size_t maximalDepth = 0;

    for(auto &s : setOfStrings)
    {
        maximalDepth += s.length();
    }

    return maximalDepth;
}

// we do not want to consider solutions that are smaller than the largest sequence in the set
std::size_t scsMinDepth(std::set<std::string> &setOfStrings)
{
    std::size_t minimalDepth = 0;

    for(auto &s : setOfStrings)
    {
        std::size_t curDepth = s.length();

        if(curDepth > minimalDepth)
        {
            minimalDepth = curDepth;
        }
    }

    return minimalDepth;
}

// tests whether the given sequence is a solution
bool isCommonSupersequence(const std::string &scs, const std::set<std::string> &setOfStrings)
{

    // begin and end iterators of given solution
    std::string::const_iterator scsTmpPos;
    std::string::const_iterator scsEnd;

    // begin and end iterators of current sequnce in set
    std::string::const_iterator sTmpPos;
    std::string::const_iterator sEnd;

    for(const std::string &s : setOfStrings)
    {
        scsTmpPos = scs.cbegin();
        scsEnd = scs.cend();

        sTmpPos = s.cbegin();
        sEnd = s.cend();


        while(sTmpPos != sEnd)
        {

            // if we have reached the last character in the solution, it means that the current string
            //  is not contained and the solution (scs) is not supersquence so we do not need to consider the other sequences in the set
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

// Shortest Common Supersequence Backtracking Algorithm
void Shortest_Common_Supersequence(const std::vector<char> &alphabet, 
                                   const std::set<std::string> &setOfStrings,
                                   const std::string &scs)
{

    std::size_t scsLen = scs.length();

    // we stop the search when we reach a depth greater than maxDepth or when we have already found a solution shorter than the current length 
    // of scsLen
    if(scsLen >= (maxDepth + 1) || shortestCommonSupersequenceLen <= scsLen)
    {
        return;
    }

    if(scsLen >= minDepth && isCommonSupersequence(scs,setOfStrings))
    {
        // we know that the size of scsLen is certainly smaller than shortestCommonSupersequenceLen, so we don't need to check
        shortestCommonSupersequenceLen = scsLen;
        shortestCommonSupersequence->assign(scs);
        
        std::cout << scs << std::endl;

        return;
    }


    for(auto &letter : alphabet)
    {
        Shortest_Common_Supersequence(alphabet, setOfStrings, scs + letter);
    }

    
}

int main(int argc, char **argv)
{

    // initialize result sequence to the empty string at the beginning
    shortestCommonSupersequence = new std::string("");

    std::vector<char> alphabet;
    std::set<std::string> setOfStrings;

    std::ifstream testInstance("TestInstances.txt");

    std::string inputAlphabet;
    std::getline(testInstance, inputAlphabet);
    std::cout << "Alphabet : ";
    for(auto &c : inputAlphabet)
    {
        alphabet.emplace_back(c);
        std::cout << c << " ";
    }
    std::cout << std::endl;

    std::string inputS;
    std::cout << "Sequences:" << std::endl;
    while(std::getline(testInstance, inputS))
    {
        setOfStrings.emplace(inputS);
        std::cout << inputS << std::endl;
    }


    std::cout << "#####Shortest_Common_Supersequence#####" << std::endl;

    maxDepth = scsMaxDepth(setOfStrings);
    minDepth = scsMinDepth(setOfStrings);

    // set shortest common supersequence length to the sum of the lengths of all strings
    shortestCommonSupersequenceLen = maxDepth;

    Shortest_Common_Supersequence(alphabet, setOfStrings, std::string(""));


    std::cout << "#####Shortest_Common_Supersequence Solution#####" << std::endl;

    std::cout << "SCS : " + *shortestCommonSupersequence << std::endl;
    std::cout << "Length : " << shortestCommonSupersequenceLen << std::endl;

    if(shortestCommonSupersequence)
    {
        delete shortestCommonSupersequence;
    }

    return 0;
}