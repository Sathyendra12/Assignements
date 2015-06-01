#include <stdio.h>
#include <string.h>
struct data{
  int num1;
  int num2;
  int num3;
};
typedef struct data data;

int main(int argc, char** argv)
{
   data my_data;
   my_data.num1 = 64;
   my_data.num2 = 65;
   my_data.num3 = 66;

   char buffer[200];
   buffer[0]='a';
   memcpy(&buffer[1], &my_data, sizeof(data));
   
        
   data copy_data;
   printf ("1st: %c",buffer[0]);
   memcpy(&copy_data, &buffer[1], sizeof(data));

   printf("the numbers : %d - %d - %d \n", copy_data.num1, copy_data.num2, copy_data.num3);

   return 0;
}