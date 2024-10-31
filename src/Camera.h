#pragma once

#include "Engine.h"

namespace GameCamera
{
    const float MIN_ZOOM = 0.5f;
    const float MAX_ZOOM = 2.0f;
    const float ZOOM_INCREMENT = 0.05f;

    struct Camera
    {
        Camera2D rlCamera;
        Vector2 target;
        Vector2 offset;
        Vector2 position;
        float moveSpeed = 500.0f;
        float lerpSpeed = 10.0f;
        float zoomTarget;
    };

    void Init(Camera& camera, Vector2 target, Vector2 offset, float zoom);
    void Update(Camera& camera);

    inline Vector2 GetWorldMousePosition(Camera& camera)
    {
        return GetScreenToWorld2D(GetMousePosition(), camera.rlCamera);
    }

}