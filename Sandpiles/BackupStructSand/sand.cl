typedef struct {
       __local int* pq;
       __local int index;
       __local int rindex;
} container;

typedef struct {
	int up;
	int down;
	int right;
	int left;
	int val;
	int checked;
} sand;

int pos(int i, int j, int h){
    return(i+ j*h);
    
}
int up(int p, int v){
    int s = p - v;
    return(s);
}
int down(int p, int v){
    int s = p +v;
    return(s);
}
int left(int p, int v){
    int s = p - 1;
 return(s);
}
int right(int p, int v){
    int s = p + 1;
    return(s);
}

void ping(__global sand* src, __local container* data,int v){
     if(data->rindex == data->index){
     }
     else{
     int i = data->rindex;
     data->rindex ++;
     src[i].val -= 4;
     src[up(i,v)].down = 1;
     src[down(i,v)].up = 1;
     src[left(i,v)].right = 1;
     src[right(i,v)].left = 1;
     }
}
void add(__global sand* src, __global sand* dst, int lpos){
          if(src[lpos].checked==1){
	  }
	  else{
	  dst[lpos].val  = src[lpos].val + src[lpos].up + src[lpos].down + src[lpos].right + src[lpos].left;
	  }
}
void rect_helper(__global sand *src, __global sand *dst, int v, int h,__local container* data){
 	int ly = get_local_id(1);
	int lx = get_local_id(0);
	int lsx = get_local_size(0);
	int lsy = get_local_size(1);
	int lpos = pos(lx,ly,lsx);
	if(src[lpos].val >=4){
	        data->pq[data->index]=lpos;
		data->index++;
	}
	else{}
	src[lpos].checked = 0;
	for(int i = 0; i < 3; i++){
		ping(src,data,lsy);
		barrier(CLK_GLOBAL_MEM_FENCE);
                data->rindex = 0;
		add(src,dst, lpos);
	}
}
	
__kernel void run(__global sand *s, __global sand *d, int v, int h){
	 __local static int pq[256]; 
	 __local container data;
	 data.index = 0;
	 data.pq = pq;
	 data.rindex = 0;
	 rect_helper(s,d,v,h,&data);
	 
	 
}
		