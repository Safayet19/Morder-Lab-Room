#ifndef spotLight_h
#define spotLight_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class SpotLight
{
public:
    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float k_c;
    float k_l;
    float k_q;

    float cutOff;
    float outerCutOff;

    SpotLight(
        glm::vec3 pos,
        glm::vec3 dir,
        glm::vec3 amb,
        glm::vec3 diff,
        glm::vec3 spec,
        float constant,
        float linear,
        float quadratic,
        float cut,
        float outerCut)
    {
        position = pos;
        direction = dir;

        ambient = amb;
        diffuse = diff;
        specular = spec;

        k_c = constant;
        k_l = linear;
        k_q = quadratic;

        cutOff = glm::cos(glm::radians(cut));
        outerCutOff = glm::cos(glm::radians(outerCut));
    }

    void setUpSpotLight(Shader& lightingShader)
    {
        lightingShader.use();

        lightingShader.setVec3("spotLight.position", position);
        lightingShader.setVec3("spotLight.direction", direction);

        lightingShader.setVec3("spotLight.ambient", ambient);
        lightingShader.setVec3("spotLight.diffuse", diffuse);
        lightingShader.setVec3("spotLight.specular", specular);

        lightingShader.setFloat("spotLight.k_c", k_c);
        lightingShader.setFloat("spotLight.k_l", k_l);
        lightingShader.setFloat("spotLight.k_q", k_q);

        lightingShader.setFloat("spotLight.cutOff", cutOff);
        lightingShader.setFloat("spotLight.outerCutOff", outerCutOff);
    }
};

#endif