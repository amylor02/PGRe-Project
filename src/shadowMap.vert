#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightProjection;
uniform mat4 model;
uniform mat4 rot;

void main()
{
   mat4 i1 = mat4(1.f);

    if(i1 == model){
     vec3 pos = vec3(rot*vec4(aPos,1.0f));
     gl_Position = lightProjection *vec4(pos,1.0f);
    }
    else{
     vec3 pos = vec3(model*vec4(aPos,1.0f));
     gl_Position = lightProjection * vec4(pos,1.0f);
    }
    

}