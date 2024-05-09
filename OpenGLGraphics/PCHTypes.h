#pragma once
#include <vector>
#include <memory>
class PointLight;
class SpotLight;

typedef std::vector<std::unique_ptr<PointLight>> PointLightVector;
typedef std::vector<std::unique_ptr<SpotLight>> SpotLightVector;