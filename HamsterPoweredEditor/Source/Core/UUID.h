#pragma once


#include <string>
#include <uuid.h>

namespace HP
{
    class UUID
    {
    public:
        friend class Scene;

        UUID();
        UUID(const std::string& str);
    
        void Generate();
        std::string GetAsString() const { return uuids::to_string(id); }
        void SetID(const std::string& _id) { this->id = uuids::uuid::from_string(_id).value(); }
        const uuids::uuid& GetUUID() const { return id; }
    
        bool operator==(const UUID& other) const { return id == other.id; }
        bool operator!=(const UUID& other) const { return id != other.id; }
    
    private:
        uuids::uuid id;
        
    };
}
