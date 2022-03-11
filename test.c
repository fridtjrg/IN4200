#include <stdio.h>

void main()
{
  int dec = 5;
  char str[] = "abc";
  char ch = 's';
  float pi = 3.14;

  printf("%d %s %f %c\n", dec, str, pi,  ch);
}

/* gcc test.c -o somefile.exe/*