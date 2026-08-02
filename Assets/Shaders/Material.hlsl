struct VS_INPUT
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};

struct VPS_INOUT
{
    float4 position : SV_POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : TEXCOORD1;
};

struct LightData
{
    float4 color;
    float4 direction;
    float4 position;
    float radius;
    float intensity;
    int type;
    int pad0;
};

cbuffer constant : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 cameraPosition;
    LightData lights[10];
    int num_active_lights;
    int pad0;
    int pad1;
    int pad2;
}

VPS_INOUT vsmain(VS_INPUT input)
{
    VPS_INOUT output = (VPS_INOUT) 0;

	//WORLD SPACE
    output.position = mul(input.position, m_world);
    output.worldPosition = output.position.xyz;
    // VIEW SPACE
    output.position = mul(output.position, m_view);
    // SCREEN SPACE
    output.position = mul(output.position, m_proj);
    
    output.texcoord = input.texcoord;
    
    output.normal = normalize(mul(input.normal, (float3x3) m_world));

    return output;
}


float3 DirLight(LightData light, float3 normal, float3 dirToCam, float3 color)
{
    float3 light_direction = normalize(light.direction.xyz);
    
    // Diffuse
    float kd = 0.7;
    float amount_diffuse_light = max(dot(light_direction.xyz, normal), 0.0);
    float3 id = light.color.rgb * light.intensity;
    id *= (color.rgb);
    float3 diffuse_light = kd * id * amount_diffuse_light;
    
    // Specular    
    float ks = 1.0;
    float3 is = float3(1.0, 1.0, 1.0) * light.intensity;
    float3 reflected_light = reflect(light_direction.xyz, normal);
    float shininess = 30.0;
    float amount_specular_light = pow(max(0.0, dot(reflected_light, dirToCam)), shininess);

    float3 specular_light = ks * amount_specular_light * is;
    
    return diffuse_light + specular_light;
}

float3 PointLight(LightData light, float3 normal, float3 worldPos,float3 dirToCam, float3 color)
{   
    float3 light_direction = normalize(light.position.xyz - worldPos);
    float distance_light_object = length(light.position.xyz - worldPos);
    float fade_area = max(0, distance_light_object - light.radius);
    float attenuation = 1.0 + fade_area;

    // Diffuse
    float kd = 0.7;
    float amount_diffuse_light = max(dot(light_direction.xyz, normal), 0.0);
    float3 id = light.color.rgb * light.intensity;
    id *= (color.rgb);
    float3 diffuse_light = (kd * id * amount_diffuse_light) / attenuation;
    
    // Specular    
    float ks = 1.0;
    float3 is = float3(1.0, 1.0, 1.0) * light.intensity;
    float3 reflected_light = reflect(light_direction.xyz, normal);
    float shininess = 30.0;
    float amount_specular_light = pow(max(0.0, dot(reflected_light, dirToCam)), shininess);

    float3 specular_light = (ks * amount_specular_light * is) / attenuation;
    
    return diffuse_light + specular_light;
}

Texture2D Color : register(t0);
sampler ColorSampler : register(s0);

float4 psmain(VPS_INOUT input) : SV_TARGET
{
    float4 color = Color.Sample(ColorSampler, input.texcoord);
    //float3 light_direction = normalize(light.direction.xyz);
    float3 normal = input.normal;
    float3 directionToCamera = normalize(input.worldPosition - cameraPosition.xyz);
    
    // AMBIENT LIGHT
    float ka = 8.5;
    float3 ia = float3(0.09, 0.082, 0.082);
    ia *= (color.rgb);

    float3 ambient_light = ka * ia;
    
    float3 final_light = ambient_light;

    for (int i = 0; i < num_active_lights; i++)
    {
        // DirLight
        if (lights[i].type == 0)
        {
            final_light += DirLight(lights[i], input.normal, directionToCamera, color.rgb);
        }
        // PointLight
        if (lights[i].type == 1)
        {
            final_light += PointLight(lights[i], input.normal, input.worldPosition.xyz, directionToCamera, color.rgb);

        }
    }

    return float4(final_light, 1.0);
	//// DIFFUSE LIGHT
 //   float kd = 0.7;
 //   float amount_diffuse_light = max(dot(light_direction.xyz, input.normal), 0.0);
 //   float3 id = light.color.rgb;
 //   id *= (color.rgb);
 //   float3 diffuse_light = kd * id * amount_diffuse_light;

 //   // SPECULAR LIGHT
 //   float ks = 1.0;
 //   float3 is = float3(1.0, 1.0, 1.0);
 //   float3 reflected_light = reflect(light_direction.xyz, input.normal);
 //   float shininess = 30.0;
 //   float amount_specular_light = pow(max(0.0, dot(reflected_light, directionToCamera)), shininess);

 //   float3 specular_light = ks * amount_specular_light * is;


}