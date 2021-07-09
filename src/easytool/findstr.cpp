#include "findstr.h"




FindStr::FindStr()
{
    
}

void FindStr::config(unsigned char *str,int len)
{
    this->str = str;
    this->len = len;
}

void FindStr::reset(void)
{
    index=0;
}

int FindStr::input(unsigned char *data,int len)
{
    index = 0;
    for(int i=0;i<len;i++)
    {
        if(data[i] == str[index])
        {
            //DBG("find %c %d %d",str[index],index,this->len);
            index++;
            if(index >= this->len)
            {
                return i+1;
            }
        }else{
            index = 0;
            if(data[i] == str[index])
            {
                //DBG("find %c %d %d",str[index],index,this->len);
                index++;
                if(index >= this->len)
                {
                    return i+1;
                }
            }
        }
    }
    
    return 0;
}
