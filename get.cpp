#ifndef GET
#define GET
#include "get.h"


std::vector<std::string> getDict(Dict& dict)
{
    std::fstream df;
    df.open(DICT_FILE);
    if(df.is_open())
    {
        std::string word;
        while(std::getline(df,word))
            dict.push_back(word);
    }
    else
        throw 1;
    df.close();

    return dict;
}


unsigned int getWord1(Dict& dict)
{
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<> rnd{0, (int)dict.size()};

    return (unsigned int)rnd(gen);
}

unsigned int getWord2(Dict& dict, unsigned int& mean, unsigned int disp)
{
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::normal_distribution<> rnd{(double)mean,(double)disp};

    return (unsigned int)std::min((int)dict.size(),std::max(0,(int)rnd(gen)));
}

double* getDistr(int* word_answ, size_t size)
{
    if(not WINDOW)
        throw 1;
    int w(0),r(0);
    double* distr = new double[size];
    for(long long i(0); i < WINDOW; ++i)
    {
        if(word_answ[i] == 0)
            ++w;
        else if(word_answ[i] == 1)
            ++r;
    }
    distr[0] = (double)r/double(w+r);

    size_t l(0),rt(WINDOW-1);
    for(long long i(1); i < size; ++i)
    {
        if(std::max((long long)0,i-WINDOW) != l)
        {
            if(word_answ[l] == 1)
                --r;
            else if(word_answ[l] == 0)
                --w;
            ++l;
        }
        if(std::min((long long)size-1,i+WINDOW) != rt)
        {
            ++rt;
            if(word_answ[rt] == 1)
                ++r;
            else if(word_answ[rt] == 0)
                ++w;
        }
        distr[i] = (double)r/double(w+r+EPS);
    }

    return distr;
}

int getVocabSize(double* distr, size_t size)
{
    double res(0);

    for(size_t i(0); i < size; ++i)
        res += distr[i];
    
    return (int)res / 100 * 100;
}

#endif