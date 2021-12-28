#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;
// added
in vec3 fragPosition;
in vec4 fragTangent;
// in vec3 lightPos;  

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
// added
uniform vec3 lightPos;      // light position
uniform mat4 matModel;


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
    // mat3 matNormal = mat3(matModel); // keeps only rotation and scaling
    mat3 matNormal = transpose(inverse(mat3(matModel))); //CPU heavy
    vec3 worldNormal = normalize(matNormal * fragNormal);

    // Calculate the location of this fragment (pixel) in world coordinates
    vec3 worldPosition = unHomogenous(matModel * vec4(fragPosition, 1.0));

    //=======================DIFFUSING LIGHT===========================
    // Shading is calculated by Shade = (LightVect dot NormalVect) * Diffused color  

    // find light source : L = Lightposition - surfacePosition
    vec3 lightDir = normalize(lightPos - worldPosition);

    // diffuse the light with the dot matrix :
    // cos(angle) = dot(v1, v2) / (length(v1) * length(v2)) but v1 and v2 are normalized
    float shading = clamp(dot(worldNormal, lightDir), 0.1, 1.0);

    //=======================ROTATING==============================
    // float cos_incidence = dot( normal_in_cam_space, rotating_light );
    // http://www.c-jump.com/bcc/common/Talk3/OpenGL/Wk06_light/Wk06_light.html
    //=======================RENDER================================

    finalColor = (texelColor*colDiffuse)*vec4(shading,shading,shading,1);
    // finalColor += texelColor*(ambient/100.0)*colDiffuse
}