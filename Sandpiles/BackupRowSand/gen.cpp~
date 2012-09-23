#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
  int h = atoi(argv[1]);
  int v = atoi(argv[2]);
  int val = atoi(argv[3]);
  int n = h*v;
  char* s =(char*)malloc(20*sizeof(char));
  sprintf(s,"%d %d",h,v);
  FILE *fp;
  char* sval =(char*)malloc(10*sizeof(char));
  sprintf(sval,"%d ",val);
  char* name =(char*)malloc(100*sizeof(char));
  sprintf(name,"/home/gpu/vgopalas/sand3/src%d.txt",n);
  fp = fopen(name,"w");
  fprintf(fp,s);
  for(int i = 0;i<n;i++){
    if(i%h ==0){
      fprintf(fp,"\n");
    }
    if(i == (h*(v/2)+h/2)){
      fprintf(fp,sval);
    }
    else{
      fprintf(fp, "0 ");
    }
  }
  fclose(fp);
  return(1);
}
