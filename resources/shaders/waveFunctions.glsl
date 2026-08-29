#define PI 3.14159
#define e 2.71828

/** Stores the properties of a direcitonal wave */
struct DirectionalWave {
    vec2 direction;
    float waveLength;
    float amplitude;
    float speed;
};

/** Stores the properties of a point wave */
struct PointWave {
    vec2 origin;
    float waveLength;
    float amplitude;
    float speed;
    float dropTime;
};

//todo replace magic 5, there could be better measures of speed and attenuation based on studies...
float computePointWaveAttenuation(float advance, vec2 originToVert, float waveAmplitude) {
    float reachedPos = ceil(clamp(PI * advance - length(originToVert), 0.0, 1.0));
    return pow(e, -advance/(waveAmplitude * 5.0)) * reachedPos;
}

/**
 * @param pWave the point wave to compute
 * @param absoluteTime the current time in seconds
 * @param vertexPos
 * returns the wave's contribution to the position of the vertex
 */
vec3 computePointWave(PointWave pWave, float absoluteTime, vec3 vertexPos){
    vec2 originToVert = pWave.origin - vertexPos.xz;
    float advance = pWave.speed * (absoluteTime - pWave.dropTime) /sqrt(pWave.waveLength);
    float attenuation = computePointWaveAttenuation(advance, originToVert, pWave.amplitude);

    vec3 newPos;
    float phase = -length(originToVert)/pWave.waveLength + advance;
    newPos.x = -attenuation * pWave.amplitude * normalize(originToVert).x * sin(phase);
    newPos.y = attenuation * pWave.amplitude * sin(phase);
    newPos.z = -attenuation * pWave.amplitude * normalize(originToVert).y * sin(phase);
    return newPos;
}

/**
 * Used to calculate the normal.
 * @param pWave the point wave to compute the effect
 * @param absoluteTime the current time in seconds
 * @param vertexPos
 * returns the wave's contribution to the binormal of the vertex
 */
vec3 computePointWaveBinormal(PointWave pWave, float absoluteTime, vec3 vertexPos){
    vec2 originToVert = pWave.origin - vertexPos.xz;
    float advance = pWave.speed * (absoluteTime - pWave.dropTime) /sqrt(pWave.waveLength);
    float attenuation = computePointWaveAttenuation(advance, originToVert, pWave.amplitude);
    float phase = -length(originToVert)/pWave.waveLength + advance;

    vec3 binormal;
    binormal.x = attenuation * pWave.amplitude * normalize(originToVert).x * originToVert.x * cos(phase);
    binormal.y = -attenuation * pWave.amplitude * cos(phase);
    binormal.z = 0.0;

    return binormal;
}


/**
 * Used to calculate the normal.
 * @param pWave the point wave to compute the effect
 * @param absoluteTime the current time in seconds
 * @param vertexPos
 * returns the wave's contribution to the tangent of the vertex
 */
vec3 computePointWaveTangent(PointWave pWave, float absoluteTime, vec3 vertexPos){
    vec2 originToVert = pWave.origin - vertexPos.xz;
    float advance = pWave.speed * (absoluteTime - pWave.dropTime) /sqrt(pWave.waveLength);
    float attenuation = computePointWaveAttenuation(advance, originToVert, pWave.amplitude);
    float phase = -length(originToVert)/pWave.waveLength + advance;

    vec3 tangent;
    tangent.x = 0.0;
    tangent.y = -attenuation * pWave.amplitude * cos(phase);
    tangent.z = attenuation * pWave.amplitude * normalize(originToVert).y *originToVert.y * cos(phase);

    return tangent;
}


/**
 * @param dWave the directional wave to compute
 * @param absoluteTime the current time in seconds
 * @param vertexPos
 * returns the wave's contribution to the position of the vertex
 */
vec3 computeDirectionalWave(DirectionalWave dWave, float absoluteTime, vec3 vertexPos){
    vec3 newPos;
    float advance = dWave.speed*absoluteTime / sqrt(dWave.waveLength);
    float phase = dot(dWave.direction, vertexPos.xz)/dWave.waveLength - advance;
    newPos.x = -dWave.amplitude * dWave.direction.x * sin(phase);
    newPos.y = dWave.amplitude * cos(phase);
    newPos.z = -dWave.amplitude * dWave.direction.y * sin(phase);
    return newPos;
}

/**
 * Used to calculate the normal.
 * @param dWave the directional wave to compute the effect
 * @param absoluteTime the current time in seconds
 * @param vertexPos
 * returns the wave's contribution to the binormal of the vertex
 */
vec3 computeDirectionalWaveBinormal(DirectionalWave dWave, float absoluteTime, vec3 vertexPos){
    float advance = dWave.speed*absoluteTime / sqrt(dWave.waveLength);
    float phase = dot(dWave.direction, vertexPos.xz)/dWave.waveLength - advance;

    vec3 binormal;
    binormal.x = -dWave.amplitude*dWave.direction.x/dWave.waveLength * cos(phase);
    binormal.y = -dWave.amplitude*dWave.direction.x/dWave.waveLength * sin(phase);
    binormal.z = 0.0;
    return binormal;
}

/**
 * Used to calculate the normal.
 * @param dWave the directional wave to compute the effect
 * @param absoluteTime the current time in seconds
 * @param vertexPos
 * returns the wave's contribution to the tangent of the vertex
 */
vec3 computeDirectionalWaveTangent(DirectionalWave dWave, float absoluteTime, vec3 vertexPos){
    float advance = dWave.speed*absoluteTime / sqrt(dWave.waveLength);
    float phase = dot(dWave.direction, vertexPos.xz)/dWave.waveLength - advance;

    vec3 tangent;
    tangent.x = 0.0;
    tangent.y = -dWave.amplitude*dWave.direction.y/dWave.waveLength * sin(phase);
    tangent.z = -dWave.amplitude*dWave.direction.y/dWave.waveLength * cos(phase);
    return tangent;
}