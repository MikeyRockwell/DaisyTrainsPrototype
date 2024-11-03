#include "Audio.h"

namespace Audio
{
    MusicPlayer musicPlayer;
    Resources resources;

    void Init()
    {
        InitAudioDevice();
        
        musicPlayer.tracks[0] = LoadMusicStream("res/music/track_01.ogg");
        musicPlayer.tracks[1] = LoadMusicStream("res/music/track_02.ogg");
        musicPlayer.tracks[2] = LoadMusicStream("res/music/track_03.ogg");
        musicPlayer.tracks[3] = LoadMusicStream("res/music/track_04.ogg");
        musicPlayer.tracks[4] = LoadMusicStream("res/music/track_05.ogg");
        musicPlayer.tracks[5] = LoadMusicStream("res/music/track_06.ogg");

        SetMusicVolume(musicPlayer.tracks[0], 0.50f);
        SetMusicVolume(musicPlayer.tracks[1], 0.50f);
        SetMusicVolume(musicPlayer.tracks[2], 0.50f);
        SetMusicVolume(musicPlayer.tracks[3], 0.50f);
        SetMusicVolume(musicPlayer.tracks[4], 0.50f);
        SetMusicVolume(musicPlayer.tracks[5], 0.50f);

        resources.place_mine.sound    = LoadSound("res/sfx/place_mine.ogg");
        resources.place_station.sound = LoadSound("res/sfx/place_station.ogg");
        resources.place_train.sound   = LoadSound("res/sfx/place_train.ogg");
        resources.remove_track.sound  = LoadSound("res/sfx/remove_track.ogg");
        resources.remove_train.sound  = LoadSound("res/sfx/train_remove.ogg");
        resources.unlock_level.sound  = LoadSound("res/sfx/unlock_level.ogg");
        resources.place_rail[0].sound = LoadSound("res/sfx/place_track_01.ogg");
        resources.place_rail[1].sound = LoadSound("res/sfx/place_track_02.ogg");
        resources.place_rail[2].sound = LoadSound("res/sfx/place_track_03.ogg");
        resources.train_running.sound = LoadSound("res/sfx/train_running.ogg");
        
        PlayMusic();
    }

    void PlayMusic()
    {
        i32 random = GetRandomValue(0, MAX_MUSIC_TRACKS - 1);
        PlayMusicStream(musicPlayer.tracks[random]);
        musicPlayer.currentTrack = random;
        musicPlayer.currentTrackTime = 0.0f;
        musicPlayer.trackDuration = GetMusicTimeLength(musicPlayer.tracks[random]);
        //musicPlayer.trackDuration = 5.0f;
    }

    void PlaySFXRandom(Sound& sound)
    {
        float pitch = 1.0f + GetRandomValue(-10, 10) / 100.0f;
        SetSoundPitch(sound, pitch);
        PlaySound(sound);
    }

    void StopSound()
    {

    }

    void Update()
    {
        if (IsKeyPressed(KEY_M))
        {
            if (musicPlayer.paused)
            {
                PlayMusicStream(musicPlayer.tracks[musicPlayer.currentTrack]);
            }
            else
            {
                PauseMusicStream(musicPlayer.tracks[musicPlayer.currentTrack]);
            }
            musicPlayer.paused = !musicPlayer.paused;
        }

        if (!musicPlayer.paused)
        {
            musicPlayer.currentTrackTime += GetFrameTime();
            if (musicPlayer.currentTrackTime > musicPlayer.trackDuration)
            {
                StopMusicStream(musicPlayer.tracks[musicPlayer.currentTrack]);
                musicPlayer.currentTrack++;
                if (musicPlayer.currentTrack >= MAX_MUSIC_TRACKS)
                {
                    musicPlayer.currentTrack = 0;
                }
                musicPlayer.currentTrackTime = 0.0f;
                PlayMusicStream(musicPlayer.tracks[musicPlayer.currentTrack]);

            }
            UpdateMusicStream(musicPlayer.tracks[musicPlayer.currentTrack]);
        }
    }

    void Clean()
    {
        for (i32 i = 0; i < MAX_MUSIC_TRACKS; i++)
        {
            UnloadMusicStream(musicPlayer.tracks[i]);
        }
        CloseAudioDevice();
    }
}
