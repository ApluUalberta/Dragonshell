#include <stdio.h>
#include <time.h>

int main ()
{
long seconds = time(NULL), timeUp = seconds + 10;
printf("\n");
while(seconds < timeUp)
{
/* SPACEBAR COUNTER GOES HERE */
seconds = time(NULL);
}

return 0;
}