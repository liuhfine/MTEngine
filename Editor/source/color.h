#pragma once
#include "meta/reflection/reflection.h"

namespace Piccolo
{
    /*REFLECTION_TYPE(Vector3)
        CLASS(Vector3, Fields)
    {
        REFLECTION_BODY(Vector3);

    public:
        float x{ 0.f };
        float y{ 0.f };
        float z{ 0.f };

    public:
        Vector3() = default;
        Vector3(float x_, float y_, float z_) : x{ x_ }, y{ y_ }, z{ z_ } {}
    };*/

    class Vector3
    {
    public:
		float x{};
		float y{};
		float z{};

        Vector3() = default;
        Vector3(float x_, float y_, float z_) : x{ x_ }, y{ y_ }, z{ z_ } {}
    };

    REFLECTION_TYPE(Color)
    CLASS(Color, Fields)
    {
        REFLECTION_BODY(Color);

    public:
        float r;
        float g;
        float b;

        bool isValid() const { return true; };
        Vector3 toVector3() const { return Vector3(r, g, b); }
    };
}