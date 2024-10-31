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
	const int WINDOW_WIDTH = 600;
	const int WINDOW_HEIGHT = 448;
	const int MAP_WIDTH = 6784;
	const int ONE_GRID_UNIT = 32;
	const int GRID_START_X = 16;
	const int GRID_START_Y = 16;
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

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
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
};