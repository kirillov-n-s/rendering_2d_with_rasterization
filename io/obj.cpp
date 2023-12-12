#include <fstream>
#include <sstream>
#include "obj.h"

namespace IO {

std::string readObjVerticesFaces(
	const std::string& path,
	HomogCoords3d& outVertices,
	IndexVec& outPolygonVertexIndices,
	IndexVec& outPolygonStarts)
{
    if (!outVertices.empty() || !outPolygonVertexIndices.empty() || !outPolygonStarts.empty())
        return "Output args are not empty";

    outPolygonStarts.push_back(0);

    std::ifstream file(path);
    if (!file.is_open())
        return "File " + path + " does not exist.";

    std::string header;
    for (;;) {

        file >> header;
        if (file.eof())
            break;

        if (header == "v") {
            HomogCoord3d vertex;
            file >> vertex.x >> vertex.y >> vertex.z;
            if (!file)
                return "Bad vertex";
            file >> vertex.w;
            if (!file) {
                file.clear();
                vertex.w = 1.0f;
            }
            vertex /= vertex.w;
            outVertices.push_back(vertex);
        }
        else if (header == "f") {

            std::string face;
            std::getline(file, face);
            face.erase(face.find_last_not_of(" \n\r\t") + 1);
            std::istringstream buffer{ face };

            int vertexInd;
            int texcoordOrNormalInd;

            int delim;

            for (;;) {

                buffer >> vertexInd;
                if (!buffer)
                    return "Bad face format";
                outPolygonVertexIndices.push_back(vertexInd - 1);
                if (buffer.eof())
                    break;

                delim = buffer.get();
                if (delim == ' ')
                    continue;
                if (delim != '/')
                    return "Bad face format";

                buffer >> texcoordOrNormalInd;
                if (buffer.eof())
                    break;
                if (!buffer)
                    buffer.clear();

                delim = buffer.get();
                if (delim == ' ')
                    continue;
                if (delim != '/')
                    return "Bad face format";

                buffer >> texcoordOrNormalInd;
                if (!buffer)
                    return "Bad face format";
                if (buffer.eof())
                    break;
            }

            outPolygonStarts.push_back(outPolygonVertexIndices.size());

            const int nFaces = outPolygonStarts.size() - 1;
            if (outPolygonStarts[nFaces] - outPolygonStarts[nFaces - 1] < 3)
                return "Not enough face elements";
        }
        else
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    file.close();

    return "";
}

IndexVec triangulateFaces(
	const IndexVec& polygonVertexIndices,
	const IndexVec& polygonStarts)
{
    IndexVec triangleVertexIndices;
    const int nPolygons = polygonStarts.size() - 1;
    for (int polygonInd = 0; polygonInd < nPolygons; ++polygonInd) {

        const int faceBegin = polygonStarts[polygonInd];
        const int faceEnd = polygonStarts[polygonInd + 1] - 1;
        for (int faceInd = faceBegin + 1; faceInd < faceEnd; ++faceInd) {

            triangleVertexIndices.push_back(polygonVertexIndices[faceBegin]);
            triangleVertexIndices.push_back(polygonVertexIndices[faceInd]);
            triangleVertexIndices.push_back(polygonVertexIndices[faceInd + 1]);
        }
    }
    return triangleVertexIndices;
}

AdjacencyMat facesToAdjacency(
	const IndexVec& triangleVertexIndices,
    const int nVertices)
{
    AdjacencyMat adjacency = makeZeroAdjacency(nVertices);
    const int nTriangles = triangleVertexIndices.size() / 3;
    for (int triangleInd = 0; triangleInd < nTriangles; ++triangleInd) {

        const int vertexInd0 = triangleVertexIndices[3 * triangleInd + 0];
        const int vertexInd1 = triangleVertexIndices[3 * triangleInd + 1];
        const int vertexInd2 = triangleVertexIndices[3 * triangleInd + 2];

        adjacency[vertexInd0][vertexInd1] = true;
        adjacency[vertexInd1][vertexInd0] = true;

        adjacency[vertexInd1][vertexInd2] = true;
        adjacency[vertexInd2][vertexInd1] = true;

        adjacency[vertexInd2][vertexInd0] = true;
        adjacency[vertexInd0][vertexInd2] = true;
    }
    return adjacency;
}

std::string verticesAndAdjacencyFromObj(
    const std::string& path,
    HomogCoords3d& outVertices,
    AdjacencyMat& outAdjacency)
{
    IndexVec triangleVertexIndices;
    verticesAndTrianglesFromObj(path, outVertices, triangleVertexIndices);
    outAdjacency = facesToAdjacency(triangleVertexIndices, outVertices.size());
    return "";
}

std::string verticesAndTrianglesFromObj(
    const std::string& path,
    HomogCoords3d& outVertices,
    IndexVec& outTriangleVertexIndices)
{
    IndexVec polygonVertexIndices;
    IndexVec polygonStarts;
    const std::string error = readObjVerticesFaces(
        path,
        outVertices,
        polygonVertexIndices,
        polygonStarts);
    if (!error.empty())
        return std::string("Obj read error: ") + error;
    outTriangleVertexIndices = triangulateFaces(polygonVertexIndices, polygonStarts);
    return "";
}
}
