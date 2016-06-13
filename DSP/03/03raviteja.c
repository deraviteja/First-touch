//Name: RAVITEJA MEESALA
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
typedef struct t_complex{
double real,imaginary;
}Complex;
Complex* createComplex(double x,double y){
Complex * C;
C=malloc(sizeof(Complex));
C->real=x;
C->imaginary=y;
return C;
}
int deleteComplex(Complex *C){
free(C);
return 1;
}
Complex* add(Complex * A, Complex* B){
Complex *C=createComplex(A->real+B->real,A->imaginary+B->imaginary);
return C;
}
Complex* substract(Complex * A, Complex* B){
Complex *C=createComplex(A->real-B->real,A->imaginary-B->imaginary);
return C;
}
Complex* multiply(Complex * A, Complex* B){
Complex *C=createComplex((A->real*B->real)-(A->imaginary*B->imaginary) ,(A->imaginary*B->real)+(A->real*B->imaginary));
return C;
}
Complex* divide(Complex * A, Complex* B){
double c = pow(B->real,2.0) + pow(B->imaginary,2.0);

Complex *C=createComplex(((A->real*B->real)+(A->imaginary*B->imaginary))/c ,((A->imaginary*B->real)-(A->real*B->imaginary))/c);
   return C;



}
Complex* exponentiate(Complex * A, Complex* B){
double a1,a2,b1,b2,r,teta;
double t1,t2;

/*Calculated using the polar form of a complex number */

a1 = A->real;
a2 = A->imaginary;
b1 = B->real;
b2 = B->imaginary;

Complex *c = createComplex(0.0,0.0);

if(a1 || a2)
{

 r = pow( (pow( a1,2.0) + pow(a2,2.0)), 0.5);
 teta = acos(a1/r);

  if(a2<0)
    teta =  - teta ; /* range of teta is kept from -pi to + pi */

Complex *c1 = createComplex(cos(b2*log(r)),sin(b2*log(r)));
Complex *c2 = createComplex(cos(teta*b1),sin(teta*b1));
c = multiply(c1,c2);
c->real  = c->real* pow(r,b1)*exp(-teta*b2); 
c->imaginary  = c->imaginary* pow(r,b1)*exp(-teta*b2);

return c; 
}

return c;
}
int printComplex(Complex *C){
//Dont change this function..Your output depends on this funcion
printf("%g ",C->real);
if(C->imaginary<0.0){
printf("- i%g",-C->imaginary);
}else{
printf("+ i%g",C->imaginary);
}
}
int main(){
Complex *temp, *C;
char c='s';
double x,y;
C=createComplex(0.0,0.0);
while(c!='q'){
scanf("%c %lf %lf",&c,&x,&y);
/* Something New !!
try changing the previous line of code to ‘‘scanf("%c %lf %lf",&c,&x,&y);‘‘
The space before %c has been removed> and run the program. Why is the
program behaving in this way when we remove the space? What is the
significance of that space ? Try to find out..!!
*/
switch(c){
case '+':
temp=createComplex(x,y);
C=add(C,temp);
break;
case '-':
temp=createComplex(x,y);
C=substract(C,temp);
break;
case '*':
temp=createComplex(x,y);
C=multiply(C,temp);
break;
case '/':
temp=createComplex(x,y);
C=divide(C,temp);
break;
case '^':
temp=createComplex(x,y);
C=exponentiate(C,temp);
break;
case 'q':
break;
}
}
printComplex(C);
deleteComplex(C);
deleteComplex(temp);
return 0;
// Important to avoid memory leaks
// Important to avoid memory leaks
}
