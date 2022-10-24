#version 460 core
out vec4 color;
in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;
uniform vec3 viewPos;
uniform bool isTextured;
uniform sampler2D texSampler;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float spec_coeff;
};
uniform Material material;

struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight point_light;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewpos){
    vec3 totalColor = vec3(0.0f);
    totalColor += light.ambient * material.ambient;
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 norm = normalize(normal);
    float cosine = max(dot(norm, lightDir), 0.0f);
    totalColor += light.diffuse * material.diffuse * cosine;
    vec3 viewDir = normalize(viewpos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float cosine_power = pow(max(dot(reflectDir, viewDir), 0.0f), material.spec_coeff);
    totalColor += (light.specular * material.specular * cosine_power);
    return totalColor;
}

void main()
{    
    if(isTextured){
        color = texture(texSampler, TexCoord);
    }else{
        color = vec4(CalcPointLight(point_light,Normal,FragPos,viewPos),1.0f); 
    }
    
}