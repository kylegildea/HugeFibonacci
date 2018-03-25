//------------------------------------------------------------------
// University of Central Florida
// COP3502C - Fall 2016
// Program Author: Kyle Gildea - PID 4163278
//------------------------------------------------------------------

#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Fibonacci.h"

#define LN10 2.3025850929940456840179914546844 //Natural Log of 10 to a buncha decimal places, because GCC is stupid and wont let me use math.h and I needed a workaround


//Function Prototypes
char *itoa(unsigned int num);
char *strrev(char *str);
int pow(int base, int exponant);
int compare(double f1, double f2);
double log10(double num);
double ln(double num);
//Prints error string
void errorPrint(char *error); 

void errorPrint(char *error) 
{ 
  fprintf(stderr, "%s", error);
}


HugeInteger *hugeAdd(HugeInteger *p, HugeInteger *q)
{
  HugeInteger *sum; //HI for the sum of p & q
  
  int carry = 0;//carry digit, because sometimes math goes over 10
  int i;
  
  if(p == NULL || q == NULL) //returns NULL if P & Q are null
    return NULL;
    
  sum = malloc(sizeof(HugeInteger));
  if (sum == NULL)
  {    
    fprintf(stderr, "a malloc failed\n");   
    return(-1); 
  }
  sum->digits = malloc((p->length + 1) * sizeof(int)); //+1 in case there's digit overflow at the end
  if (sum->digits == NULL)
  {    
    fprintf(stderr, "b malloc failed\n");   
    return(-1); 
  }
  
  if (q->length >= p->length)//Flips p & q pointers if necessary so p is always the larger number
  {
    HugeInteger *temp = p;
    p = q;
    q = temp;
  }
  
  for(i = 0; i < q->length; i++)
  {
    if(p->digits[i] + q->digits[i] + carry < 10)//adds p + q + carry digit. if the result is less than 10, sets carry digit for next addition to zero
    {
      sum->digits[i] = p->digits[i] + q->digits[i] + carry;
      carry = 0;
    }
      
    else
    {
      sum->digits[i] = (p->digits[i] + q->digits[i] + carry) - 10;//adds p + q + carry digit. if the result is above 10, sets carry digit to 1 for next additon and just adds ones place
      carry = 1;
    }
  }

  if(p->length >= q->length)//Because p might be > q or there might be a final carry digit, continues past the length of q with additon stuff
  { 
    for(i; i < p->length; i++)
    {
      if(p->digits[i] + carry < 10)//same deal but no q
      {
        sum->digits[i] = p->digits[i] + carry;
        carry = 0;
      }
      
      else
      {
        sum->digits[i] = p->digits[i] + carry - 10; //same deal but no q
        carry = 1;
      }
    }
  
    if(carry == 1)//if there's  final carry digit, sets sum length to p+1
    {
      sum->length = (p->length) + 1;
      sum->digits[i] = carry;
    }
    else//otherwise sums length = p's length
      sum->length = p->length;
  }
    
  return sum;
}

HugeInteger *hugeDestroyer(HugeInteger *p)
{
  if(p != NULL && p->digits != NULL)//if both p and p.digits aren't null, frees both
  {
    free(p->digits);
    free(p);
  }
  
}

HugeInteger *parseString(char *str)
{
  HugeInteger *huge;
  char *hiDigits;
  
  if(str == NULL)
    return NULL;//returns null if the stirng is null
  
  
  else if(strlen(str) == 0)//treats an empty string as 0
  {
    huge = malloc(sizeof(HugeInteger));
    if (huge == NULL)
    {    
      fprintf(stderr, "c malloc failed\n");   
      return(-1); 
    }
    huge->length = 0;
    huge->digits = malloc(sizeof(int));
    if (huge->digits == NULL)
    {    
      fprintf(stderr, "d malloc failed\n");   
      return(-1); 
    }
    huge->digits[0] = 0;
  }
    
  
  else
  {
    huge = malloc(sizeof(HugeInteger));
    if (huge == NULL)
    {    
      fprintf(stderr, "g malloc failed\n");   
      return(-1); 
    }
    huge->length = strlen(str);//sets length of hugeinteger to length of string
    huge->digits = malloc((huge->length) * sizeof(int));//uses length attribute to malloc size of p
    if (huge->digits == NULL)
    {    
      fprintf(stderr, "e malloc failed\n");
      return(-1); 
    }
    
    hiDigits = strrev(str);//reverses characters in string
    
    for(int i = 0; i < huge->length; i++)
    {
      huge->digits[i] = (int)(hiDigits[i] - 48);//ASCII 48 is 0.
    }
    free(hiDigits);
  }
  return huge;
}

