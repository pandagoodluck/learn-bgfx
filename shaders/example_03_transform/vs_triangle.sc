// 根据varying.def.sc 定义的参数，设置输入输出
$input a_position, a_color0
$output v_color0

#include "../common/common.sh"

void main()
{
    // 设置gl_Position，对位置进行矩阵变换，需要乘以MVP矩阵
	gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0) );
	v_color0 = a_color0;
}