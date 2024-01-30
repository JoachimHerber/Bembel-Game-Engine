#version 330

//layout(location = 0) in vec4 aPosition;
//layout(location = 1) in vec4 aNormalQuat;
//layout(location = 2) in vec2 aTexCoord;  
 
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTexCoord;  
 
uniform mat4 uNormalMatrix;
uniform mat4 uModleViewMatrix;
uniform mat4 uProjectionMatrix;
 
out DATA {
	vec3 tangent;
	vec3 binormal;  
	vec3 normal; 
	vec2 tex_coord;
} vert_data;
 
mat3 quatToMat3(vec4 q)
{
	float qxx = q.x * q.x;
	float qyy = q.y * q.y;
	float qzz = q.z * q.z;
	float qxz = q.x * q.z;
	float qxy = q.x * q.y;
	float qyz = q.y * q.z;
	float qwx = q.w * q.x;
	float qwy = q.w * q.y;
	float qwz = q.w * q.z;

	mat3 result;
	result[0].xyz = vec3(1.0 - 2.0 * (qyy + qzz),       2.0 * (qxy + qwz),       2.0 * (qxz - qwy));
	result[1].xyz = vec3(      2.0 * (qxy - qwz), 1.0 - 2.0 * (qxx + qzz),       2.0 * (qyz + qwx));
	result[2].xyz = vec3(      2.0 * (qxz + qwy),       2.0 * (qyz - qwx), 1.0 - 2.0 * (qxx + qyy));
	return result;
}

void main()
{
	//mat3 tangent_space = quatToMat3(aNormalQuat);
	mat3 normal_transform  = mat3(uNormalMatrix);//*tangent_space;
 	vert_data.tangent   = normalize(normal_transform*aTangent);
 	vert_data.binormal  = normalize(normal_transform*cross(aNormal, aTangent));
 	vert_data.normal    = normalize(normal_transform*aNormal);
	vert_data.tex_coord = aTexCoord;
 
 	gl_Position = 
 		uProjectionMatrix*uModleViewMatrix*vec4(aPosition, 1.0);
		
}
