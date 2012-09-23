typedef struct {
       int ping;
       int add;
} controller;
typedef struct{
	int up;
	int down;
	int left;
	int right;
	int val;
}sand;
typedef struct{
	int lx;
	int ly;
	int lsx;
	int lsy;
	int gx;
	int gy;
	int gsy;
	int gsx;
	int lpos;
	int gpos;
	int npos;
	int nx;
	int ny;
	int nsx;
	int nsy;
}variables;
constant const int  ROW = 0x00;
constant const int  COL = 0x01;
constant const int  ADD = 0x10;
constant const int  PING =  0x11;

	
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
int check(__global controller* rowFlag, __global controller* cellFlag,__private variables var, int row, int add, int i){
    if(row&ROW){
	if(add&ADD){
		if(rowFlag[var.npos*var.lsy+var.ly].add){
			return(1);
		}
		else{ return(0); }
		}
	else{
		if(rowFlag[var.npos*var.lsy+var.ly].ping){
                        return(1);
                }
                else{ return(0); }

    	}
    }
    else{
	if(add&ADD){
                if(cellFlag[var.gpos+i].add){
                       
                        return(1);
                }
                else{ return(0); }
     		}
        else{
                if(cellFlag[var.gpos+i].ping){
                       
                        return(1);
                }
                else{ return(0); }
		}
		}
}
void ping_update_add(__global sand* s,__global controller* rowFlag, __global controller* cellFlag,__private variables var, int i){

     		    s[up(var.gpos+i,var.gsx)].down = 1;
		    if(var.gy>1){
		    rowFlag[var.npos*var.lsy+var.ly - var.lsx*var.nsx].add = 1;
		    cellFlag[up(var.gpos+i,var.gsx)].add = 1;
		    }

		    s[down(var.gpos+i,var.gsx)].up = 1;
		    if(var.gy<(var.gsy-1)){
		    rowFlag[var.npos*var.lsy+var.ly + var.lsx*var.nsx].add = 1;
                    cellFlag[down(var.gpos+i,var.gsx)].add = 1;
		    }

		    s[left(var.gpos+i,var.gsx)].right = 1;
		    rowFlag[var.npos*var.lsy+var.ly].add = 1;
		    if(var.gx > 1){
                    cellFlag[left(var.gpos+i,var.gsx)].add = 1;
		    }
	    
		    s[right(var.gpos+i,var.gsx)].left = 1;
		    if(var.gx < (var.gsx-1)){
                    cellFlag[right(var.gpos+i,var.gsx)].add = 1;
		    }

		    s[var.gpos+i].val -=4;
     
	            if(s[var.gpos + i].val >= 4){
		    		  rowFlag[var.npos*var.lsy+var.ly].ping = 1;
				  cellFlag[var.gpos + i].ping = 1;
				  }
}

void apply_add_update_ping(__global sand* s,__global controller* rowFlag, __global controller* cellFlag, __private variables var,int j){

     if(s[var.gpos+j].val >=0){
     s[var.gpos+j].val = s[var.gpos+j].val + s[var.gpos+j].up + s[var.gpos+j].down + s[var.gpos+j].left + s[var.gpos+j].right;
     s[var.gpos+j].up = 0;
     s[var.gpos+j].down = 0;
     s[var.gpos+j].left = 0;
     s[var.gpos+j].right = 0;
     }


     if(s[var.gpos+j].val >= 4){
     		   rowFlag[var.npos*var.lsy+var.ly - var.lsx*var.nsx].ping = 1;
                   cellFlag[var.gpos+j].ping = 1;

     }
}

void aaup_manager(__global sand* s,__global controller* rowFlag, __global controller* cellFlag, __private variables var){
     	 if(var.ly==0 || var.ly == (var.lsy-1)){
             for(int j = 0; j < var.lsx; j++){
         
                 apply_add_update_ping(s,rowFlag,cellFlag,var,j);
             }
         }
         else{

        apply_add_update_ping(s,rowFlag,cellFlag,var,0);

         if(check(rowFlag,cellFlag,var,ROW,ADD,0)){
          for(int i = 1; i < (var.lsx-1); i++){
             if(check(rowFlag,cellFlag,var,COL,ADD,i)){
	 
                 apply_add_update_ping(s,rowFlag,cellFlag,var,i);
             }
          }
         }
         apply_add_update_ping(s,rowFlag,cellFlag,var,(var.lsx-1));
         }
	 
	 
}
void run_once(__global sand* s,__global controller* rowFlag,__global controller* cellFlag,__private variables var){
         for(int i = 0; i < var.lsx; i++){
	 	 if(s[var.gpos+i].val >3){
	 	 ping_update_add(s,rowFlag,cellFlag,var,i);
	 }
	 }
	 

	 barrier(CLK_GLOBAL_MEM_FENCE);
	 
	 aaup_manager(s,rowFlag,cellFlag,var);

	 barrier(CLK_GLOBAL_MEM_FENCE);
	 
}
void run_multiple(__global sand* s,__global controller* rowFlag, __global controller* cellFlag,__private variables var){

     	if(check(rowFlag,cellFlag,var,ROW,PING,0)){
		for(int i = 0; i < var.lsx; i++){
			if(check(rowFlag,cellFlag,var,COL,PING,i)){
			cellFlag[var.gpos+i].ping = 0;
			ping_update_add(s,rowFlag,cellFlag,var,i);
			}

		}
	}
	rowFlag[var.npos*var.lsy+var.ly].ping = 0;

	barrier(CLK_GLOBAL_MEM_FENCE);

	aaup_manager(s,rowFlag,cellFlag,var);


	barrier(CLK_GLOBAL_MEM_FENCE);
	
}     
__kernel void run(__global sand *s, global controller* rCtrl, global controller* cCtrl, int v, int h,int lsy,int lsx,int flag){
	 
	 __private variables var;
	 var.lx = get_local_id(0);
	 var.ly = get_local_id(1);
	 var.nx = get_group_id(0);
	 var.ny = get_group_id(1);
	 var.nsx = get_num_groups(0);
	 var.nsy = get_num_groups(1);
	 var.gx = get_global_id(0);
	 var.gy = get_global_id(1);
	 var.lpos = pos(var.lx,var.ly,var.lsx);
	 var.gpos = pos(var.gx,var.gy,h);
	 var.npos = pos(var.nx,var.ny,var.nsx);
	 var.gsx = h;
	 var.gsy = v; 	 	 
	 var.lsx = lsx;
	 var.lsy = lsy;
         if(var.lx == 0){
	 if(!flag){
    	 run_once(s,rCtrl,cCtrl,var);
	 }
	 else{
	 run_multiple(s,rCtrl,cCtrl,var);	
	 }
	
         }

	 
	
	 
	 
}

		