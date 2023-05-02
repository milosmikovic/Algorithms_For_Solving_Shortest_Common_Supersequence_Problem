#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <bits/stdc++.h>


class SCS
{
    public: 
        SCS(const std::string &testInstance)
        {
            inPath += testInstance + ".txt";
            outPath += testInstance + ".txt";
            loadResources();
        }

        ~SCS()
        {
            fResults.close();
            fInput.close();
        }

        void SCS_BFBT_Algorithm()
        {
            fResults << "------------------" << std::endl;
            fResults << "SCS_BFBT_Algorithm" << std::endl;
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();

            // +1 to cover trivial soultions
            shortestCommonSupersequenceLen = maxDepth + 1;

            fResults << "---List of solutions---" << std::endl;
            Shortest_Common_SupersequenceBFBT(std::string(""));


            fResults << "-Optimal solution-" << std::endl;
            fResults << "SCS : " + shortestCommonSupersequence << std::endl;
            fResults << "SCS length : " << shortestCommonSupersequenceLen << std::endl;

            time(&end);
            measureTime(start, end);
            fResults << "------------------" << std::endl;
        }

        void SCS_BS_Algorithm()
        {
            fResults << "------------------" << std::endl;
            fResults << "SCS_BS_Algorithm" << std::endl;
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();
            shortestCommonSupersequenceLen = maxDepth;

            Shortest_Common_SupersequenceBS();

            fResults << "-Beam Search solution-" << std::endl;
            fResults << "SCS : " + shortestCommonSupersequence << std::endl;
            fResults << "SCS length : " << shortestCommonSupersequenceLen << std::endl;

            time(&end);
            measureTime(start, end);
            fResults << "------------------" << std::endl;
        }

        static bool sortrev(const std::pair<std::size_t, std::string> &a,  const std::pair<std::size_t, std::string> &b) 
        { 
            return (a.first > b.first); 
        }

    private: 
        
        void measureTime(std::time_t &start, std::time_t &end)
        {
            double time_taken = double(end - start);
            fResults << "Time taken by program is : " << std::fixed
                << time_taken << std::setprecision(5);
            fResults << " sec " << std::endl;
        }

        void loadResources()
        {
            fInput = std::ifstream(inPath);
            fResults = std::ofstream(outPath);

            std::string inputAlphabet;
            std::getline(fInput, inputAlphabet);
            fResults << "Alphabet : ";
            for(auto &c : inputAlphabet)
            {
                alphabet.emplace_back(c);
                fResults << c << " ";
            }
            fResults << std::endl;

            std::string inputS;
            fResults << "Sequences:" << std::endl;
            while(std::getline(fInput, inputS))
            {
                setOfStrings.emplace(inputS);
                fResults << inputS << std::endl;
            }

            setOfStringsSize = setOfStrings.size();
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

            fResults << "MIN DEPTH: " << minDepth << std::endl;

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

            fResults << "MAX DEPTH: " << maxDepth << std::endl;

        }

