#ifndef INSIGHT_SHADER_FRAGMENT_H
#define INSIGHT_SHADER_FRAGMENT_H

#include "ShaderCollection.h"

const char* ShaderCollection::fragment_source_ = R"glsl(
    #version 150 core

    in vec3 colour;                     // object / vertex colour
    in vec3 normal;                     // normal to vertex face
    in vec3 fragmentPosition;           // vertex position in world (not normalised device) co-ords

    uniform int lightingOn;
    uniform vec3 lightPosition;
    uniform vec3 lightColour;
    uniform float lightIntensity;
    uniform vec3 cameraPosition;        // view / camera location in world co-ords

    out vec4 outColour;                 // final fragment colour

    void main()
    {
        float specularStrength = 0.5;

        vec3 ambient = lightIntensity * lightColour;

        // Calculate the direction vector between the light source and the fragment
        vec3 lightingDirection = normalize(lightPosition - fragmentPosition);

        // Calculate the angle (actually just dot product) between the light direction vector and the fragment face's normal
        vec3 normalisedNormal = normalize(normal);
        float diffuseIntensity = max(dot(normalisedNormal, lightingDirection), 0.0);

        // Calculate the diffuse light intensity based on the angle between the light and the normal
        vec3 diffuse = diffuseIntensity * lightColour;

        // Calculate the direction vector between the camera / view location and the fragment (to determine reflection intensity)
        vec3 cameraDirection = normalize(cameraPosition - fragmentPosition);
        vec3 reflectionDirection = reflect(-lightingDirection, normalisedNormal);

        float specularIntensity = pow(max(dot(cameraDirection, reflectionDirection), 0.0), 32);   // 32 is the shininess of the object
        vec3 specular = specularStrength * specularIntensity * lightColour;

        vec3 fragmentColour = (ambient + diffuse + specular) * colour;

        if (lightingOn == 0)
        {
            fragmentColour = colour;
        }

        outColour = vec4(fragmentColour, 1.0);
    }
)glsl";

#endif //INSIGHT_SHADER_FRAGMENT_H
