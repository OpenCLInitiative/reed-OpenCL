void rect_helper(__global int *s, __global int *d, int id, int v, int h){
 __private int add = 0;
if(s[id]>=0){
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

}
else{}
if(s[id]>=4){
	add -= 4;
}
else{}
barrier(CLK_GLOBAL_MEM_FENCE);
d[id] = s[id]+add;
}

void hex_helper(__global int *s, __global int *d, int id, int v, int h){
/* __private int add = 0;
if(s[id]>=0){
	if(s[id
*/
}
__kernel void hex(__global int *s, __global int *d, int v, int h){
	 int id = get_global_id(0);
	 hex_helper(s,d,id,v,h);
}
__kernel void run(__global int *s, __global int *d, int v, int h){

	 int id = get_global_id(0);
	 rect_helper(s,d,id,v,h);
	 

}
		