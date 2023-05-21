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
            printAlphabetAndSequences();
            fResults << "------------------" << std::endl;
            fResults << "SCS_BFBT_Algorithm" << std::endl;
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();

            fResults << "MIN DEPTH: " << minDepth << std::endl;
            fResults << "MAX DEPTH: " << maxDepth << std::endl;

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
            printAlphabetAndSequences();
            fResults << "------------------" << std::endl;
            fResults << "SCS_BS_Algorithm" << std::endl;
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();

            std::size_t minimalDepth = (minDepth < bsGreedyMinDepth) ? minDepth : bsGreedyMinDepth;
            fResults << "MIN DEPTH: " << minimalDepth << std::endl;

            shortestCommonSupersequenceLen = maxDepth;

            Shortest_Common_SupersequenceBS();

            fResults << "-Beam Search solution-" << std::endl;
            fResults << "SCS : " + shortestCommonSupersequence << std::endl;
            fResults << "SCS length : " << shortestCommonSupersequenceLen << std::endl;

            time(&end);
            measureTime(start, end);
            fResults << "------------------" << std::endl;
        }

        void LAWMMAlgorithm()
        {
            printAlphabetAndSequences();
            fResults << "------------------" << std::endl;
            fResults << "LAWMMAlgorithm" << std::endl;
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();

            // +1 to cover trivial soultions
            shortestCommonSupersequenceLen = maxDepth + 1;

            fResults << "---List of solutions---" << std::endl;
 
            LAWMMCONTROL();

            fResults << "-Optimal solution-" << std::endl;
            fResults << "SCS : " + shortestCommonSupersequence << std::endl;
            fResults << "SCS length : " << shortestCommonSupersequenceLen << std::endl;

            time(&end);
            measureTime(start, end);
            fResults << "------------------" << std::endl;
        }

        static bool sortRev1(const std::pair<std::size_t, std::string> &a,  const std::pair<std::size_t, std::string> &b) 
        { 
            return (a.first > b.first); 
        }

        static bool sortRev2(const std::pair<char, std::size_t> &a,  const std::pair<char, std::size_t> &b) 
        { 
            return (a.second > b.second); 
        }

        static bool sortRev3(const std::pair<std::string, std::size_t> &a,  const std::pair<std::string, std::size_t> &b) 
        { 
            return (a.second > b.second); 
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
            for(auto &c : inputAlphabet)
            {
                alphabet.emplace_back(c);
            }

            std::string inputS;
            while(std::getline(fInput, inputS))
            {
                setOfStrings.emplace(inputS);
            }

            setOfStringsSize = setOfStrings.size();
        }

        void printAlphabetAndSequences()
        {
            fResults << "******************" << std::endl;
            fResults << "Alphabet : ";
            for(char &c : alphabet)
            {
                fResults << c << " ";
            }
            fResults << std::endl;

            fResults << "Sequences:" << std::endl;
            for(const std::string &s : setOfStrings)
            {
                fResults << s << std::endl;
            }
            fResults << "******************" << std::endl;
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

            bool printIndicator = false;
            
            std::vector<std::string> *sls1 = new std::vector<std::string>();
            for(auto &c : alphabet)
            {
                sls1->emplace_back(1,c);
            }

            std::vector<std::string> *sls2 = new std::vector<std::string>();

            bool isSuperSequenceFound = false;

            // set depth to 1 because we start from level 1 of the search tree
            std::size_t depth = 1;

            // start taking beta sequances when depth >= minDepth, sometimes the minimum depth can be
            // too big, which drastically affects the performance, that's why we limit the depth with a parameter
            std::size_t minimalDepth = (minDepth < bsGreedyMinDepth) ? minDepth : bsGreedyMinDepth;

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

                if(depth >= minimalDepth)
                {
                    sls2 = takeBetaSequancesGreedyApproach(sls2, isSuperSequenceFound, printIndicator);
                    printIndicator = true;
                }

                sls1->clear();
                std::vector<std::string> *tmpSls = sls1;
                sls1 = sls2; 
                sls2 = tmpSls;        
            }

            delete sls1;
            delete sls2;
        }
        
        std::vector<std::string>* takeBetaSequancesGreedyApproach(std::vector<std::string> *sls,
                                                                  bool &isSuperSequenceFound,
                                                                  bool &printIndicator)
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
            rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
            std::shuffle(slsCnt.begin(), slsCnt.end(), rng);

            std::sort(slsCnt.begin(),slsCnt.end(),sortRev1);

            if(printIndicator)
            {
                fResults << "---SLS---" << std::endl;
                for(auto &s : slsCnt)
                {
                    fResults << s.first << " " << s.second << std::endl;
                }
            }

            // std::size_t sameCnt = 0;
            // std::size_t bestCnt = slsCnt[0].first;
            // for(auto &s : slsCnt)
            // {
            //     if(s.first == bestCnt)
            //     {
            //         ++sameCnt;
            //     }
            // }
            // fResults << "SAME CNT:" << sameCnt  << std::endl;
            // if(sameCnt >= beamSize)
            // {
            //     slsCnt = std::vector<std::pair<std::size_t, std::string>>(slsCnt.begin(),slsCnt.begin() + sameCnt);
            // }
            // fResults << "BEGIN REDUCING*********"  << std::endl;
            // for(auto &s : slsCnt)
            // {
            //     fResults << s.first << " " << s.second << std::endl;
            // }
            // fResults << "END REDUCING*********"  << std::endl;

            // std::shuffle(slsCnt.begin(), slsCnt.end(), rng);

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


        void LAWMMCONTROL()
        {
            // scss solution for lawmm heuristic
            std::string solution("");

            // we make a copy of setOfStrings so they don't change the real set
            std::vector<std::string>setOfStringsCpy;
            for(std::string s : setOfStrings)
            {
                setOfStringsCpy.emplace_back(s);
            }

            // end of algorithm indicator
            bool end = false;

            while(!end)
            {
                fResults << "BEGIN:PHASE OF CHOOSING THE BEST CHARACTER" << std::endl;

                // vector of pairs <solution, weight>
                std::vector<std::pair<std::string, std::size_t>> vecSls;

                std::vector<std::string> lookAheadCombinations;
                makeLACombinations(std::string(""), 0, lookAheadCombinations);

                // for each combination of length max lookAhead we calculate the weight
                for(std::string &s : lookAheadCombinations)
                {
                    std::pair<std::string, std::size_t> sls = WMMForGivenSolution(setOfStringsCpy, s);

                    /* 
                    do not insert sls that already exists or one that could not be generated because not a single word
                    starts with the first character of the sequence s
                    
                        Example1:
                            Alphabet = {a,b}
                            setOfStringsCpy: {
                                "abb",
                                "ab"
                                "a",
                                }
                            lookAhead = 2
                            lookAheadCombinations = {aa,ab,ba,bb}

                            WMMForGivenSolution will return in order for "aa" -> ["a",3]
                                                                     for "ab" -> ["ab",4]
                                                                     for "ba" -> ["",0]
                                                                     for "bb" -> ["",0]
                            so we don't include "ba" and "bb"
                        Example2:
                            Alphabet = {a,b,c}
                            setOfStringsCpy: {
                                "abba",
                                "abcc"
                                "abcc",
                                }    
                            lookAhead = 3   
                            lookAheadCombinations = {aaa,aab, ... , aca, acb, ...}

                            WMMForGivenSolution will return for "aca" and "acb" the following:
                                for "aca" -> ["a",9]
                                for "acb" -> ["a",9] 
                            so there is no need to insert the same value (also a better one could not be generated)
                    */

                    if(sls.first != std::string("") && (std::find(vecSls.begin(),vecSls.end(),sls) == vecSls.end()))
                    {
                        vecSls.emplace_back(sls);
                    }
                }

                // weight sorting
                std::sort(vecSls.begin(), vecSls.end(), sortRev3);
                
                for(auto s : vecSls)
                {
                    fResults << s.first << "-" << s.second << std::endl;
                }

                // for the best character we take the first character of the best lookAhead solution
                char bestChar = vecSls[0].first[0];
                
                // we add the best character to the scss solution
                solution += bestChar;

                //weight of the best solution
                std::size_t bestWeigth = vecSls[0].second;

                fResults << "Best letter to remove:" << bestChar << std::endl;
                fResults << "END:PHASE OF CHOOSING THE BEST CHARACTER" << std::endl;

               // from each sequence that starts with the best selected character in setOfStrings we
               // remove that character from the beginning of that sequence
                for(std::string &s : setOfStringsCpy)
                {
                    if(s.starts_with(bestChar))
                    {
                        s.erase(s.begin());
                    }
                    fResults << s << std::endl;
                }

                /*
                    If bestWeigth == 0 we know that we have reached the end and that there are only words left that
                    are either empty or have only one character, because otherwise wmm would calculate a weight greater than 0
                */
                if(bestWeigth == 0)
                {
                    std::string postRes("");
                    for(std::string &s : setOfStringsCpy)
                    {
                        /* 
                        when appending the remaining characters (setOfStringsCpy is empty or there are strings with one character each)
                        we need to connect the remaining characters, but without repetition ("","a","a","")--> connect 'a' but only once
                        */
                        if(!s.empty() && std::find(postRes.begin(),postRes.end(),s[0]) == postRes.end())
                        {
                            postRes += s[0];
                        }
                    }
                    solution += postRes;
                    end = true;
                }
                fResults << "Temp Solution:" << solution << std::endl;
            }

            if(isCommonSupersequence(solution))
            {
                shortestCommonSupersequence = solution;
                shortestCommonSupersequenceLen = solution.length();
            }
            else
            {
                fResults << "LAWMM could not find a solution" << std::endl;
            }
        }

        // makes all combinations of letters from alphabet, the sequence size is lookAhead size
        void makeLACombinations(const std::string &sls, 
                                const std::size_t &counter,
                                std::vector<std::string>  &lookAheadCombinations)
        {

            /*
                Example:    
                    Alphanet = {a,b}
                    lookAhead = 2
                    function return {aa,ab,ba,bb}
            */

            if(counter == lookAhead)
            {
                lookAheadCombinations.emplace_back(sls);
                return;
            }

        
            for(char c : alphabet)
            {
                makeLACombinations(sls + c, counter + 1, lookAheadCombinations);
            }
        }

        std::pair<std::string, std::size_t> WMMForGivenSolution(std::vector<std::string> &setOfStrings, std::string &sls)
        {

        /* 

            For a given setOfStrings and a given sequence (sls), the function calculates the wmm weight for the selected sequence,
            and if it is not possible for the whole sequence, then for the part of the sequence that can be calculated
         
            example1: 
                Alphabet : a, b, c
                setOfStrings: {
                                "abc",
                                "cab"
                                "bac"
                                }
                sls = "ab"
                ret = {"ab",5}

            example2:
                Alphabet : a, b, c
                setOfStrings: {
                                "abc",
                                "cab"
                                "bac"
                                }
                sls = "cc"
                ret = {"c",2}
        
        */
            
            std::vector<std::string>setOfStringsCpy;
            for(std::string s : setOfStrings)
            {
                setOfStringsCpy.emplace_back(s);
            }
            
            std::string retSls("");

            std::size_t weight = 0;
            for(char c : sls)
            {
                bool startWith = false;
                for(std::string &s: setOfStringsCpy)
                {
                    if(s.starts_with(c))
                    {
                        weight += (s.length() - 1);
                        s.erase(s.begin());
                        startWith = true;
                    }
                }
                if(startWith)
                {
                    retSls += c;
                }
                else
                {
                    break;
                }
            }

            return std::pair<std::string, std::size_t>(retSls, weight);
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
        const std::size_t bsGreedyMinDepth = 6;

        // Probabilistic Beam Search
        std::size_t lookAhead = 1;

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
        return EXIT_SUCCESS;
    }

    SCS alg(argv[1]);
    // alg.SCS_BFBT_Algorithm();
    alg.SCS_BS_Algorithm();
    alg.LAWMMAlgorithm();

    return EXIT_SUCCESS;
}