#include "Level_1.h"

void Level_1::init()
{
	samples[0] = Sound::loadSample("../Resources/SoundFiles/rainforest.wav", BASS_SAMPLE_LOOP);	//adding sound files to the array to be played later in code
	samples[1] = Sound::loadSample("../Resources/SoundFiles/Collectable.wav", BASS_SAMPLE_OVER_POS);
	samples[2] = Sound::loadSample("../Resources/SoundFiles/checkpoint.wav", BASS_SAMPLE_OVER_POS);
	samples[3] = Sound::loadSample("../Resources/SoundFiles/endpoint.wav", BASS_SAMPLE_OVER_POS);

	Sound::playSample(samples[0]);

	for (int i = 0; i < MAX_COLLECTABLES; i++)
	{
		collectable[i]->init();
	}
	for (int i = 0; i < MAX_PLATFORMS; i++)
	{
		platform[i]->init();
	}
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemy[i]->init();
	}
	for (int i = 0; i < MAX_CHECKPOINTS; i++)
	{
		checkPoint[i]->init();
	}

	endPoint->init();
	endPoint->setPosition(glm::vec3(-3.0f, 3.0f, 90.0f));

	//proj_Enemy->init();

	platform[0]->setPosition(glm::vec3(-3.0f, 2.0f, 0.0f));
	platform[1]->setPosition(glm::vec3(-3.0f, 4.0f, 10.0f));
	platform[2]->setPosition(glm::vec3(-3.0f, 2.0f, 15.0f));
	platform[3]->setPosition(glm::vec3(-3.0f, 3.0f, 20.0f));
	platform[4]->setPosition(glm::vec3(-3.0f, 5.0f, 25.0f));

	collectable[0]->setPosition(glm::vec3(-3.0f, 3.0f, 30.0f));
	collectable[1]->setPosition(glm::vec3(-3.0f, 3.0f, 35.0f));
	collectable[2]->setPosition(glm::vec3(-3.0f, 3.0f, 40.0f));
	collectable[3]->setPosition(glm::vec3(-3.0f, 3.0f, 45.0f));
	collectable[4]->setPosition(glm::vec3(-3.0f, 3.0f, 60.0f));

	enemy[0]->setEnemyPosition(glm::vec3(-3.0f, 0.5f, 60.0f));
	enemy[1]->setEnemyPosition(glm::vec3(-3.0f, 0.5f, 70.0f));
	enemy[0]->setMinRange(glm::vec3(-3.0f, 0.0f, 55.0f));
	enemy[0]->setMaxRange(glm::vec3(-3.0f, 0.0f, 65.0f));
	enemy[1]->setMinRange(glm::vec3(-3.0f, 0.0f, 65.0f));
	enemy[1]->setMaxRange(glm::vec3(-3.0f, 0.0f, 75.0f));

	//proj_Enemy->setMinRange(glm::vec3(0.0f, 0.0f, 45.0f));
	//proj_Enemy->setMaxRange(glm::vec3(0.0f, 0.0f, 65.0f));

	checkPoint[0]->setPosition(glm::vec3(-3.0f, 3.0f, -300.0f));
	checkPoint[1]->setPosition(glm::vec3(-3.0f, 3.0f, 50.0f));

	player->init();
	bush->init();
	tree->init();
}

void Level_1::update(AbstractLevel** level)
{
	for (int i = 0; i < MAX_COLLECTABLES; i++)
	{
		if (Collision::collisionDetection(player, collectable[i]))
		{
			player->addCollectable();
			collectable[i]->setPosition(glm::vec3(collectable[i]->getPosition().x,
				collectable[i]->getPosition().y, collectable[i]->getPosition().z + 1000.0f));
						Sound::playSample(samples[1]);
		}
	}

	for (int i = 0; i < MAX_PLATFORMS; i++)
	{
		if (Collision::collisionDetection(player, platform[i]))
		{
			if (player->getPosition().y >= platform[i]->getPosition().y)
			{
				player->setPlatformCollision(false);
				player->setJump(false);
				Move::resetV();

				glm::vec3 position = player->getPosition();
				position.y = platform[i]->getPosition().y + 1.3f;

				if (!player->getJumping())
					player->setPlayerPosition(position);
			}
			else
				player->setPlatformCollision(true);
		}

	}

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		//CHECKING COLLISION FOR ENEMIES
		if (Collision::collisionDetection(player, enemy[i]))
		{
			if (player->getAllowDamage())
			{
				player->setEnemyCollision(false);
				player->setAllowDamage(false);
				player->takeDamage();
			}
		}
		else
			player->setEnemyCollision(true);

		enemy[i]->update();
	}

	player->update();
	//proj_Enemy->update();

	//if (player->getPosition().z == proj_Enemy->getPosition().z - 5
	//	|| player->getPosition().z == proj_Enemy->getPosition().z + 5)
	//{
	//	proj_Enemy->shoot();
	//}


	for (int i = 0; i < MAX_CHECKPOINTS; i++)
	{
		if (Collision::collisionDetection(player, checkPoint[i]))
		{
			player->setCheckPoint(checkPoint[i]->getPosition());
			Sound::playSample(samples[2]);
		}
	}
	
	if (Collision::collisionDetection(player, endPoint))
	{
		delete endPoint;
		delete* collectable;
		delete* platform;
		delete* enemy;
		lUpdate->update(level);
	}
}

void Level_1::draw(SDL_Window* window)
{
	display->draw(window);
	player->draw(window);
	endPoint->draw(window);
	//proj_Enemy->draw(window);

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemy[i]->draw(window);
	}
	for (int i = 0; i < MAX_COLLECTABLES; i++)
	{
		collectable[i]->draw(window);
	}
	for (int i = 0; i < MAX_PLATFORMS; i++)
	{
		platform[i]->draw(window);
	}
	for (int i = 0; i < MAX_CHECKPOINTS; i++)
	{
		checkPoint[i]->draw(window);
	}
	bush->draw(window);
	tree->draw(window);

	SDL_GL_SwapWindow(window);
}