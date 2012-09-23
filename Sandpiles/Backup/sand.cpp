#include <reedGPU.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

off_t fsize(const char *file){
  struct stat st;
  if (stat(file,&st) == 0){
    return st.st_size;
  }
}
int* file_extractor(char *file_name, int* h, int* v){
  FILE *file;
  int sz = (int)fsize(file_name);
  int* s = (int*)malloc(sz*sizeof(int));
  file = fopen(file_name, "r");
  int z = 0;
  int x;
  fscanf(file, "%d", h);
  fscanf(file, "%d", v);
  
  while(fscanf(file,"%d",&x) != EOF){
    s[z]=x;
    z++;
    
  }
  fclose(file);
  return(s);

}

void ppm(int* src, int h, int v, int n, int r, char* s){
  FILE* fp;
  char* fn = (char*)malloc(100);
  sprintf(fn,"%s%03d.ppm",s,r);
  fp = fopen(fn, "w");
  fprintf(fp,"P3\n%d %d\n256",h,v);
  for(int i =0;i<n;i++){
    if(i%60 == 0){
      fprintf(fp,"\n");
    }
    if(src[i]==0){
      fprintf(fp,"0 0 0   ");
    }
    else{ if(src[i]==1){
	fprintf(fp,"256 0 0   ");
      }
      else{ if(src[i]==2){
	  fprintf(fp,"0 256 0   ");
	}
	else{ if(src[i]==3){
	  fprintf(fp,"0 0 256   ");
	}
	  
	    }}}}
  fclose(fp);
}
	
int main(int argc, char **argv){

  if (argc < 3){
    printf("Usage: %s <Reps> <Source File> ", argv[0]);
    exit(-1);
  }

  long time = 0;
  int reps = atoi(argv[1]);
  int graph_h;
  int graph_v;
  int* sand_i = file_extractor(argv[2],&graph_h,&graph_v);
  int graph_n = graph_v*graph_h;
  
  rGcontext context;
  rGinit(&context);
  rGload(&context,"sand.cl");
  
  rGmem dev_src;
  rGmem dev_dst;

  int *graph_src = (int *)rGmalloc(&context, "I", sizeof(cl_int)*graph_n,&dev_src);
  int *graph_dst = (int *)rGmalloc(&context, "O", sizeof(cl_int)*graph_n,&dev_dst);
  for(int u = 0; u <graph_n;u++){
    graph_src[u]=sand_i[u];
  }
  for(int j = 0; j<reps; j++){

  rGinitX(&context);
  printf("\n");

  rGnewX(&context,"run");
  rGargXmem(&context,dev_src);
  rGargXmem(&context,dev_dst);
  rGargXint(&context,graph_v);
  rGargXint(&context,graph_h);
  
  rGXid x = rGsubmitX(&context,graph_n,16);

  rGwaitallX(&context);
  
  time += rGgettimeX(&context,x);
  for(int z = 0;z < graph_n;z++){
    graph_src[z]=graph_dst[z];
  }
  ppm(graph_src,graph_h,graph_v,graph_n,j,argv[0]);
  }

  printf("%s \t%lu\n",argv[0],time);
}
