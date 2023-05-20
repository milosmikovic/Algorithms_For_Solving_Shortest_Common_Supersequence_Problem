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
            shortestCommonSupersequenceLen = maxDepth;

            Shortest_Common_SupersequenceBS();

            fResults << "-Beam Search solution-" << std::endl;
            fResults << "SCS : " + shortestCommonSupersequence << std::endl;
            fResults << "SCS length : " << shortestCommonSupersequenceLen << std::endl;

            time(&end);
            measureTime(start, end);
            fResults << "------------------" << std::endl;
        }

        void WMMAlgorithm()
        {
            printAlphabetAndSequences();
            fResults << "------------------" << std::endl;
            fResults << "ProbabilisticBS_Algorithm" << std::endl;
            time_t start, end;
            time(&start);

            setScsMinDepth();
            setScsMaxDepth();

            // +1 to cover trivial soultions
            shortestCommonSupersequenceLen = maxDepth + 1;

            fResults << "---List of solutions---" << std::endl;
            WMMCONTROL();

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
            // WMMCONTROL();
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
                    sls2 = takeBetaSequancesGreedyApproach(sls2, isSuperSequenceFound);
                }

                sls1->clear();
                std::vector<std::string> *tmpSls = sls1;
                sls1 = sls2; 
                sls2 = tmpSls;        
            }

            delete sls1;
            delete sls2;
        }
        
        std::vector<std::string>* takeBetaSequancesGreedyApproach(std::vector<std::string> *sls, bool &isSuperSequenceFound)
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

            std::sort(slsCnt.begin(),slsCnt.end(),sortRev1);

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


        void LAWMMCONTROL()
        {
            std::string solution("");
            std::vector<std::string>setOfStringsCpy;
            for(std::string s : setOfStrings)
            {
                setOfStringsCpy.emplace_back(s);
            }

            bool end = false;
            std::size_t emptyStringsCounter;
            while(!end)
            {
                // ideja je da napravis kombinacije velicine lookAhead a,b,c ili aa,ab,ac,ba,bb,bc,ca,cb,cc...
                // zatim napravis WMM algoritam koji skida pocetno slovo sa date kombinacije i racuna tezinu,
                // za ab skida prvo a, racuna tezinu i onda skida b i racuna tezinu i vrati tezinu za ab ...
                // zatim se sve to popakuje u neku strukturu, sortira se i dobiju se rankovi bira se resenje sa najvecim rankom(tezinom)
                // skida se pocetno slovo tog resenja sa setOfStrings i nastavlja se proces dok se setOfStrings ne isprazni...
                fResults << "BEGIN:FAZA ODABIRA NAJBOLJEG KARAKTERA" << std::endl;
                std::vector<std::pair<std::string, std::size_t>> vecSls;
                std::vector<std::string> lookAheadCombinations;
                makeLACombinations(std::string(""), 0, lookAheadCombinations);
                for(std::string &s : lookAheadCombinations)
                {
                    std::pair<std::string, std::size_t> sls = WMMForSolution(setOfStringsCpy, s);
                    vecSls.emplace_back(sls);
                }
                std::sort(vecSls.begin(), vecSls.end(), sortRev3);
                for(auto s : vecSls)
                {
                    fResults << s.first << "-" << s.second << std::endl;
                }
                char bestChar = vecSls[0].first[0];
                solution += bestChar;
                std::size_t bestWeigth = vecSls[0].second;
                fResults << "Best Char To Remove:" << bestChar << std::endl;
                fResults << "END:FAZA ODABIRA NAJBOLJEG KARAKTERA" << std::endl;

                for(std::string &s : setOfStringsCpy)
                {
                    if(s.starts_with(bestChar))
                    {
                        s.erase(s.begin());
                    }
                    fResults << s << std::endl;
                    if(s.empty())
                    {
                        ++emptyStringsCounter;
                    }
                }

                if(bestWeigth == 0)
                {
                    std::string postRes("");
                    for(std::string &s : setOfStringsCpy)
                    {
                        // pri nadovezivanju karaktera koji su preostali (setOfStringsCpy je prazan ili ima stringova sa po jednim karakterom)
                        // treba da nadovezemo preostale karaktere ali bez ponavljanja zato ide deo sa && u if-u ("","a","a","")--> nadovezati
                        // a ali samo jedanput
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

            shortestCommonSupersequence = solution;
            shortestCommonSupersequenceLen = solution.length();
        }

        // NAPRAVI SAMO VALIDNE KOMBINACIJE VELICINE MAX lookAhead
        void makeLACombinations(const std::string &sls, 
                                const std::size_t &counter,
                                std::vector<std::string>  &lookAheadCombinations)
        {
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

        void WMMCONTROL()
        {
            std::string solution("");
            std::vector<std::string>setOfStringsCpy;
            for(std::string s : setOfStrings)
            {
                setOfStringsCpy.emplace_back(s);
            }

            bool end = false;
            std::size_t emptyStringsCounter;
            while(!end)
            {
                emptyStringsCounter = 0;
                std::vector<std::pair<char, std::size_t>> res = WMM(setOfStringsCpy);
                char bestChar = res[0].first;
                // bitno ako je najbolja tezina 0 znaci da su reci iz seta ili prazne ili imaju po jedan karakter,
                // u tom slucaju samo nadovezemo preostale karaktere na solution
                std::size_t bestWeigth = res[0].second;
                solution += bestChar;
                for(std::string &s : setOfStringsCpy)
                {
                    if(s.starts_with(bestChar))
                    {
                        s.erase(s.begin());
                    }
                    fResults << s << std::endl;
                    if(s.empty())
                    {
                        ++emptyStringsCounter;
                    }
                }
                if(bestWeigth == 0)
                {
                    std::string postRes("");
                    for(std::string &s : setOfStringsCpy)
                    {
                        // pri nadovezivanju karaktera koji su preostali (setOfStringsCpy je prazan ili ima stringova sa po jednim karakterom)
                        // treba da nadovezemo preostale karaktere ali bez ponavljanja zato ide deo sa && u if-u ("","a","a","")--> nadovezati
                        // a ali samo jedanput
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
            shortestCommonSupersequence = solution;
            shortestCommonSupersequenceLen = solution.length();
        }


        // treba mi wmm koji za datu azbuku (skracenu ili celu) daje slovo koje je najbolje za jedan wmm prolaz
        // posle ovo iskoristiti kada zatreba vise puta za lawmm

        std::vector<std::pair<char, std::size_t>> WMM(std::vector<std::string> &setOfStrings)
        {
            std::vector<std::pair<char, std::size_t>> res;
            for(char c : alphabet)
            {
                std::size_t weight = 0;
                for(std::string &s: setOfStrings)
                {
                    if(s.starts_with(c))
                    {
                        weight += (s.length() - 1);
                    }
                }
                res.emplace_back(std::pair(c,weight));
            }
            std::sort(res.begin(), res.end(), sortRev2);
            for(auto i : res)
            {
                fResults << i.first << " " << i.second << std::endl;
            }
            return res;
        }

        std::pair<std::string, std::size_t> WMMForSolution(std::vector<std::string> &setOfStrings, std::string &sls)
        {
            
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

        // Probabilistic Beam Search
        std::size_t lookAhead = 2;

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
    // alg.SCS_BS_Algorithm();
    alg.WMMAlgorithm();
    alg.LAWMMAlgorithm();

    return EXIT_SUCCESS;
}