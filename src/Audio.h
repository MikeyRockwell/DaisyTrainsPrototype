#pragma once

#include "Engine.h"

namespace Audio
{
    const i32 MAX_MUSIC_TRACKS = 6;
    const i32 PLACE_RAIL_SFX = 3;

    struct SFX
    {
        Sound sound;
        bool playing = false;
    };

    struct MusicPlayer
    {
        i32 currentTrack = 0;
        Music tracks[MAX_MUSIC_TRACKS];
        float currentTrackTime = 0.0f;
        float trackDuration = 0.0f;
        bool paused = false;
    };
    extern MusicPlayer musicPlayer;

    struct Resources
    {
        SFX train_horn;
        SFX place_mine;
        SFX place_station;
        SFX place_rail[PLACE_RAIL_SFX];
        SFX place_train;
        SFX remove_track;
        SFX remove_train;
        SFX unlock_level;
        SFX train_running;
    };
    extern Resources resources;

    void Init();
    void PlayMusic();
    void PlaySFXRandom(Sound& sound);
    void StopSound();

    void Update();
    void Clean();

}