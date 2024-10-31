//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SpriteComponent.h"
#include "Random.h"
#include <fstream>
#include "CollisionComponent.h"
#include "Block.h"
#include "Player.h"

Game::Game()
{
	mGameRenderer = nullptr;
	mGameWindow = nullptr;
	mGameLoop = true;
	mPreviousTime = 0;
	mPlayer = nullptr;
	mCameraPosition = Vector2(0.0f, 0.0f);
	mMusicChannel = 0;
}

bool Game::Initialize()
{
	bool correctlyInitializing = true;

	Random::Init();

	//initialize SDL
	int resultingValue = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO);
	if (resultingValue != 0)
	{
		correctlyInitializing = false;
	}

	//create game window if SDL init worked
	if (correctlyInitializing)
	{
		mGameWindow = SDL_CreateWindow("Cat Game", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
		if (mGameWindow == NULL)
		{
			correctlyInitializing = false;
		}
	}

	//create renderer if other steps worked
	if (correctlyInitializing)
	{
		mGameRenderer = SDL_CreateRenderer(mGameWindow, -1,
										   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (mGameRenderer == NULL)
		{
			correctlyInitializing = false;
		}
	}

	IMG_Init(IMG_INIT_PNG);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	LoadData();

	return correctlyInitializing;
}

void Game::RunLoop()
{
	while (mGameLoop)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	//check to stop game
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			mGameLoop = false;
		}
	}
	int numkeys = 0;
	Uint8* keysArray = const_cast<Uint8*>(SDL_GetKeyboardState(&numkeys));
	if (keysArray[SDL_SCANCODE_ESCAPE])
	{
		mGameLoop = false;
	}

	//check player input
	std::vector<class Actor*> actorsCopy = mActors;
	size_t copySize = actorsCopy.size();
	for (size_t i = 0; i < copySize; i++)
	{
		actorsCopy.at(i)->ProcessInput(keysArray);
	}
}

void Game::UpdateGame()
{
	//calculate delta time (frame limiting and capping max delta included)
	Uint32 currentTime = SDL_GetTicks();
	Uint32 timeDifference = (currentTime - mPreviousTime);
	while (timeDifference < 16)
	{
		currentTime = SDL_GetTicks();
		timeDifference = (currentTime - mPreviousTime);
	}
	if (timeDifference > 33)
	{
		timeDifference = 33;
	}
	mPreviousTime = currentTime;
	float deltaTime = (timeDifference / 1000.0f);

	std::vector<class Actor*> actorsCopy = mActors;
	size_t copySize = actorsCopy.size();
	for (size_t i = 0; i < copySize; i++)
	{
		actorsCopy.at(i)->Update(deltaTime);
	}
	std::vector<class Actor*> actorsToDestroy;
	for (int i = 0; i < copySize; i++)
	{
		if (actorsCopy.at(i)->GetState() == ActorState::Destroy)
		{
			actorsToDestroy.push_back(actorsCopy.at(i));
		}
	}
	size_t actorsToDestroySize = actorsToDestroy.size();
	for (size_t i = 0; i < actorsToDestroySize; i++)
	{
		delete actorsToDestroy.at(i);
	}
}

