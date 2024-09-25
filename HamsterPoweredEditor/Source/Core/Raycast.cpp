#include "Raycast.h"

#include "App.h"
#include "Scene.h"
#include "Actors/ClothActor.h"
#include "Actors/DynamicMeshActor.h"

Raycast::Raycast(const glm::vec3& origin, const glm::vec3& direction, Hit& hit, float maxDistance)
{
    // Get actors from the scene
    auto actors = App::Instance().m_currentScene->GetActorsOfClass<DynamicMeshActor>();

    for (auto actor : actors)
    {
        if (!actor->TraceEnabled) continue;
        StaticMesh& mesh = actor->m_mesh;

        // Get the world matrix of the actor
        glm::mat4 worldMatrix = actor->GetTransform();

        // Get the inverse of the world matrix
        glm::mat4 invWorldMatrix = glm::inverse(worldMatrix);
        
        // Transform the ray into the local space of the mesh
        glm::vec3 localOrigin = glm::vec3(invWorldMatrix * glm::vec4(origin, 1.0f));
        
        // Transform the direction into the local space of the mesh
        glm::vec3 localDirection = glm::vec3(invWorldMatrix * glm::vec4(direction, 0.0f));

        // Normalize the direction
        localDirection = glm::normalize(localDirection);

        // Get the vertices of the mesh
        const std::vector<Vertex>& vertices = mesh.GetVertices();

        // Get the indices of the mesh
        const std::vector<unsigned int>& indices = mesh.GetIndices();

        // Loop through all the triangles of the mesh
        for (unsigned int i = 0; i < indices.size(); i += 3)
        {
            // Get the three vertices of the triangle
            const Vertex& v0 = vertices[indices[i]];
            const Vertex& v1 = vertices[indices[i + 1]];
            const Vertex& v2 = vertices[indices[i + 2]];

            // Get the position of the vertices
            glm::vec3 p0 = v0.Position;
            glm::vec3 p1 = v1.Position;
            glm::vec3 p2 = v2.Position;

            // Calculate the edge vectors
            glm::vec3 e1 = p1 - p0;
            glm::vec3 e2 = p2 - p0;

            // Calculate the determinant
            glm::vec3 p = glm::cross(localDirection, e2);
            float det = glm::dot(e1, p);

            // If the determinant is negative, the triangle is backfacing
            if (det < 0.0f)
                continue;

            // Calculate the distance from vertex 0 to the ray origin
            glm::vec3 t = localOrigin - p0;

            // Calculate the u parameter
            float u = glm::dot(t, p);

            // Check if the ray intersects the triangle
            if (u < 0.0f || u > det)
                continue;

            // Calculate the v parameter
            glm::vec3 q = glm::cross(t, e1);
            float v = glm::dot(localDirection, q);

            // Check if the ray intersects the triangle
            if (v < 0.0f || u + v > det)
                continue;

            // Calculate the t parameter
            float t2 = glm::dot(e2, q);

            // Calculate the intersection distance
            float distance = t2 / det;

            // Check if the intersection is within the ray's range
            if (distance < 0.0f || distance > maxDistance)
                continue;

            // Check if the intersection is closer than the previous one
            if (distance < hit.Distance)
            {
                // Update the hit
                hit.Distance = distance;
                hit.Location = origin + direction * distance;
                hit.Normal = glm::normalize(glm::cross(e1, e2));
                hit.Actor = actor;
                hit.HitSomething = true;
            }
        }
        
    }
    
}
