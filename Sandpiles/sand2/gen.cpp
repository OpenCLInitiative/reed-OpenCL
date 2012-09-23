#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
  int h = atoi(argv[1]);
  int v = h;
  int n = h*v;
  int l_frac = 4*(h/10);
  int u_frac = 6*(h/10);
  int v_lower = h*(l_frac)+l_frac;  
  int v_higher = h*(u_frac)+u_frac;
  int h_lower = l_frac;
  int h_higher = u_frac;
  FILE *fp;
  fp = fopen("/home/gpu/vgopalas/sand2/src.txt","w");
  fprintf(fp,"254 254");
  for(int i = 0;i<n;i++){
    if(i%h ==0){
      fprintf(fp,"\n");
    }
    if(i>v_lower&&i<v_higher&& i%h >h_lower && i%h<h_higher){
      fprintf(fp, "300 ");
    }
    else{
      fprintf(fp, "0 ");
    }
  }
  fclose(fp);
  return(1);
}
