#include <algorithm>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include "shading.h"


namespace Rasterization {

void flatShading(
    const Coords3d& faceNormals,
    const Coord3d& lightDirection,
    ColorVec& inOutFaceColors)
{
    const int nFaces = faceNormals.size();
    for (int faceInd = 0; faceInd < nFaces; ++faceInd) {
        const Rasterization::Color origColor = inOutFaceColors[faceInd];
        const Coord3d faceNormal = faceNormals[faceInd];
        const float diffuseComponent = std::clamp(
            -glm::dot(faceNormal, lightDirection),
            0.0f,
            1.0f);
        inOutFaceColors[faceInd] = Rasterization::Color{
            .r = byte(origColor.r * diffuseComponent),
            .g = byte(origColor.g * diffuseComponent),
            .b = byte(origColor.b * diffuseComponent)
        };
    }
}
}
