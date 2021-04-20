#include "get.h"

int main()
{
    Dict dict;
    getDict(dict);

    unsigned int wrong(0),right(0),mean(0),disp(100);
    int* word_answ = new int[dict.size()];

    for(int i(0); i < dict.size(); ++i)
        word_answ[i] = -1;

    unsigned int count(0),word_ind;
    
    while(true)
    {
        if(count > 100)
            break;
        if(count < PHASE_PAR)
            word_ind = getWord1(dict);
        else if(count > PHASE_PAR)
            word_ind = getWord2(dict,mean,disp);
        else
        {
            double* distr = getDistr(word_answ,dict.size());
            double* distr_smooth = smooth(distr,dict.size(),SMOOTH_WINDOW);
            
            int mn(100000);
            long l(0),r(0);
            for(long i(0); i < dict.size(); ++i)
            {
                if(distr_smooth[i] < UPPER_BND and i < mn)
                {
                    l = i;
                    mn = i;
                }
                else if(distr_smooth[i] < LOWER_BND)
                {
                    r = i;
                    break;
                }
            }
            if(std::abs(l-r) < 10)
            {
                std::cerr << "Whether your answers was not real or your vocabulary is very large" << '\n';
                throw 1;
            }
            else if(l > r)
            {
                std::cerr << "Please, give real answers!" << '\n';
                throw 1;
            }
            mean = (l+r)/2;
            disp = mean-l;
            std::cout << mean << ' ' << disp << '\n';
            ++count;
            continue;
        }
        std::string word(dict[word_ind]),answ;
        std::cout << "Do you know word '" << word << "'?(Y/n)" << '\n';
        std::cin >> answ;
        if(answ == "N" or answ == "n")
        {
            ++wrong;
            word_answ[word_ind] = 0;
        }
        else
        {
            ++right;
            word_answ[word_ind] = 1;
        }
        ++count;
        std::cout << count << '\n';
    }

    std::cout << '\n' << "Your vocabulary size is: " << getVocabSize(getDistr(word_answ,dict.size()),dict.size());

    return 0;
}