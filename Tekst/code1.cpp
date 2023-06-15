setScsMinDepth();
setScsMaxDepth();
shortestCommonSupersequenceLen = maxDepth + 1;

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
