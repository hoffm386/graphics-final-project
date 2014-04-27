#version 140
// Input vertex data, different for all executions of this shader.
in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 texCoord;

uniform vec4 LightPosition;
uniform mat4 mPVM;
uniform mat4 mVM;
uniform mat4 mT;
uniform mat4 mR;

void main(){

   // Transform vertex  position into eye coordinates
   vec3 pos = (mVM*mT*mR*vPosition).xyz;

   // Transform vertex normal into eye coordinates
   fN = (mVM*mT*mR*vec4(vNormal, 0.0)).xyz;
        
   fE = -pos;
   fL = LightPosition.xyz - pos;
   
   texCoord = vTexCoord;
	gl_Position = mPVM* mT*mR * vPosition;
}