HugeInteger *parseInt(unsigned int n)
{
  HugeInteger *huge;
  char *str = itoa(n);//integer to string written below
  char *hiDigits;
  
  huge = malloc(sizeof(HugeInteger));
  if (huge == NULL)
  {    
    fprintf(stderr, "h malloc failed\n");   
      return(-1); 
  }
  
  if(n == 0)
    huge->length = 1;
  else
  {
    huge->length = strlen(str);//sets length of hugeinteger with string length
  }
  
  huge->digits = malloc((huge->length) * sizeof(int));
  if (huge->digits == NULL)
  {    
    fprintf(stderr, "f malloc failed\n");   
    return(-1); 
  }
  
  hiDigits = strrev(str);//flips characters in string
  
  for(int i = 0; i < huge->length; i++)
  {
      huge->digits[i] = (int)(hiDigits[i] - 48);//ASCII 48 is 0
  }
  free(hiDigits);
  free(str);
  return huge;
}

unsigned int *toUnsignedInt(HugeInteger *p)
{
  unsigned int num = 0;
  unsigned int *ptr;
  int base = 10;
  int i = 0;
  
  ptr = malloc(sizeof(unsigned int));
  if (ptr == NULL)
  {    
    fprintf(stderr, "g malloc failed\n");   
    return(-1); 
  }
  
  int currPlace = 1;
  
  if(p == NULL)
  {
    free(ptr);
    return NULL;
  }
    
  for(i =0; i < p->length; i++)//Goes through array and multiples number in that place in the array by 10^i. if i is 0, just adds the number to the unsigned int version of the huge integer
  {
    if(i == 0)
      num+= (unsigned int)p->digits[i];
      
    else
    {
      if(p->digits[i] == 0)//if digit is a 0 adds the 10 in that place to num
        num+= pow(base, i);
      else        
       currPlace = pow(base, i);//otherwise sets the current place (ones, tens, hundreds, whatever) 
       
      if(UINT_MAX - num < (unsigned int)(currPlace * p->digits[i]))//if there's not enough room left to add the next digit, and there'll be overflow returns null
         return NULL;
       else//otherwise adds the number in its proper place by multiplying digit by 10^i and adding it to the total. this probably could have been simplified but i kept screwing it up and finally just broke it apart to make it easier to read
         num+= (unsigned int)(currPlace * p->digits[i]);
    }
  }
  *ptr = num;
  return ptr;  
}

HugeInteger *fib(int n)
{
  int i = 0;
  
  HugeInteger *first = parseInt(0);//F(0)
  HugeInteger *second = parseInt(1);//F(1)
  HugeInteger *sum;
  
  if(n <= 1)//if n is 0 or 1 just returns HugeInteger 0 or 1 because speed is important
  {
    hugeDestroyer(first);
    hugeDestroyer(second);
    return parseInt(n);
    
  }
  else//Otherwise, adds the numbers, destroys the first number since we dont need it anymore, points first to what used to be second and second to sum which is our new total
  {
    for(i = 2; i <= n; i++)
    {
      sum = hugeAdd(first, second);
      hugeDestroyer(first);
      first = second;
      second = sum;
    }
  }
  return sum;
}

double difficultyRating(void)
{
  return 3.5;
}

double hoursSpent(void)
{
  return 10.0; //includes cigarette breaks + time spent watching football with file open
}

char *strrev(char *str)//String reversal function. Reverses strings
{
  char *flippedString;
  
  flippedString = malloc(strlen(str) * sizeof(int));
  
  int i = 0, j = strlen(str) - 1;
  
  for(i, j; i <= strlen(str); i++, j--)
  {
    flippedString[i] = str[j];
  }
  
  flippedString[strlen(str)] = '\0';
  
  return flippedString; 
}

int pow(int base, int exponant)//Poorly written exponent math function because math.h requires -lm flag
{
  if(exponant == 0)
    return 1;
  else
  {
    int total = base;
    for(int i = 1; i < exponant; i++)
    {
      total*=base;
    }
    return total;
  }
}

char *itoa(unsigned int num)//converts into to string by using log10 to determine string length and sprintf to print unsigned int to character buffer
{ 
  int size;
    if(num !=0)
    {
      size = log10(num);
    }
    else
    {
      size = 1;
    }
    
    char *str = malloc(size);
    sprintf(str, "%u", num);
    
    return str;
}

double ln(double num)//calculates natural log of number
{
    double old_sum = 0.0;
    double xmlxpl = (num - 1) / (num + 1);
    double xmlxpl_2 = xmlxpl * xmlxpl;
    double denom = 1.0;
    double frac = xmlxpl;
    double term = frac;
    double sum = term;

    while (!compare(sum, old_sum))
    {
        old_sum = sum;
        denom += 2.0;
        frac *= xmlxpl_2;
        sum += frac / denom;
    }
    return 2.0 * sum;
}

double log10(double num)//uses log rules to get log10. again, because math.h is broken
{
    return ln(num) / LN10;    
}

int compare(double f1, double f2)//double comparison for comparing sums as part of ln function
 {
  float precision = 0.00001;
  if (((f1 - precision) < f2) && 
      ((f1 + precision) > f2))
   {
    return 1;
   }
  else
   {
    return 0;
   }
 }