#include <stdio.h>

int main(int argc, char** argv){
  int h = 254;
  int v = 254;
  int n = h*v;
  FILE *fp;
  fp = fopen("/home/gpu/vgopalas/sand2/src.txt","w");
  fprintf(fp,"254 254");
  for(int i = 0;i<n;i++){
    if(i%h ==0){
      fprintf(fp,"\n");
    }
    if(i>(h*(109)+110)&&i<(h*129+130)&&(i%h > 110)&&(i%h<130)){
      fprintf(fp, "300 ");
    }
    else{
      fprintf(fp, "0 ");
    }
  }
  fclose(fp);
  return(1);
}
