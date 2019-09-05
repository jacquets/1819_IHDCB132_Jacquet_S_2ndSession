/**
 *      Filename : audio.c
 *      Description:  audio function.
 *      Created:  21-07-19 | Revision: --.
 *      Compiler:  GNU Compiler - GCC.
 *      Author:  Samuel Jacquet
 */

#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <FMOD/fmod.h>

#define MAX_SOUND 12

#define SOUND_FILE_PATH_00 "../../data/song/breakout.mp3"
#define SOUND_FILE_PATH_01 "../../data/song/Remix_DaBaDee-IAmBlue.mp3"
#define SOUND_FILE_PATH_02 "../../data/song/Remix_LinkinPark-InTheEnd.mp3"
#define SOUND_FILE_PATH_03 "../../data/sound/monkeyscream.wav"
#define SOUND_FILE_PATH_04 "../../data/sound/jump.wav"
#define SOUND_FILE_PATH_05 "../../data/sound/pan.wav"
#define SOUND_FILE_PATH_06 "../../data/sound/jaguar.wav"
#define SOUND_FILE_PATH_07 "../../data/sound/parrot.wav"
#define SOUND_FILE_PATH_08 "../../data/sound/jungle_drum.wav"
#define SOUND_FILE_PATH_09 "../../data/sound/jungle01.mp3"
#define SOUND_FILE_PATH_10 "../../data/sound/jungle02.wav"
#define SOUND_FILE_PATH_11 "../../data/sound/jungle03.mp3"

/* Definitions.*/
//-

/* Prototypes. */
void FMOD_Check(FMOD_RESULT result);
void InitAudio(void);
void playAudio(unsigned int SoundID, FMOD_BOOL isPlaying);
void pauseAudio(FMOD_CHANNEL *channel);
void stopAudio(void);
void FMOD_Version(void);

/* FMOD prototypes. */
// FMOD_RESULT FMOD_System_Create(FMOD_SYSTEM **system);
// FMOD_RESULT FMOD_System_Init(FMOD_SYSTEM *system,int maxchannels,FMOD_INITFLAGS flags,void *extradriverdata);
// FMOD_RESULT FMOD_System_CreateSound(FMOD_SYSTEM *system,const char *name_or_data,FMOD_MODE mode,FMOD_CREATESOUNDEXINFO *exinfo,FMOD_SOUND **sound);
// FMOD_RESULT FMOD_Sound_SetLoopCount(FMOD_SOUND *sound,int loopcount);
// FMOD_RESULT FMOD_System_PlaySound(FMOD_SYSTEM *system,FMOD_SOUND *sound,FMOD_CHANNELGROUP *channelgroup,FMOD_BOOL paused,FMOD_CHANNEL **channel);
// FMOD_RESULT FMOD_Channel_IsPlaying(FMOD_CHANNEL *channel,FMOD_BOOL *isplaying);
// FMOD_RESULT FMOD_Sound_Release(FMOD_SOUND *sound);
// FMOD_RESULT FMOD_System_Close(FMOD_SYSTEM *system);
// FMOD_RESULT FMOD_System_Release(FMOD_SYSTEM *system);
// FMOD_RESULT FMOD_System_GetVersion(FMOD_SYSTEM *system,unsigned int *version);

#endif
