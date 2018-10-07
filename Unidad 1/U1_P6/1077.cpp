#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

void conjetura_collatz(int i,int j){
	int max=0;
	for(int k=i;k<=j;k++){
		int c=1;
		int n=k;
		while(n!=1){
			if(n%2==0)// Par
				n=n/2;
			else
				n=(3*n)+1;
			c++;
		}
		if(c>max)max=c;
	}
	printf("%d %d %d\n",i,j,max);
}

int main() {
	int i,j;
	while(cin >> i >> j){
		conjetura_collatz(i,j);
	}
	return 0;
}




