#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
// added
in vec3 fragPosition;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
// added
uniform vec3 lightPos;      // light position
uniform mat4 matModel;      // pos, rotation and scaling of object
uniform vec3 viewPos;       //eyes position



// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables
vec3 unHomogenous(vec4 v)
{
    // replaces vec3(matModel * vec4(fragPosition, 1.0)));
    return v.xyz/v.w;
}

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord);

    //=======================PARAMETERS==============================

    // calculate normal in world coordinates
    // mat3 matNormal = mat3(matModel);                     // keeps only rotation and scaling
    mat3 matNormal = transpose(inverse(mat3(matModel)));    //CPU heavy
    vec3 worldNormal = normalize(matNormal * fragNormal);

    // Calculate the location of this fragment (pixel) in world coordinates
    vec3 worldPosition = unHomogenous(matModel * vec4(fragPosition, 1.0));

    //=======================DIFFUSING LIGHT===========================
    // Shading is calculated by Shade = (LightVect dot NormalVect)

    // find light source : L = Lightposition - surfacePosition
    vec3 lightDir = normalize(lightPos - worldPosition);

    // diffuse the light with the dot matrix :
    float shading = clamp(dot(worldNormal, lightDir), 0.1, 1.0);

    //=======================SPECULAR LIGHTNING====================

    //intensity between 0 and 1
    float specularStrength = 1;
    
    // //calculate the view direction vector and corresponding reflect vector along the normal axis
    vec3 viewDir = normalize(viewPos - worldPosition);
    vec3 reflectDir = reflect(-lightDir, worldNormal);  

    //Note that we negate the lightDir vector to get R = 2 (N dot L) * N - L
    //The reflect function expects the first vector to point from the light source towards the fragment's position, 
    //but the lightDir vector is currently pointing the other way 
    

    // calclulate the specular component, 32 is the shininess value of the highlight
    int shininess = 32;
    float spec = pow(clamp(dot(viewDir, reflectDir), 0.1, 1.0), shininess);

    vec3 lightColor = vec3(1.0,1.0,1.0);          // (0.992,0.722,0.075) for yellow light
    vec3 specular = specularStrength * spec * lightColor; 


    //=======================RENDER================================
    finalColor = (texelColor*colDiffuse) * (vec4(shading,shading,shading,1) + vec4(specular,1.0));

}