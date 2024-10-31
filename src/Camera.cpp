#include "Camera.h"

void GameCamera::Update(Camera& camera)
{
    if (IsKeyDown(KEY_Q) && camera.zoomTarget != MIN_ZOOM)
    {
        camera.zoomTarget -= ZOOM_INCREMENT;
        camera.target = Vector2Lerp(camera.target, GetWorldMousePosition(camera), 0.25f);
    }
    if (IsKeyDown(KEY_E) && camera.zoomTarget != MAX_ZOOM)
    {
        camera.zoomTarget += ZOOM_INCREMENT;
        camera.target = Vector2Lerp(camera.target, GetWorldMousePosition(camera), 0.25f);
    }
    if (camera.zoomTarget < MIN_ZOOM)
    {
        camera.zoomTarget = MIN_ZOOM;
    }
    if (camera.zoomTarget > MAX_ZOOM)
    {
        camera.zoomTarget = MAX_ZOOM;
    }

    camera.rlCamera.zoom = Lerp(camera.rlCamera.zoom, camera.zoomTarget, GetFrameTime() * 10.0f);
    camera.rlCamera.target = Vector2Lerp(camera.rlCamera.target, camera.target, GetFrameTime() * 10.0f);
}
