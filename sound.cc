#include "sound.h"
#include <SDL2/SDL_audio.h>
#include <iostream>

SDL_AudioDeviceID audio_dev;

void LazyInit();

struct Sound {
  Uint8* audio_buf = 0;
  Uint32 audio_len = 0;

  void Play() {
    if (!audio_buf) {
      LazyInit();
    }
    SDL_PauseAudioDevice(audio_dev, 0);

    SDL_ClearQueuedAudio(audio_dev);
    SDL_QueueAudio(audio_dev, audio_buf, audio_len);
  }
};

Sound sounds[NUM_SOUNDS];

void DSound_Play(SoundID sound_id) {
  sounds[sound_id].Play();
}

void LazyInit() {
  std::cout << "Init sounds\n";
  SDL_AudioSpec desired_audio_spec = {0};
  desired_audio_spec.freq = 44100;
  desired_audio_spec.channels = 2;
  desired_audio_spec.samples = 2048;
  desired_audio_spec.format = AUDIO_U8;
  desired_audio_spec.callback = 0;

  SDL_AudioSpec audio_spec;
  audio_dev =
      SDL_OpenAudioDevice(nullptr, 0, &desired_audio_spec, &audio_spec, 0);

  SDL_LoadWAV("sounds/chomp.wav", &audio_spec,
              &sounds[chompID].audio_buf,
              &sounds[chompID].audio_len);
  SDL_LoadWAV("sounds/return.wav", &audio_spec,
              &sounds[returnID].audio_buf,
              &sounds[returnID].audio_len);
  SDL_LoadWAV("sounds/die.wav", &audio_spec,
              &sounds[dieID].audio_buf,
              &sounds[dieID].audio_len);
  SDL_LoadWAV("sounds/eatfruit.wav", &audio_spec,
              &sounds[eatFruitID].audio_buf,
              &sounds[eatFruitID].audio_len);
  SDL_LoadWAV("sounds/eatghost.wav", &audio_spec,
              &sounds[eatGhostID].audio_buf,
              &sounds[eatGhostID].audio_len);
  SDL_LoadWAV("sounds/weak.wav", &audio_spec,
              &sounds[weakID].audio_buf,
              &sounds[weakID].audio_len);
  SDL_LoadWAV("sounds/start.wav", &audio_spec,
              &sounds[startID].audio_buf,
              &sounds[startID].audio_len);
  std::cout << "Done init audio\n";
}
