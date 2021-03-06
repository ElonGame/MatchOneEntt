#pragma once

#include "entt.hpp"

class System
{
protected:
    System() {};
    System(const System&) = default;
    System& operator=(const System&) = default;

public:
    virtual ~System() = default;

    virtual void Initialize(entt::DefaultRegistry &Registry);
    virtual void Update(entt::DefaultRegistry &Registry);
};
