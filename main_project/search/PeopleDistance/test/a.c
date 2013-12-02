#include "stdio.h"
#include "stdlib.h"
int foo(){
	char * fname = "./a.txt";
	FILE * fp;
	if ((fp = fopen(fname, "r")) == NULL) {
		printf("Cannot open file %s\n " , fname);
		return 0;
	}  
	int idin = 0; 
	int uid;
	float loc[10];	
	while(	fscanf(fp,"%d %f %f %f %f %f %f %f %f %f %f\n",&uid, 
				&loc[0], &loc[1], &loc[2], &loc[3], &loc[4],
				&loc[5], &loc[6], &loc[7], &loc[8], &loc[9]) != EOF){
		printf("idin:%d\n",idin);
		printf("uid:%d\n",uid);
		int i;
		for(i = 0; i < 10 ;++i){
			printf("idx:%d %f ",i,loc[i]);		
		}
		printf("\n");
		++idin;
	}
	fclose(fp);
}
int main(){
	foo();
	return 1;
}