void Game::GenerateOutput()
{
	//draw background
	SDL_SetRenderDrawColor(mGameRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(mGameRenderer);

	//draw sprites if they're visible
	size_t spriteSize = mSpriteComponents.size();
	for (size_t i = 0; i < spriteSize; i++)
	{
		if (mSpriteComponents.at(i)->IsVisible())
		{
			mSpriteComponents.at(i)->Draw(mGameRenderer);
		}
	}

	//render it all
	SDL_RenderPresent(mGameRenderer);
}

void Game::AddActor(class Actor* addMe)
{
	mActors.push_back(addMe);
}

void Game::RemoveActor(class Actor* removeMe)
{
	mActors.erase(std::find(mActors.begin(), mActors.end(), removeMe));
}

void Game::LoadData()
{
	//Play bg music
	mMusicChannel = Mix_PlayChannel(-1, GetSound("Assets/Sounds/Music.ogg"), -1);

	//Adds background
	Actor* background = new Actor(this);
	background->SetPosition(Vector2((MAP_WIDTH / 2), (WINDOW_HEIGHT / 2)));
	SpriteComponent* sc = new SpriteComponent(background, 50);
	sc->SetTexture(GetTexture("Assets/Background.png"));

	//Loads in objects
	std::ifstream reader("Assets/Level1.txt");
	char currentChar = ' ';
	int currentX = GRID_START_X;
	int currentY = GRID_START_Y;
	std::string input;
	while (std::getline(reader, input))
	{
		size_t inputSize = input.length();
		for (size_t i = 0; i < inputSize; i++)
		{
			currentChar = input[i];
			if ((currentChar >= 'A') && (currentChar <= 'I'))
			{
				Block* b = new Block(this, currentChar);
				b->SetPosition(Vector2(currentX, currentY));
			}
			else if (currentChar == 'P')
			{
				Player* p = new Player(this);
				p->SetPosition(Vector2(currentX, currentY));
				mPlayer = p;
			}

			currentX += ONE_GRID_UNIT;
		}
		currentX = GRID_START_X;
		currentY += ONE_GRID_UNIT;
	}
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	if (!mTextures.empty())
	{
		for (auto& currentTexture : mTextures)
		{
			SDL_DestroyTexture(currentTexture.second);
		}
	}
	mTextures.clear();
	if (!mSounds.empty())
	{
		for (auto& currentSound : mSounds)
		{
			Mix_FreeChunk(currentSound.second);
		}
	}
	mSounds.clear();
}

SDL_Texture* Game::GetTexture(std::string fileName)
{
	if (mTextures.find(fileName) != mTextures.end())
	{
		return mTextures[fileName];
	}
	else
	{
		SDL_Surface* resultFromLoad = IMG_Load(fileName.c_str());
		SDL_Texture* resultingTexture = SDL_CreateTextureFromSurface(mGameRenderer, resultFromLoad);
		SDL_FreeSurface(resultFromLoad);
		mTextures.insert({fileName, resultingTexture});
		return resultingTexture;
	}
}

void Game::AddSprite(class SpriteComponent* addMe)
{
	mSpriteComponents.push_back(addMe);
	std::sort(mSpriteComponents.begin(), mSpriteComponents.end(),
			  [](SpriteComponent* a, SpriteComponent* b) {
				  return a->GetDrawOrder() < b->GetDrawOrder();
			  });
}

void Game::RemoveSprite(class SpriteComponent* removeMe)
{
	mSpriteComponents.erase(
		std::find(mSpriteComponents.begin(), mSpriteComponents.end(), removeMe));
}

void Game::AddBlock(class Block* addMe)
{
	mBlocks.push_back(addMe);
}

void Game::RemoveBlock(class Block* removeMe)
{
	mBlocks.erase(std::find(mBlocks.begin(), mBlocks.end(), removeMe));
}

const std::vector<class Block*>& Game::GetBlocks()
{
	return mBlocks;
}

const class Player* Game::GetPlayer()
{
	return mPlayer;
}

Vector2& Game::GetCameraPos()
{
	return mCameraPosition;
}

void Game::PlaySoundOnce(const std::string& filename)
{
	Mix_PlayChannel(-1, GetSound(filename), 0);
}

Mix_Chunk* Game::GetSound(const std::string& filename)
{
	if (mSounds.find(filename) != mSounds.end())
	{
		return mSounds[filename];
	}
	else
	{
		Mix_Chunk* resultingMixChunk = Mix_LoadWAV(filename.c_str());
		mSounds.insert({filename, resultingMixChunk});
		return resultingMixChunk;
	}
}

void Game::StopBGMusic(bool playerWins)
{
	Mix_HaltChannel(mMusicChannel);
	if (playerWins)
	{
		PlaySoundOnce("Assets/Sounds/StageClear.wav");
	}
	else
	{
		PlaySoundOnce("Assets/Sounds/Dead.wav");
	}
}

void Game::Shutdown()
{
	UnloadData();
	Mix_CloseAudio();
	IMG_Quit();
	SDL_DestroyRenderer(mGameRenderer);
	SDL_DestroyWindow(mGameWindow);
	SDL_Quit();
}