        // tests whether the given sequence is a solution
        bool isCommonSupersequence(const std::string &scs)
        {

            for(const std::string &s : setOfStrings)
            {
                if(!isSubsequence(s,scs))
                {
                    return false;
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
                              
                // optimization is possible if the sequence is of the form aaaR if R is the solution then we can 
                // remove the a's from the beginning...
                std::string::const_iterator scsPos = scs.cbegin();
                std::string::const_iterator scsEnd = scs.cend();
                bool isStillSupersequence = true;
                while(isStillSupersequence)
                {
                    if(isCommonSupersequence(std::string(scsPos + 1, scsEnd)))
                    {
                        ++scsPos;
                    }
                    else
                    {
                        isStillSupersequence = false;
                    }
                }

                
                shortestCommonSupersequence = std::string(scsPos, scsEnd);
                shortestCommonSupersequenceLen = shortestCommonSupersequence.length();
                
                fResults << shortestCommonSupersequence << std::endl;
 

                return;
            }


            for(auto &letter : alphabet)
            {
                Shortest_Common_SupersequenceBFBT(scs + letter);
            }

        }


        void Shortest_Common_SupersequenceBS()
        {

            std::vector<std::string> *sls1 = new std::vector<std::string>();
            for(auto &c : alphabet)
            {
                sls1->emplace_back(1,c);
            }

            std::vector<std::string> *sls2 = new std::vector<std::string>();

            bool isSuperSequenceFound = false;

            // set depth to 1 because we start from level 1 of the search tree
            std::size_t depth = 1;

            while(!isSuperSequenceFound)
            {

                ++depth;

                for(auto &s : *sls1)
                {
                    for(auto &c : alphabet)
                    {
                        sls2->emplace_back(s + c);
                    }
                }

                // 1. odaberem beta sekvenci, za svaku proverim da li je supersequence, ako ne nadjem idem dalje ako nadjem stajem
                // optimizacija da ne vracam vector stringova pa da za svaki proveravam da li je supersequence nego
                // da posaljem vector parova pa ako je vrednost nekog para == broju sekvenci u setOfString onda imam resenje...

                // start taking beta sequances when depth >= minDepth
                if(depth >= minDepth)
                {
                    sls2 = takeBetaSequances(sls2, isSuperSequenceFound);
                }

                sls1->clear();
                std::vector<std::string> *tmpSls = sls1;
                sls1 = sls2; 
                sls2 = tmpSls;        
            }

            delete sls1;
            delete sls2;
        }
        
        std::vector<std::string>* takeBetaSequances(std::vector<std::string> *sls, bool &isSuperSequenceFound)
        {
            std::vector<std::pair<std::size_t, std::string>> slsCnt;
            
            for(auto &s : *sls)
            {
                slsCnt.emplace_back(std::pair<std::size_t, std::string>(countSubsequences(s), s));
            }

            // deleting sls because we dont need it anymore
            delete sls;

            // shuffle elements before sorting to avoid same sort order every time, and avoid algorithm loop bug when most of 
            // sequences have same count of subsequences ...
            auto rng = std::default_random_engine();
            std::shuffle(slsCnt.begin(), slsCnt.end(), rng);

            std::sort(slsCnt.begin(),slsCnt.end(),sortrev);

            fResults << "---SLS---" << std::endl;
            for(auto &s : slsCnt)
            {
                fResults << s.first << " " << s.second << std::endl;
            }


            const std::pair<std::size_t, std::string> potentialSuperSequence = slsCnt[0];
            if(potentialSuperSequence.first == setOfStringsSize)
            {
                isSuperSequenceFound = true;
                shortestCommonSupersequence = potentialSuperSequence.second;
                shortestCommonSupersequenceLen = potentialSuperSequence.second.length();
            }

            std::vector<std::string> *newSls = new std::vector<std::string>();
            for(int i = 0;i < beamSize;++i)
            {
                newSls->emplace_back(slsCnt[i].second);
            }

            fResults << "---SLS_NEW---" << std::endl;
            for(auto &s : *newSls)
            {
                fResults << s << std::endl;
            }

            return newSls;

        }

        // count how many subsequences scs have in setOfString
        std::size_t countSubsequences(const std::string &scs)
        {
            std::size_t ct = 0;

            for(auto &s : setOfStrings)
            {
                if(isSubsequence(s,scs))
                {
                    ++ct;
                }
            }

            return ct;

        }

        // is s subsequence of scs
        bool isSubsequence(const std::string &s,const std::string &scs)
        {
            std::string::const_iterator sPos = s.cbegin();
            std::string::const_iterator sEnd = s.cend();

            std::string::const_iterator scsPos = scs.cbegin();
            std::string::const_iterator scsEnd = scs.cend();

            while(sPos != sEnd)
            {
                if(scsPos == scsEnd)
                {
                    return false;
                }
                else if(*sPos == *scsPos)
                {
                    ++sPos;
                    ++scsPos;
                }
                else
                {
                    ++scsPos;
                }
            }

            return true;

        }

        // the sequence that represents the solution to the problem and its size 
        std::string shortestCommonSupersequence = std::string("");
        std::size_t shortestCommonSupersequenceLen = 0;

        // maximal and minimal depth to consider in algorithm
        std::size_t minDepth;
        std::size_t maxDepth;

        std::vector<char> alphabet;
        std::set<std::string> setOfStrings;
        std::size_t setOfStringsSize;

        const std::size_t beamSize = 5;

        std::string inPath = "TestInstances";
        std::string outPath = "Results";
        std::ofstream fResults;
        std::ifstream fInput;
         

};

int main(int argc, char **argv)
{
    
    if(argc != 2)
    {
        std::cout << "Run: ./SCS N(number of test instance to load)" << std::endl;
        return 0;
    }

    SCS alg(argv[1]);
    alg.SCS_BFBT_Algorithm();
    alg.SCS_BS_Algorithm();

    return 0;
}