#include <shared.h>

void itoh(int a, char *b)
{
    int i, i1, tmp;
  char c;
  
  if (a==0) {
      b[0]='0';
      b[1]='x';
      b[2]='0';
      b[3]=0;
      return;
  }
  
  i=0;
  while (a>0)
  {
    tmp=(a%16);
    if (tmp<10) b[i]=tmp+'0';
    else b[i]=tmp+'a';
    a=a/16;
    i++;
  }

  b[i++]='x';
  b[i++]='0';
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}
