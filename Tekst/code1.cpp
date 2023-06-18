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