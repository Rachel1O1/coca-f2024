#pragma once

#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include "Math.h"
#include "SDL2/SDL_mixer.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#define emscripten_cancel_main_loop()
#endif

class Game
{
public:
	const int ONE_GRID_UNIT = 32;
	const int WINDOW_WIDTH = 600;
	const int WINDOW_HEIGHT = 14*ONE_GRID_UNIT;
	const int MAP_WIDTH = 212*ONE_GRID_UNIT;
	const int GRID_START_X = 16;
	const int GRID_START_Y = 16;
	const float COUNTER_OFFSET_Y = -270.0f;
	const float COUNTER_OFFSET_X = 560.0f;
	const int LIVES_STARTING = 9;
	Game();
	bool Initialize();
	void EmRunIteration()
	{
		if (!mGameLoop)
		{
			emscripten_cancel_main_loop();
		}
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
	void RunLoop();
	void Shutdown();
	void AddActor(class Actor* addMe);
	void RemoveActor(class Actor* removeMe);
	SDL_Texture* GetTexture(std::string fileName);
	void AddSprite(class SpriteComponent* addMe);
	void RemoveSprite(class SpriteComponent* removeMe);
	void AddBlock(class Block* addMe);
	void RemoveBlock(class Block* removeMe);
	const std::vector<class Block*>& GetBlocks();
	const class Player* GetPlayer();
	Vector2& GetCameraPos();
	Mix_Chunk* GetSound(const std::string& filename);
	void PlaySoundOnce(const std::string& filename);
	void StopBGMusic(bool playerWins);
	void SetCounterPos(int x);
	void Reset();
	void TriggerReset() {mTriggerReset = true; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData(int numFade = 0);
	void UnloadData();
	std::vector<class Actor*> mActors;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	std::unordered_map<std::string, Mix_Chunk*> mSounds;
	std::vector<class SpriteComponent*> mSpriteComponents;
	std::vector<class Block*> mBlocks;
	SDL_Renderer* mGameRenderer;
	SDL_Window* mGameWindow;
	class Player* mPlayer;
	bool mGameLoop;
	Uint32 mPreviousTime;
	Vector2 mCameraPosition;
	int mMusicChannel;
	class UICounter* mCounter;
	int mCount;
	bool mTriggerReset;
};