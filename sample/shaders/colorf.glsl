#version 330 core
uniform sampler2D diffuse;
uniform sampler2D specular;
uniform float     shininess;
uniform vec4 _Color;

struct Light {
    vec3 position;
    float strength;
    vec3 color;
};

uniform Light light;  

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
  
uniform vec3 viewPos; 

void main()
{    
  	FragColor = _Color;
      /*
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 ambient  = 1 * vec3(texture(diffuse, TexCoords));
	vec3 diffuse  = 1 * (diff * vec3(texture(diffuse, TexCoords)));
	vec3 specular = 1 * (spec * vec3(texture(specular, TexCoords)));
        
    vec3 result = light.strength * light.color * (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);*/
}