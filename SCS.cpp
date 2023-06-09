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

        void SCS_MM_Beam_Search_Algorithm()
        {
            printAlphabetAndSequences();
            fResults << "------------------" << std::endl;
            fResults << "SCS_MM_Beam_Search_Algorithm" << std::endl;
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();

            shortestCommonSupersequenceLen = maxDepth;

            MM_Beam_Search();

            fResults << "-Optimal solution-" << std::endl;
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

        void SCS_LAWMM_Beam_Search_Algorithm()
        {
            printAlphabetAndSequences();
            fResults << "------------------" << std::endl;
            fResults << "SCS_LAWMM_Beam_Search_Algorithm" << std::endl;
            time_t start, end;
            time(&start);

            setScsMaxDepth();
            shortestCommonSupersequenceLen = maxDepth;

            LAWMM_Beam_Search();

            fResults << "-LAWMM Beam Search solution-" << std::endl;
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

        static bool sortAsc(const std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> &a,  
                            const std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> &b) 
        { 
            return (a.second.first > b.second.first); 
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

            for(auto &s : setOfStrings)
            {
                setOfStringsVec.emplace_back(s);
            }
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


        void MM_Beam_Search()
        {
            
            std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *sls1 = 
                                                            new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>();
            
            std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *sls2 = 
                                                            new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>();

            bool isSuperSequenceFound = false;

            std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> initialSolution
                        (        
                        std::string(1,alphabet[0]),
                        std::pair<size_t, std::vector<std::size_t>>(0,std::vector<std::size_t>(setOfStringsSize,0))
                        );

            std::vector<std::pair<char, std::size_t>> lr = MM(initialSolution);

            for(auto &c : alphabet)
            {
                std::size_t rankOfC;
                for(int i = 0;i < lr.size();++i)
                {
                    if(c == lr[i].first)
                    {
                        rankOfC = lr[i].second;
                    }
                }

                std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> tmpSls
                    (
                        std::string(1,c), std::pair<size_t, std::vector<std::size_t>>(rankOfC,std::vector<std::size_t>(setOfStringsSize,0))
                    );
                sls1->emplace_back(tmpSls);
            }

            sls1 = takeBetaSequancesGreedyApproach(sls1,isSuperSequenceFound);

            while(!isSuperSequenceFound)
            {
                
                for(auto &s : *sls1)
                {
                    // fResults << "Ranks for seq:" << s.first << std::endl;
                    std::vector<std::pair<char, std::size_t>> letterRanks = MM(s);
                    
                    for(auto &c : alphabet)
                    {
                        std::size_t rankOfC;
                        for(int i = 0;i < letterRanks.size();++i)
                        {
                            if(c == letterRanks[i].first)
                            {
                                rankOfC = letterRanks[i].second;
                            }
                        }
                        std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> newSLS
                        (
                            std::string(s.first + c), std::pair<size_t, std::vector<std::size_t>>(s.second.first + rankOfC, s.second.second)
                        );
                        sls2->emplace_back(newSLS);
                    }
                }
                // memory licking... fix it!!! in wmm also
                delete sls1;
                sls1 = takeBetaSequancesGreedyApproach(sls2, isSuperSequenceFound);
                sls2 = new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>();    
            }

            delete sls1;
            delete sls2;
        }

        
        
        std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>* takeBetaSequancesGreedyApproach(
                                                                                            std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *sls,
                                                                                            bool &isSuperSequenceFound
                                                                                            )
        {

            // if the number of partial solutions is less than the beam size, it means that we take all the solutions, but we have to calculate new positions for partial solutions
            if(sls->size() <= beamSize)
            {
                for(auto &s : *sls)
                {
                    updatePositions(s);
                    if(isCommonSupersequence(s.first))
                    {
                        shortestCommonSupersequence = s.first;
                        shortestCommonSupersequenceLen = s.first.length();
                        isSuperSequenceFound = true;
                    }
                }
                std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>* slsNew = new
                                                                                            std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>(*sls);
                delete sls;
                sls = nullptr;
                return slsNew;
            }
            
            // we mix the elements before sorting to avoid same sort order if all elemts have same ranks (we do shuffle in LAWMM so it's not necessary here)
            auto rng = std::default_random_engine();
            rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
            std::shuffle(sls->begin(), sls->end(), rng);

            // we sort the partial solutions in relation to the sum of the ranks
            std::sort(sls->begin(), sls->end(), sortAsc);

            // fResults << "Sorted VALUES" << std::endl;
            // for(auto &s : *sls)
            // {
            //     fResults << s.first << "-" << s.second.first << std::endl;
            // }

            // we create a vector of new partial solutions
            std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *slsNew =
                                         new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>();
            
            // we choose the best beam size solutions from the newly created partial solutions
            for(int i = 0;i < beamSize;++i)
            {
                // a roulette selection can be added here
                std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> &elemRef = (*sls)[i];

                // updating positions of the partial solution
                updatePositions(elemRef);

                // if a partial solution is a solution to the problem, we stop the algorithm
                if(isCommonSupersequence(elemRef.first))
                {
                    shortestCommonSupersequence = elemRef.first;
                    shortestCommonSupersequenceLen = elemRef.first.length();
                    isSuperSequenceFound = true;
                }
                // fResults << "Nova izabrana sekvenca:" << (*sls)[i].first << std::endl;
                slsNew->emplace_back((*sls)[i]);
            }
            delete sls;
            sls = nullptr;
            return slsNew;
        }

        std::vector<std::pair<char, std::size_t>> MM(std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> &s)
        {
 
            std::vector<std::pair<char, std::size_t>> letterRanks;
            for(auto &c : alphabet)
            {
                letterRanks.emplace_back(c,0);
            }

            for(int i = 0;i < setOfStringsVec.size();++i)
            {
                std::vector<std::size_t> tmpPositions = s.second.second;

                if(tmpPositions[i] <= (setOfStringsVec[i].size() - 1))
                {
                    for(auto &lr : letterRanks)
                    {
                        if(lr.first == setOfStringsVec[i][tmpPositions[i]])
                        {
                            ++lr.second;
                        }
                    }
                }
            }
            // we mix the elements so that 'a' would not always be selected if say a,c,t all have the same value
            auto rng = std::default_random_engine();
            rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
            std::shuffle(letterRanks.begin(), letterRanks.end(), rng);

            std::sort(letterRanks.begin(), letterRanks.end(), sortRev2);

            // fResults << "MM rank latters" << std::endl;

            // for(auto &rank : letterRanks)
            // {
            //     fResults << rank.first << "-" << rank.second << std::endl;
            // }
            
            return letterRanks;
        }

        // count how many subsequences scs have in setOfString
        std::size_t countLettersInSubsequences(const std::string &scs)
        {
            std::size_t cnt = 0;

            for(auto &s : setOfStrings)
            {
                cnt += countLettersInSubsequence(scs,s);
            }
            
            return cnt;
        }

        std::size_t countLettersInSubsequence(const std::string &scs, const std::string &s)
        {
            std::size_t cnt = 0;

            std::string::const_iterator sPos = s.cbegin();
            std::string::const_iterator sEnd = s.cend();

            std::string::const_iterator scsPos = scs.begin();
            std::string::const_iterator scsEnd = scs.cend();

            while(scsPos != scsEnd)
            {
                if(sPos == sEnd)
                {
                    break;
                }
                else if(*scsPos == *sPos)
                {
                    ++scsPos;
                    ++sPos;
                    ++cnt;
                }
                else
                {
                    ++scsPos;
                }
            }

            return cnt;
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
                // fResults << "BEGIN:PHASE OF CHOOSING THE BEST CHARACTER" << std::endl;

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
                
                // for(auto s : vecSls)
                // {
                //     fResults << s.first << "-" << s.second << std::endl;
                // }

                //weight of the best solution
                std::size_t bestWeigth = vecSls[0].second;

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
                else
                {
                    // for the best character we take the first character of the best lookAhead solution
                    char bestChar = vecSls[0].first[0];
                
                    // we add the best character to the scss solution
                    solution += bestChar;


                    // fResults << "Best letter to remove:" << bestChar << std::endl;
                    // fResults << "END:PHASE OF CHOOSING THE BEST CHARACTER" << std::endl;

                    // from each sequence that starts with the best selected character in setOfStrings we
                    // remove that character from the beginning of that sequence
                    for(std::string &s : setOfStringsCpy)
                    {
                        if(s.starts_with(bestChar))
                        {
                            s.erase(s.begin());
                        }
                        // fResults << s << std::endl;
                    }

                }

                // fResults << "Temp Solution:" << solution << std::endl;
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

        void LAWMM_Beam_Search()
        {
            // BEGIN : algorithm initialization
            /* 
                solutions are pairs where first is solution string, second is pair where second.first is rank of solution and second.second is vector of positions is setOfStrings
            */
            std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *sls1 
                                                = new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>();

            std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *sls2 =
                                                 new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>();

            bool isSuperSequenceFound = false;

            std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> initialSolution
                        (        
                        std::string(1,alphabet[0]),
                        std::pair<size_t, std::vector<std::size_t>>(0,std::vector<std::size_t>(setOfStringsSize,0))
                        );
            std::vector<std::pair<char, std::size_t>> lr = LAWMM(initialSolution);

            for(auto &c : alphabet)
            {
                std::size_t weigthOfC;
                for(auto &l : lr)
                {
                    if(c == l.first)
                    {
                        weigthOfC = l.second;
                    }
                }

                std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> tmpSls
                    (
                        std::string(1,c), std::pair<size_t, std::vector<std::size_t>>(weigthOfC,std::vector<std::size_t>(setOfStringsSize,0))
                    );
                sls1->emplace_back(tmpSls);
            }

            sls1 = takeBetaSequances(sls1,isSuperSequenceFound);

            // END : algorithm initialization
            // fResults << "***START***" << std::endl;
            // main loop of Beam Search algorithms
            while(!isSuperSequenceFound)
            {
                for(auto &s : *sls1)
                {
                    // calculation of ranks of partial solutions
                    // fResults << "Ranks for seq:" << s.first << std::endl; 
                    std::vector<std::pair<char, std::size_t>> letterRanks = LAWMM(s);
                    /*
                     if all the ranks of the letters are 0 then there is a solution, actually if the rank of the first
                     letter is 0 it means that the others are also because they are sorted
                    */
                    if(letterRanks[0].second == 0)
                    {
                        std::string solution = s.first;
                        std::string postRes("");
                        for(int i = 0;i < setOfStringsSize;++i)
                        {
                            // inside the loop we remove the remaining letters from setOfStrings but without repetition
                            std::size_t posInSqs = s.second.second[i];
                            while(posInSqs < setOfStringsVec[i].size())
                            {
                                char postChar = setOfStringsVec[i][posInSqs];
                                if(std::find(postRes.begin(),postRes.end(),postChar) == postRes.end())
                                {
                                    postRes += postChar;
                                }

                                ++posInSqs;
                                // if we find a supersequence we terminate the loop execution earlyer
                                if(isCommonSupersequence(solution + postRes))
                                {
                                    solution += postRes;
                                    shortestCommonSupersequence = solution;
                                    shortestCommonSupersequenceLen = solution.length();
                                    isSuperSequenceFound = true;
                                    break;
                                }
                            }
                            if(isSuperSequenceFound)
                            {
                                break;
                            }
                        }
                    }

                    //we add characters from the alphabet to the fixed partial solution and assign a new rank based on the ranks of the letters
                    for(auto &c : alphabet)
                    {
                        std::size_t weigthOfC;
                        for(auto &l : letterRanks)
                        {
                            if(c == l.first)
                            {
                                weigthOfC = l.second;
                            }
                        }
                        std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> newSLS
                        (
                            std::string(s.first + c), std::pair<size_t, std::vector<std::size_t>>(s.second.first + weigthOfC, s.second.second)
                        );
                        sls2->emplace_back(newSLS);
                    }
                }
                
                // we choose the best beam size solutions from the newly created partial solutions
                delete sls1;
                sls1 = takeBetaSequances(sls2,isSuperSequenceFound);
                sls2 = new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>();       
            }
            

            if(sls1)
            {
                delete sls1;
            }

            if(sls2)
            {
                delete sls2;
            }

        }


        std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>* 
                                    takeBetaSequances(std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *sls,
                                                             bool &isSuperSequenceFound)
        {
            // if the number of partial solutions is less than the beam size, it means that we take all the solutions, but we have to calculate new positions for partial solutions
            if(sls->size() <= beamSize)
            {
                for(auto &s : *sls)
                {
                    updatePositions(s);
                    if(isCommonSupersequence(s.first))
                    {
                        shortestCommonSupersequence = s.first;
                        shortestCommonSupersequenceLen = s.first.length();
                        isSuperSequenceFound = true;
                    }
                }
                std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *slsNew =
                                         new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>(*sls);
                delete sls;
                return slsNew;
            }
            
            // we mix the elements before sorting to avoid same sort order if all elemts have same ranks (we do shuffle in LAWMM so it's not necessary here)
            auto rng = std::default_random_engine();
            rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
            std::shuffle(sls->begin(), sls->end(), rng);

            // we sort the partial solutions in relation to the sum of the ranks
            std::sort(sls->begin(), sls->end(), sortAsc);

            // fResults << "Sorted VALUES" << std::endl;
            // for(auto &s : *sls)
            // {
            //     fResults << s.first << "-" << s.second.first << std::endl;
            // }

            // we create a vector of new partial solutions
            std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>> *slsNew =
                                         new std::vector<std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>>>();
            
            // we choose the best beam size solutions from the newly created partial solutions
            for(int i = 0;i < beamSize;++i)
            {
                // a roulette selection can be added here
                std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> &elemRef = (*sls)[i];

                // updating positions of the partial solution
                updatePositions(elemRef);

                // if a partial solution is a solution to the problem, we stop the algorithm
                if(isCommonSupersequence(elemRef.first))
                {
                    shortestCommonSupersequence = elemRef.first;
                    shortestCommonSupersequenceLen = elemRef.first.length();
                    isSuperSequenceFound = true;
                }
                // fResults << "Nova izabrana sekvenca:" << (*sls)[i].first << std::endl;
                slsNew->emplace_back((*sls)[i]);
            }
            delete sls;
            sls = nullptr;
            return slsNew;
        }

        std::vector<std::pair<char, std::size_t>> LAWMM(std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> &s)
        {
            // contains all possible combinations
            std::vector<std::pair<std::string, std::size_t>> laWeightedCombinations;
            // contains only possible combinations
            std::vector<std::pair<std::string, std::size_t>> finalLaWeightedCombinations;

            // we make all possible combinations of letters from the alphabet of length lookAhead
            makeLACombinationsPair(std::string(""), 0, laWeightedCombinations);

            // 
            std::vector<std::pair<char, std::size_t>> letterRanks;
            for(auto &c : alphabet)
            {
                letterRanks.emplace_back(c,0);
            }

            for(std::pair<std::string, std::size_t> &comb: laWeightedCombinations)
            {
                std::vector<std::size_t> tmpPositions = s.second.second;
                std::string retSls("");

                for(char &c : comb.first)
                {
                    bool startWith = false;
                    for(int i = 0;i < setOfStringsVec.size();++i)
                    {
                        if(setOfStringsVec[i][tmpPositions[i]] == c && (tmpPositions[i] <= setOfStringsVec[i].size() - 1))
                        {
                            comb.second += (setOfStringsVec[i].size() - 1 - tmpPositions[i]);
                            tmpPositions[i] += 1;
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

                comb.first = retSls;
        

                if(comb.first != std::string("") && (std::find(finalLaWeightedCombinations.begin(),finalLaWeightedCombinations.end(),comb) == finalLaWeightedCombinations.end()))
                {
                    finalLaWeightedCombinations.emplace_back(comb);

                    char firstCharOfCombination = comb.first[0];
                    std::size_t weigthOfCombination = comb.second;

                    for(auto &lr : letterRanks)
                    {
                        if(lr.first == firstCharOfCombination && lr.second < weigthOfCombination)
                        {
                            lr.second = weigthOfCombination;
                        }
                    }
                } 
            }

            // we mix the elements so that 'a' would not always be selected if say a,c,t all have the same value
            auto rng = std::default_random_engine();
            rng.seed(std::chrono::system_clock::now().time_since_epoch().count());
            std::shuffle(letterRanks.begin(), letterRanks.end(), rng);

            std::sort(letterRanks.begin(), letterRanks.end(), sortRev2);

            std::sort(finalLaWeightedCombinations.begin(), finalLaWeightedCombinations.end(), sortRev3);

            // fResults << "LAWMM ranks for all lawmm combinations" << std::endl;            

            // for(auto &i : finalLaWeightedCombinations)
            // {
            //     fResults << i.first << "-" << i.second << std::endl;
            // }

            // fResults << "LAWMM rank latters" << std::endl;

            // for(auto &rank : letterRanks)
            // {
            //     fResults << rank.first << "-" << rank.second << std::endl;
            // }
            
            return letterRanks;
        }

        // updating positions for partial solution: We move in each sequence in setOfStrings for the last added letter in partial solution
        void updatePositions(std::pair<std::string, std::pair<size_t, std::vector<std::size_t>>> &s)
        {
            // fResults << "Updating positions for:" << s.first << std::endl;
            if(s.first.empty())
            {
                fResults << "EROOR UPDATING POSITIONS!!!" << std::endl;
            }

            char c = s.first.back();

            for(int i = 0;i < setOfStringsSize;++i)
            {
                if(setOfStringsVec[i][s.second.second[i]] == c)
                {
                    ++s.second.second[i];
                }
                // fResults << s.second.second[i] << " ";
            }
            // fResults << std::endl;
            // fResults << "END Updating positions for:" << s.first << std::endl;
        }

        void makeLACombinationsPair(const std::string &sls, 
                                const std::size_t &counter,
                                std::vector<std::pair<std::string, std::size_t>>  &lookAheadCombinations)
        {

            /*
                Example:    
                    Alphanet = {a,b}
                    lookAhead = 2
                    function return {[aa,0],[ab,0],[ba,0],[bb,0]}
            */

            if(counter == lookAhead)
            {
                lookAheadCombinations.emplace_back(std::pair(sls,0));
                return;
            }

        
            for(char c : alphabet)
            {
                makeLACombinationsPair(sls + c, counter + 1, lookAheadCombinations);
            }
        }




        // the sequence that represents the solution to the problem and its size 
        std::string shortestCommonSupersequence = std::string("");
        std::size_t shortestCommonSupersequenceLen = 0;

        // maximal and minimal depth to consider in algorithm
        std::size_t minDepth;
        std::size_t maxDepth;

        std::vector<char> alphabet;
        std::set<std::string> setOfStrings;
        std::vector<std::string> setOfStringsVec;
        std::size_t setOfStringsSize;

        // Beam size paramether of Beam Search algorithm
        const std::size_t beamSize = 300;

        // LAWMM lookAhead parameter
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

    // running different algorithms
    // alg.SCS_BFBT_Algorithm();
    alg.SCS_MM_Beam_Search_Algorithm();
    // alg.LAWMMAlgorithm();
    alg.SCS_LAWMM_Beam_Search_Algorithm();

    return EXIT_SUCCESS;

}