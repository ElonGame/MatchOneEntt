#pragma once

#include "Extension/System.h"

class GameBoardSystem : public System
{
public:
    virtual void Initialize(entt::DefaultRegistry &Registry) override;
    virtual void Update(entt::DefaultRegistry &Registry) override;
};
