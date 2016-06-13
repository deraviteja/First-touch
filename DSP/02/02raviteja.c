// Name: RAVITEJA MEESALA
/*

string can be of the format:
1.<nonreal characters> <real constant> <nonreal characters>
2.<realconstant> <nonreal>
3.<non real> <real constant>
4.<real constant>
5.<non real>

case 1,3,5 are eliminated without any code based on ssacnf functionality

for differentiating 2 and 4:
used sscanf function to parse string and read a real value and a character value. Character value was initialized with a integer character. If there was any character along with the real number, data in that character variable changes to a non-integer character. Checking this condition, if variable has changed, would eliminate case 2. Leaving only valid entry case:4

Additionally, cases to eliminate, after valid 'case 4' entries are: 1.3e, 1.4e<math. operator>. These are eliminated by verifying the if the last character of the given string is a number or ends with a '.'. 

*/


#include<stdio.h>
double parseReal(char * TOKEN){
double val=0.0;
int correctFormat=0;

/* code starts here */
char c = '4';
char c1 = '4';
int j=0;

sscanf(TOKEN, " %lf %c", &val, &c);
if( (int)c == (int)c1)
{
correctFormat = 1;
}

if(correctFormat==1)
{
while((int)(*(TOKEN+j))!= 0)
{
sscanf(TOKEN+j, "%c",&c);

j++;
}  
if((((int) c < (int)'0')||((int) c >(int)'9'))&& ((int)c != (int)'.') )
correctFormat = 0;
}

/* code ends  here */


if(correctFormat==1){
sscanf(TOKEN,"%lf",&val);
return val;
}else{
return 0.0;
}
}
int main(int argc, char ** argv){
double sum=10.0;/*value 10.0 is intentional ~TA*/
int i;
if( argc >1){
for(i=1;i<argc;i++){
sum=sum+parseReal(argv[i]);
}
}
printf("%e\n",sum);
return 0;
}
