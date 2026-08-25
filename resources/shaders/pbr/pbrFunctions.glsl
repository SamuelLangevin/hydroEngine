/**
 * These functions compute the lights and the environment's contribution to the surface's
 * color of a fragment based on the PBR model. They are based on the folowing links :
 *   https://learnopengl.com/PBR/Lighting
 *   https://learnopengl.com/PBR/IBL/Diffuse-irradiance
 *   https://learnopengl.com/PBR/IBL/Specular-IBL
 */

#define PI 3.14159265359

/** Stores the surface's properties of an object. */
struct Material {
    float metallic;
    float roughness;
    float ao;
    sampler2D texture_diffuse0;
};

/** Stores the environment light's properties. */
struct Environment {
    /** ambient light map of the environment. */
    samplerCube irradianceMap;

    /** Gives for a combination of reflection direction and roughness what the environment looks like from the light's dispersed reflection.*/
    samplerCube prefilterMap;

    /** Gives for a combination of roughness and viewing angle, how the environment contribution is scaled according to the microfacet BRDF. */
    sampler2D brdfLUT;
};

/** Stores the properties of a fragment. */
struct Fragment {
    vec3 position;
    vec3 normal;
    vec2 texCoords;
};

/** Fragment constructor. */
Fragment cFragment(vec3 fPos, vec3 fNormal, vec2 fTexCoords){
    Fragment fragment;
    fragment.position = fPos;
    fragment.normal = fNormal;
    fragment.texCoords = fTexCoords;
    return fragment;
};

/** Stores the properties of a directional light. */
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

/** Stores the properties of a point light. */
struct PointLight {
    vec3 position;
    float strength;
    vec3 color;
};


/** Stores the properties of a SpotLight. */
struct SpotLight {
    vec3 position;
    vec3 direction;
    float strength;
    float angularAttenuation;
    vec3 color;
};


/**
 * Approximates the amount of microfacets aligned with the halfway vector
 *  @param N normal
 *  @param H halfway vector between lightDir and normal
 */
float distributionGGX(vec3 N, vec3 H, float roughness){
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max(dot(N,H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = NdotH2 * (a2 - 1.0) + 1.0;
    denom = PI * denom * denom;
    return a2 / denom;
}

/** Used by geometrySmith() to approximates the self-shadowing property of the microfacets. */
float geometrySchlickGGX(float cosTheta, float roughness){
    float r = roughness + 1.0;
    float k = (r*r) / 8.0;
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

/**
 * Approximates the self-shadowing property of the microfacets.
 *  @param N normal
 *  @param V incoming view of the camera
 *  @param L direction of the light
 *  @param roughness of the surface
 */
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N,V), 0.0);
    float NdotL = max(dot(N,L), 0.0);
    float ggx1 = geometrySchlickGGX(NdotV, roughness);
    float ggx2 = geometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

/**
 * Describes the ratio of surface reflection
 *  @param F_0 base reflectivity factor
 *  @param HdotV dot product between 1. the halfway vector between lightDir and normal and 2. the view vector
 */
vec3 fresnelSchlick(float HdotV, vec3 F_0){
    return F_0 + (1.0 - F_0) * pow(1.0 - HdotV, 5.0);
}


/*
 * Ratio of surface reflection based on roughness
 *   @param F_0 the base reflectivity factor
 */
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F_0, float roughness){
    return F_0 + (max(vec3(1.0 -roughness), F_0) - F_0) * pow(1.0 - cosTheta, 5.0);
}

/*
 * Base calculations for any light.
 * @param lightColor
 * @param material physical properties of the fragment
 * @param geometric properties of the fragment
 * @param L direction of the light
 * @param V incoming directon of the camera
 * @param F_0 surface reflection at zero incidence
 * @returns the light's contribution to the surface's color.
 */
vec3 calcGenericLight(vec3 lightColor, Material material, Fragment frag, vec3 L, vec3 V, vec3 F_0){
    vec3 albedo = vec3(texture(material.texture_diffuse0, frag.texCoords));
    vec3 H = normalize(V + L);
    vec3 N = normalize(frag.normal);

    float D = distributionGGX(N, H, material.roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F_0);
    float G = geometrySmith(N, V, L, material.roughness);

    vec3 k_D = (vec3(1.0) - F) * (1.0 - material.metallic);

    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular = (D * F * G) / max(denominator, 0.001);

    return (k_D * albedo / PI + specular) * lightColor * max(dot(N,L), 0.0);
}

/**
 * @param light the directionalLight illuminating the surface
 * @param material physical properties of the fragment's surface
 * @param frag the geometric properties of the fragment
 * @param viewDir the direction of the camera's view
 * @param base reflectivity factor
 * @returns the light's contribution to the surface's color.
 */
vec3 calcDirLight(DirectionalLight light, Material material, Fragment frag, vec3 viewDir, vec3 F_0){

    vec3 lightDir = normalize(-light.direction);
    return calcGenericLight(light.color, material, frag, lightDir, viewDir, F_0);
}

/**
 * @param light the pointLight illuminating the surface
 * @param material physical properties of the fragment's surface
 * @param frag the geometric properties of the fragment
 * @param viewDir the direction of the camera's view
 * @param base reflectivity factor
 * @returns the light's contribution to the surface's color.
 */
vec3 calcPointLight(PointLight light, Material material, Fragment frag, vec3 viewDir, vec3 F_0){

    vec3 lightDir = normalize(light.position - frag.position);
    float dist = length(light.position - frag.position);
    float attenuation = light.strength / (1.0f + dist * dist);

    return calcGenericLight(light.color, material, frag, lightDir, viewDir, F_0) * attenuation;
}

/**
 * @param light the spotLight illuminating the surface
 * @param material physical properties of the fragment's surface
 * @param frag the geometric properties of the fragment
 * @param viewDir the direction of the camera's view
 * @param base reflectivity factor
 * @returns the light's contribution to the surface's color.
 */
vec3 calcSpotLight(SpotLight light, Material material, Fragment frag, vec3 viewDir, vec3 F_0){

    vec3 lightDir = normalize(light.position - frag.position);
    float dist = length(light.position - frag.position);
    float attenuation = (light.strength * pow(max(dot(light.direction, -lightDir), 0.0f), light.angularAttenuation) ) / (1.0f + dist * dist);

    return calcGenericLight(light.color, material, frag, lightDir, viewDir, F_0) * attenuation;
}

/**
 * @param env the environment's light properties.
 * @param material the physical properties of the surface
 * @param albedo the diffuse color of the surface.
 * @param N normal of the fragment
 * @param V incoming direction of the camera's view
 * @param base reflectivity factor
 * @returns the environment's contribution to the surface's color
 */
vec3 calcAmbientLight(Environment env, Material material, vec3 albedo, vec3 N, vec3 V, vec3 F_0) {
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F_0, material.roughness);
    vec3 kS = F; //Ratio of the light reflected
    vec3 kD = (1.0 - kS) * (1.0 - material.metallic); //Ratio of the light absorbed

    vec3 irradiance = texture(env.irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    vec3 R = reflect(-V, N);
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(env.prefilterMap, R, material.roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF = texture(env.brdfLUT, vec2(max(dot(N, V), 0.0), material.roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    return (kD * diffuse + specular) * material.ao;
}











