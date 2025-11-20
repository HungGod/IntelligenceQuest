#pragma once

#define MAX_KEYS 1024

#define Int ValTemplate<int>
#define Float ValTemplate<float>
#define Double ValTemplate<double>
#define Bool ValTemplate<bool>
#define Json ValTemplate<nlohmann::json>
#define DavidString ValTemplate<std::string>

#define RectangleVector Template<std::vector<Rect>>
#define SystemVector Template<std::vector<ISystem*>>
#define GJKVector Template<std::vector<IGJK*>>
#define ColliderVector Template<std::vector<Component::Collider*>>
#define KeyboardArray Template<std::array<bool, MAX_KEYS>>
#define JsonMap Template<std::unordered_map<std::string, nlohmann::json>>
#define BoolMap Template<std::unordered_map<std::string, bool>>
#define CommandVector Template<std::vector<ICommand*>>
#define QuadTreeVector Template<std::vector<Component::QuadTree*>>