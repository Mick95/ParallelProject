#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
using namespace std; 

float noise;
float chi_c;
float chi_0;
float c;

using namespace std;

int main (){
	float t;
	string str = "1";
	srand((unsigned) time(NULL));
	noise = rand()%400 + 800;
	noise =	noise / 1000;
	
	
	while(str != "no"){
		printf("\n Execute Time Estimation? YES or NO\n");
		getline(std::cin,str);
		
		if(str == "yes"){
			printf("\n Xc value: ");
			scanf("%f",&chi_c);
			
				fflush(stdin);
			
			printf("\n X0 value: ");
			scanf("%f",&chi_0);
			
				fflush(stdin);
			
			printf("\n c value: ");
			scanf("%f",&c);
			
				fflush(stdin);
				
			t=chi_c/c+chi_0;
			t=t*noise;
			
			printf("\n Execution time is : %f" , t);
				
		}
		
		
		
	}
	
} 
