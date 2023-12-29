﻿#pragma once
#include <d3d11.h>

#include "Singleton.h"

class Scene : public Singleton<Scene>
{
public:
    Scene();
    virtual ~Scene() final = default;
    
    void Tick(float delta_time);
    void Render();

private:
    void SearchCharacter(const std::string& character_name);

    class ImVec4 GetColorByGrade(const std::string& grade);
    
    std::string date_;

    int character_image_width;
    int character_image_height;

    ID3D11ShaderResourceView* character_image;
    
};
