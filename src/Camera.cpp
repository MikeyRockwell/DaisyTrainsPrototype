#include "Camera.h"

void GameCamera::Update(Camera& camera)
{
    if (IsKeyPressed(KEY_Q))
    {
        camera.zoomTarget -= ZOOM_INCREMENT;
    }
    if (IsKeyPressed(KEY_E))
    {
        camera.zoomTarget += ZOOM_INCREMENT;
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
