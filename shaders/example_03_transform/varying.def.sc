vec3 a_position  : POSITION;
vec4 a_color0    : COLOR0;

vec4 v_color0    : COLOR0 = vec4(0.0, 1.0, 0.0, 1.0);

// varying.def.sc 注释必须写在后面，否则影响shader编译

// 提前定义使用到的输入输出变量，供vertex shader和fragment shader使用
// shaderc 工具进行转化时，会读取这个文件的变量定义
// 为了方便兼容语法：glsl定义在外面；hlsh（d3d、metal）定义在入口函数
// 否则需要将shader转化成语法树来提取变量信息

