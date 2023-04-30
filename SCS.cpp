#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <bits/stdc++.h>

class SCS
{
    public: 
        SCS()
        {
            loadResources();
        }

        void SCS_BFBT_Algorithm()
        {
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();
            // +1 to cover trivial soultions
            shortestCommonSupersequenceLen = maxDepth + 1;

            std::cout << "---List of solutions---" << std::endl;
            Shortest_Common_SupersequenceBFBT(std::string(""));


            std::cout << "-Optimal solution-" << std::endl;
            std::cout << "SCS : " + shortestCommonSupersequence << std::endl;
            std::cout << "SCS length : " << shortestCommonSupersequenceLen << std::endl;

            time(&end);
            measureTime(start, end);
        }

        void SCS_BS_Algorithm()
        {
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();
            shortestCommonSupersequenceLen = maxDepth;

            std::cout << "---List of solutions---" << std::endl;
            Shortest_Common_SupersequenceBS();

            time(&end);
            measureTime(start, end);
        }

    private: 
        
        void measureTime(std::time_t &start, std::time_t &end)
        {
            double time_taken = double(end - start);
            std::cout << "Time taken by program is : " << std::fixed
                << time_taken << std::setprecision(5);
            std::cout << " sec " << std::endl;
        }

        void loadResources()
        {
            std::ifstream testInstance(fPath);

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
        }

        void setScsMinDepth()
        {

            // we do not want to consider solutions that are smaller than the largest sequence in the set

            minDepth = 0;

            for(auto &s : setOfStrings)
            {
                std::size_t curDepth = s.length();

                if(curDepth > minDepth)
                {
                    minDepth = curDepth;
                }
            }

            std::cout << "MIN DEPTH: " << minDepth << std::endl;

        }

        void setScsMaxDepth()
        {

            /*
                Alphabet: S = {a, c, t, g}
                Sequences: SEQ = {acctg, act, atgg} 
                L - Length of max sequence (acctg --> len = 5)
                |S| - length of alphabet ({a,c,t,g} |S| = 4)
                Sequence that surely represents a solution to the problem --> actg actg actg actg actg (legthOfSolution = 4 * 5 = 20)
                MaxDepth = L * |S| 
            */

            std::size_t maxSequenceSize = 0;

            for(auto &s : setOfStrings)
            {
                std::size_t tmpSize = s.length();

                if(tmpSize > maxSequenceSize)
                {
                    maxSequenceSize = tmpSize;
                }
            }

            maxDepth = maxSequenceSize * alphabet.size();

            std::cout << "MAX DEPTH: " << maxDepth << std::endl;

        }

        // tests whether the given sequence is a solution
        bool isCommonSupersequence(const std::string &scs)
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

        // Shortest Common Supersequence Brute-Force Backtracking Algorithm
        void Shortest_Common_SupersequenceBFBT(const std::string &scs)
        {

            std::size_t scsLen = scs.length();

            // we stop the search when we reach a depth greater than maxDepth or when we have already found a solution shorter than the current length 
            // of scsLen
            if(scsLen >= (maxDepth + 1) || shortestCommonSupersequenceLen <= scsLen)
            {
                return;
            }

            if(scsLen >= minDepth && isCommonSupersequence(scs))
            {
                // we know that the size of scsLen is certainly smaller than shortestCommonSupersequenceLen, so we don't need to check
                shortestCommonSupersequenceLen = scsLen;
                shortestCommonSupersequence = scs;
                
                std::cout << scs << std::endl;

                return;
            }


            for(auto &letter : alphabet)
            {
                Shortest_Common_SupersequenceBFBT(scs + letter);
            }

        }


        void Shortest_Common_SupersequenceBS()
        {
            std::vector<std::string> *sls1 = new std::vector<std::string>({"a","b","c"});
            std::vector<std::string> *sls2 = new std::vector<std::string>();

            for(int i = 2;i < 4;++i)
            {
                for(auto &s : *sls1)
                {
                    for(auto &c : alphabet)
                    {
                        sls2->emplace_back(s + c);
                    }
                }
                
                

                std::cout << "####" << std::endl;
                for(auto &s : *sls2)
                {
                    std::cout << s << std::endl;
                }
                std::cout << "####" << std::endl;

                sls1->clear();
                std::vector<std::string> *tmpSls = sls1;
                sls1 = sls2; 
                sls2 = tmpSls;        
            }

            delete sls1;
        }

        
        std::vector<std::string>* takeBetaSequances(std::vector<std::string> *sls)
        {
            
        }

        // count how many subsequences scs have in setOfString
        std::size_t countSubsequences(std::string &scs)
        {
            std::size_t ct = 0;
            for(auto &s : setOfStrings)
            {
                
            }
        }

        // is s subsequence of scs
        bool isSubsequence(std::string &s, std::string &scs)
        {

        }

        // the sequence that represents the solution to the problem and its size 
        std::string shortestCommonSupersequence = std::string("");
        std::size_t shortestCommonSupersequenceLen = 0;

        // maximal and minimal depth to consider in algorithm
        std::size_t minDepth;
        std::size_t maxDepth;

        std::vector<char> alphabet;
        std::set<std::string> setOfStrings;

        const std::string fPath = "TestInstances3.txt";
        
        const std::size_t beamSize = 5;

};

int main(int argc, char **argv)
{
    SCS alg;
    alg.SCS_BS_Algorithm();

    return 0;
}