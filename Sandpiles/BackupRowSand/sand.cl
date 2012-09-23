typedef struct {
       __local int* pq;
       __local int* index;
} container;
typedef struct{
	int val;
	int add;
}sand;
int pos(int i, int j, int h){
    int s = i+ j*h;
    return(s);
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
/*
int check(int gpos, int v, __global int* src){
    int add = 0;
    if(src[up(gpos,v)]>3){
                        add +=1;
                }
                if(src[down(gpos,v)]>3){
                        add +=1;
                }
                if(src[left(gpos,v)]>3){
                        add +=1;
                }
                if(src[right(gpos,v)]>3){
                        add +=1;
                }
                if(src[gpos]>3){
                        add -=4;
                }
		return(add);
}

void rect_helper(__global int *src, __global int *dst, int v, int h){
        
	 int gx = get_global_id(0);
	 int gy = get_global_id(1);
	 int add = 0;
	 int gpos = pos(gx,gy,h);
	 if(src[gpos]>=0){
	 add = check(gpos,v,src);
	}
	barrier(CLK_GLOBAL_MEM_FENCE);
	dst[gpos]= src[gpos] + add;
}

void addSurrounding(__private container* data, __global int* src, int gpos, int v,int up, int down, int left, int right){
     int nIndex = data->index+1;
     int flag = 0;
    
     while(nIndex%16 != data->index && flag == 0){
     		     if(up){
		     
     		     data->pq[nIndex%16]= gpos - v;
		     nIndex++;
		     }
		     if(down){
		     
                     data->pq[nIndex%16]= gpos + v;
                     nIndex++;

		     }
		     if(left){
		     
                     data->pq[nIndex%16]= gpos - 1;
                     nIndex++;

		     }
		     if(right){
		     
                     data->pq[nIndex%16]= gpos + 1;
                     nIndex++;

		     }
		     flag = 1;
	}

}
		     
void check2(int gpos, int v, __global int* src,__private container* data){
     data->add = 0;
    if(src[up(gpos,v)]>3){
                         data->add +=1;
			 addSurrounding(data, src, up(gpos,v),v,0,1,1,1);
                }
                if(src[down(gpos,v)]>3){
                        data->add +=1;
          		addSurrounding(data, src, down(gpos,v),v,1,0,1,1);

	        }
                if(src[left(gpos,v)]>3){
                        data->add +=1;
			addSurrounding(data, src, left(gpos,v),v,1,1,0,1);

                }
                if(src[right(gpos,v)]>3){
			data->add +=1;
			addSurrounding(data, src, right(gpos,v),v,1,1,1,0);

                }
                if(src[gpos]>3){
                        data->add -=4;
                }
                
}

void rect_helper_2(__global int* src, __global int* dst, int v, int h,__private container* data){
         int gx = get_global_id(0);
         int gy = get_global_id(1);
	 int gpos = pos(gx,gy,h); 
	 if(data->pq[data->index]<0){
		if(dst[gpos]>=0){
			check2(gpos,v,src,data);
			dst[gpos] = src[gpos] + data->add;
		}	
		else{}
	}	
	else{
		int nVal = data->pq[data->index];
		while(nVal>0){
			if(dst[nVal]>=0){
				check2(nVal,v,src,data);
				dst[nVal] = src[nVal]+ data->add;
				data->pq[data->index]= -1;
				data->index ++;
				data->index %= 16;
			}
			else{}
		}
	}
				
	 	 
}	
*/
void check_queue_add(__global sand* s,__global sand* d, __local container * data, int ly, int lsx, int h){
     int gpos = pos(get_global_id(0),get_global_id(1), h);

  for(int i = 0; i < lsx; i++){
        int npos = gpos+i;
	if(s[npos].val >=0){
	if(s[npos].val >3){
		    s[npos].add -= 4;

	}
	if(s[up(npos,h)].val > 3){
		    s[npos].add += 1;
	}
	if(s[down(npos,h)].val > 3){
                    s[npos].add += 1;
        }
	if(s[right(npos,h)].val > 3){
                    s[npos].add += 1;           
        }
	if(s[left(npos,h)].val > 3){
		    s[npos].add +=1;
        }
	d[npos].val = s[npos].val + s[npos].add;	
	s[npos].add = 0;
	}
}
  
	
}


void run_once(__global sand* s, __global sand* d, int v, int h,__local container * data,int lx, int ly, int lsx, int lpos){
     int lsy = get_local_size(1);
     	if(lx == 0){
     	 check_queue_add(s,d,data,ly,lsx,h);
    	 }

}     
__kernel void run(__global sand *s, __global sand *d, int v, int h){
	 
	 __local container data;
	 __local int pq[512];
	 __local int index[512];
	 
	 int lx = get_local_id(0);
	 int ly = get_local_id(1);
	 int lsx = get_local_size(0);
	 int lpos = pos(lx,ly,lsx);
	 if(lpos%lsx==0){
		pq[lpos]= -1;
	 }
	  else{pq[lpos]=0;}
	 index[lpos]=0;
	 int num = get_num_groups(1)*get_num_groups(0);
	 data.pq = pq;
	 data.index = index;	
    	 run_once(s,d,v,h,&data,lx,ly,lsx,lpos);
	 }
		