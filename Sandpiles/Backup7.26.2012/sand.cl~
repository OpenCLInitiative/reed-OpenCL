void helper(__global int *s, __global int *d, int id, int v, int h){
 __private int add = 0;
 if(id==0){
 if(s[id+1]>=4){
	add += 1;
	}
 if(s[id+v]>=4){
	add += 1;
	}
	
	
}	
else{ if(id == (h-1)){
	 if(s[id-1]>=4){
        add += 1;
        }
	 if(s[id+v]>=4){
        add += 1;
        }


}
else{ if(id == h*(v-1)){
 if(s[id+1]>=4){
        add += 1;
        }
 if(s[id-v]>=4){
        add += 1;
        }


}
else{ if(id ==(h*v - 1)){
 if(s[id-1]>=4){
        add += 1;
        }
 if(s[id-v]>=4){
        add += 1;
        }

        }

else{ if(id < h){

 if(s[id+1]>=4){
        add += 1;
        }
 if(s[id+v]>=4){
        add += 1;
        }
 if(s[id-1]>=4){
	add += 1;
	}	

}
else{ if(id % h ==0){
if(s[id+1]>=4){
        add += 1;
        }
 if(s[id+v]>=4){
        add += 1;
        }
 if(s[id-v]>=4){
        add += 1;
        }

}
else{ if((id+1)%h==0){
if(s[id-1]>=4){
        add += 1;
        }
 if(s[id+v]>=4){
        add += 1;
        }
 if(s[id-v]>=4){
        add += 1;
        }

}
else{ if(id > h*(v-1)){
if(s[id+1]>=4){
        add += 1;
        }
 if(s[id-1]>=4){
        add += 1;
        }
 if(s[id-v]>=4){
        add += 1;
        }

}
else{
if(s[id+1]>=4){
        add += 1;
        }
 if(s[id+v]>=4){
        add += 1;
        }
 if(s[id-1]>=4){
        add += 1;
        }
if(s[id-v]>=4){
	add += 1;
	}

}}}}}}}}
if(s[id]>=4){
	add -= 4;
}
d[id] = add+s[id];
}

__kernel void run(__global int *s, __global int *d, int v, int h){

	 int id = get_global_id(0);
	 helper(s,d,id,v,h);
	 barrier(CLK_GLOBAL_MEM_FENCE);

}
		