#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "SceneGame.hpp"
#include "bomberman.hpp"
#include "FileUtils.hpp"
#include "Save.hpp"
#include "ModelsManager.hpp"
#include "BoxCollider.hpp"

#include "Player.hpp"
#include "Wall.hpp"
#include "Crispy.hpp"
#include "Flag.hpp"
#include "End.hpp"

#include "EnemyBasic.hpp"
#include "EnemyFollow.hpp"
#include "EnemyWithEye.hpp"
#include "EnemyFly.hpp"
#include "EnemyCrispy.hpp"
#include "EnemyFrog.hpp"

#include "AudioManager.hpp"
#include "SceneManager.hpp"

/**
 * @brief Update function to be called in menu
 *
 * @return false If failed
 */
bool	SceneGame::updateForMenu() {
	/* set camera position for menu */
	_gui->cam->setMode(CamMode::STATIC);
	_gui->cam->pos = glm::vec3(0, 1.2, 2.5);
	_gui->cam->lookAt(glm::vec3(0, 0.7, 0));
	return true;
}

/**
 * @brief update is called each frame.
 *
 * @return true
 * @return false
 */
bool	SceneGame::update() {
	entitiesCount.enemy = -1;
	entitiesCount.staticElements = -1;
	entitiesCount.players = -1;
	entitiesCount.total = -1;

	if (level == NO_LEVEL)
		return true;

	if (_gui->cam->getMode() == CamMode::STATIC)
		_gui->cam->setMode(CamMode::STATIC_DEFPOS);

	_gui->cam->update(_dtTime);

	if (player->getState() == EntityState::DYING)
		state = GameState::GAME_OVER;

	allUI.introText->setEnabled(state == GameState::INTRO);

	if (Inputs::getKeyUp(InputType::CANCEL)) {
		if (state != GameState::INTRO && state != GameState::WIN && state != GameState::GAME_OVER)
			state = GameState::PAUSE;
		else
			_gui->disableExitForThisFrame();
	}

	if (Inputs::getKeyByScancodeUp(SDL_SCANCODE_H))
		_loadHelp = true;

	// set all enemies to Idle on win/loose
	if (state == GameState::WIN || state == GameState::GAME_OVER) {
		for (auto &&enemy : enemies) {
			enemy->alive = false;

			if (enemy->getState() != EntityState::IDLE) {
				enemy->setState(EntityState::IDLE);
				enemy->update();

				// reset EnemyCrispy rot/pos
				if (enemy->name == ENEMY_CRISPY_STR) {
					reinterpret_cast<EnemyCrispy *>(enemy)->setIdlePos();
				}
			}
		}
	}

	if (state == GameState::PAUSE || _loadHelp) {
		AudioManager::pauseAllSounds();
		if (_loadHelp) {
			AudioManager::resumeAllSounds();
			_loadHelp = false;
			state = GameState::PAUSE;
			// open help menu
			SceneManager::loadScene(SceneNames::HELP);
		}
		else {
			// open pause menu
			SceneManager::loadScene(SceneNames::PAUSE);
		}
		player->playPauseAnimation(false);
		return true;
	}
	else if (state == GameState::INTRO) {
		if (_gui->cam->isFollowFinished() || Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
			|| Inputs::getKeyUp(InputType::CANCEL)) {
			_gui->cam->setMode(CamMode::STATIC_DEFPOS);
			AudioManager::playMusic(musicLevel, 0.3f, true);
			AudioManager::stopSound(INTROLEVEL_SOUND, 500);
			state = GameState::PLAY;
			if (level == 0)
				_loadHelp = true;
		}
		return true;
	}
	else if (state == GameState::WIN) {
		if (_gui->cam->getMode() != CamMode::FOLLOW_PATH) {
			// calculate score and save it
			int32_t	crispiesLast = 0;
			for (auto &&box : board) {
				for (auto &&row : box) {
					for (auto &&element : row) {
						if (element->type == Type::CRISPY)
							crispiesLast++;
					}
				}
			}
			score.addBonusTime(levelTime, time);
			score.addBonusEnemies(levelEnemies, levelEnemies - enemiesKilled, levelCrispies, crispiesLast);
			Save::updateSavedFile(*this, true);
			Save::save(true);

			// manage winning sounds
			AudioManager::stopAllSounds();
			try {
				AudioManager::playSound(WIN_SOUND, 1.0f, false, true, 2000);
			} catch(Sound::SoundException const & e) {
				logErr(e.what());
			}

			player->startWinAnim();

			// start win camera animation
			_gui->cam->setMode(CamMode::FOLLOW_PATH);
			_gui->cam->setFollowPath(_getVictoryAnim());
		}

		player->update();

		// load victory menu on camera anim end
		if (_gui->cam->isFollowFinished() || Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
			|| Inputs::getKeyUp(InputType::CANCEL)) {
			delete player;
			player = nullptr;
			SceneManager::loadScene(SceneNames::VICTORY);
			return true;
		}

		return true;
	}
	else if (state == GameState::GAME_OVER) {
		if (_gui->cam->getMode() != CamMode::FOLLOW_PATH) {
			// manage loosing sounds
			AudioManager::stopAllSounds();
			try {
				AudioManager::playSound(GAME_OVER_SOUND, 1.0f, false, true, 2000);
			} catch(Sound::SoundException const & e) {
				logErr(e.what());
			}

			// start loose camera animation
			_gui->cam->setMode(CamMode::FOLLOW_PATH);
			_gui->cam->setFollowPath(_getGameOverAnim());

			if (player->getState() != EntityState::DYING) {
				player->setState(EntityState::LOOSE_EMOTE);
				player->update();
			}
		}

		// load loosing menu on camera anim end
		if (_gui->cam->isFollowFinished() || Inputs::getKeyUp(InputType::CONFIRM) || Inputs::getKeyUp(InputType::ACTION)
			|| Inputs::getKeyUp(InputType::CANCEL)) {
			// clear game infos.
			player->resetParams();
			Save::updateSavedFile(*this, false);
			Save::save(true);
			delete player;
			player = nullptr;
			SceneManager::loadScene(SceneNames::GAME_OVER);
		}

		return true;
	}

	// manage level time limit
	time += _dtTime;
	if ((levelTime - time) < 0) {
		state = GameState::GAME_OVER;
		return true;
	}
	// play alarm_time sound when there is not much time left
	if ((levelTime - time) < 20) {
		if (!_alarm) {
			try {
				AudioManager::playSound(ALARM_TIME_SOUND);
			} catch(Sound::SoundException const & e) {
				logErr(e.what());
			}
			_alarm = true;
		}
	}

	// switch to fps camera on key press
	#if DEBUG
		if (Inputs::getKeyByScancodeUp(SDL_SCANCODE_C)) {
			if (_gui->cam->getMode() == CamMode::FPS) {
				_gui->cam->setMode(CamMode::STATIC_DEFPOS);
			}
			else if (_gui->cam->getMode() == CamMode::STATIC_DEFPOS) {
				_gui->cam->setMode(CamMode::FPS);
			}
		}
		_gui->enableCursor(_gui->cam->getMode() != CamMode::FPS);  // disable cursor in FPS mode
	#endif

	// update board entities
	entitiesCount.staticElements = 0;
	for (auto &&board_it0 : board) {
		for (auto &&board_it1 : board_it0) {
			for (AEntity *board_it2 : board_it1) {
				entitiesCount.staticElements++;
				if (!board_it2->update())
					return false;
			}
		}
	}
	for (auto &&board_it0 : boardFly) {
		for (auto &&board_it1 : board_it0) {
			entitiesCount.staticElements += board_it1.size();
		}
	}
	entitiesCount.enemy = enemies.size();
	for (auto &&enemy : enemies) {
		if (!enemy->update())
			return false;
	}
	entitiesCount.staticElements += spawners.size();
	for (auto &&spawner : spawners) {
		if (!spawner->update())
			return false;
	}
	entitiesCount.players = 1;
	if (!player->update()) {
		return false;
	}

	entitiesCount.total =
			entitiesCount.enemy +
			entitiesCount.staticElements +
			entitiesCount.players;

	_updateGameInfos();

	return postUpdate();
}

/**
 * @brief postUpdate is called each frame, after update.
 *
 * @return true
 * @return false
 */
bool	SceneGame::postUpdate() {
	player->postUpdate();
	auto enemy = enemies.begin();
	while (enemy != enemies.end()) {
		if (!(*enemy)->postUpdate()) {
			enemy = enemies.begin();
			continue;
		}
		enemy++;
	}

	for (auto &&board_it0 : board) {
		for (auto &&board_it1 : board_it0) {
			std::vector<AEntity *>::iterator it = board_it1.begin();
			AEntity * copy;
			while (it != board_it1.end()) {
				copy = *it;
				if (!(*it)->postUpdate()) {
					it = board_it1.begin();
					continue;
				}
				if (it == board_it1.end())
					continue;
				if (copy == *it)
					it++;
			}
		}
	}

	return true;
}

/**
 * @brief Update game informations
 */
void	SceneGame::_updateGameInfos() {
	glm::vec2	winSz = _gui->gameInfo.windowSize;
	glm::vec2	tmpPos;
	float		imgY;
	float		textY;
	glm::vec2	tmpSize;
	uint32_t	padding = 5;
	float		menuWidth = winSz.x / 2;
	float		menuHeight = menuWidth / 8;

	try {
		tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
		tmpPos.y = winSz.y - menuHeight * 2;
		imgY = tmpPos.y;
		textY = tmpPos.y + 2;
		tmpSize.x = menuWidth;
		tmpSize.y = menuHeight;
		tmpSize = {32, 32};

		// -- Title ---------
		/* level name */
		allUI.levelNameText->setText(getLevelName(level))
			.setSize(VOID_POS).setCalculatedSize();
		allUI.levelNameText->setPos({(winSz.x / 2) - allUI.levelNameText->getSize().x / 2, tmpPos.y + 1.5 * tmpSize.y});

		tmpPos.x = (winSz.x / 2) - (menuWidth / 2);
		tmpPos.y = winSz.y - menuHeight * 2;

		// -- Top -----------
		/* time left */
		if (player->activeBonus.time > 0) {
			allUI.timeLeftImgActive->setPos({tmpPos.x, imgY}).setSize(tmpSize);
			allUI.timeLeftImg->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.timeLeftImgActive->getSize().x;
		}
		else {
			allUI.timeLeftImg->setPos({tmpPos.x, imgY}).setSize(tmpSize);
			allUI.timeLeftImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.timeLeftImg->getSize().x;
		}
		allUI.timeLeftText->setPos({tmpPos.x, textY}).setText(timeToString(levelTime - time))
			.setSize(VOID_POS).setCalculatedSize();
		if (!_alarm)
			allUI.timeLeftText->setTextColor(colorise(s.j("colors").j("font").u("color")));
		else
			allUI.timeLeftText->setTextColor(colorise(s.j("colors").j("orange").u("color")));
		tmpPos.x += allUI.timeLeftText->getSize().x;

		/* life */
		tmpPos.x += padding;
		if (player->activeBonus.life > 0) {
			allUI.lifeImgActive->setPos({tmpPos.x, imgY}).setSize(tmpSize);
			allUI.lifeImg->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.lifeImgActive->getSize().x;
		}
		else {
			allUI.lifeImg->setPos({tmpPos.x, imgY}).setSize(tmpSize);
			allUI.lifeImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.lifeImg->getSize().x;
		}
		allUI.lifeText->setPos({tmpPos.x, textY}).setText(std::to_string(player->lives))
			.setSize(VOID_SIZE).setCalculatedSize();
		tmpPos.x += allUI.lifeText->getSize().x;

		/* enemies counter */
		if (enemiesToKill > 0) {
			tmpPos.x += padding;
			std::string enemiesStr = std::to_string(enemiesKilled) + "/" + std::to_string(enemiesToKill);
			glm::vec4 color = enemiesKilled >= enemiesToKill
												? colorise(0x39A0DD)
												: colorise(0xBF001E);
			allUI.enemiesCounterText->setPos({tmpPos.x, textY}).setText(enemiesStr)
				.setSize(VOID_POS).setCalculatedSize().setTextColor(color);
			tmpPos.x += allUI.enemiesCounterText->getSize().x;
		}

		/* score */
		tmpPos.x = (winSz.x / 2) + (menuWidth / 2);
		allUI.scoreText->setText(score.toString()).setSize(VOID_POS).setCalculatedSize();
		tmpPos.x -= allUI.scoreText->getSize().x;
		allUI.scoreText->setPos({tmpPos.x, textY});
		if (player->activeBonus.score > 0) {
			allUI.scoreImgActive->setSize(tmpSize);
			tmpPos.x -= allUI.scoreImgActive->getSize().x;
			allUI.scoreImgActive->setPos({tmpPos.x, imgY});
			allUI.scoreImg->setPos(VOID_POS).setSize(VOID_SIZE);
		}
		else {
			allUI.scoreImg->setSize(tmpSize);
			tmpPos.x -= allUI.scoreImg->getSize().x;
			allUI.scoreImg->setPos({tmpPos.x, imgY});
			allUI.scoreImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
		}

		// -- Bonus -----------
		glm::vec2 pos = {winSz.x / 10, winSz.y - menuHeight * 4};
		tmpPos = pos;

		/* speed */
		// tmpPos.x += padding;
		if (player->activeBonus.speed > 0) {
			allUI.speedImgActive->setPos(tmpPos).setSize(tmpSize);
			allUI.speedImg->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.speedImgActive->getSize().x;
		}
		else {
			allUI.speedImg->setPos(tmpPos).setSize(tmpSize);
			allUI.speedImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.speedImg->getSize().x;
		}
		std::string	speed = std::to_string(player->speed);
		speed = speed.substr(0, speed.find("."));
		allUI.speedText->setPos({tmpPos.x, tmpPos.y + 2}).setText(speed)
			.setSize(VOID_SIZE).setCalculatedSize();
		pos.y -= menuHeight * 0.8;
		tmpPos = pos;

		/* bonus bomb */
		if (player->activeBonus.bombs > 0) {
			allUI.bonusBombImgActive->setPos(tmpPos).setSize(tmpSize);
			allUI.bonusBombImg->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.bonusBombImgActive->getSize().x;
		}
		else {
			allUI.bonusBombImg->setPos(tmpPos).setSize(tmpSize);
			allUI.bonusBombImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.bonusBombImg->getSize().x;
		}
		allUI.bonusBombText->setPos({tmpPos.x, tmpPos.y + 2}).setText(std::to_string(player->totalBombs))
			.setSize(VOID_SIZE).setCalculatedSize();
		pos.y -= menuHeight * 0.8;
		tmpPos = pos;

		/* bonus flame */
		if (player->activeBonus.flames > 0) {
			allUI.bonusFlameImgActive->setPos(tmpPos).setSize(tmpSize);
			allUI.bonusFlameImg->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.bonusFlameImgActive->getSize().x;
		}
		else {
			allUI.bonusFlameImg->setPos(tmpPos).setSize(tmpSize);
			allUI.bonusFlameImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			tmpPos.x += allUI.bonusFlameImg->getSize().x;
		}
		allUI.bonusFlameText->setPos({tmpPos.x, tmpPos.y + 2}).setText(std::to_string(player->bombProgation))
			.setSize(VOID_SIZE).setCalculatedSize();
		pos.y -= menuHeight * 0.8;
		tmpPos = pos;

		/* bonus flampass */
		if (player->passFire) {
			if (player->activeBonus.flampass > 0) {
				allUI.bonusFlampassImgActive->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusFlampassImg->setPos(VOID_POS).setSize(VOID_SIZE);
			}
			else {
				allUI.bonusFlampassImg->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusFlampassImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			}
			pos.y -= menuHeight * 0.8;
			tmpPos = pos;
		} else {
			allUI.bonusFlampassImg->setPos(VOID_POS).setSize(VOID_SIZE);
		}
		/* bonus wallpass */
		if (player->passWall) {
			if (player->activeBonus.wallpass > 0) {
				allUI.bonusWallpassImgActive->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusWallpassImg->setPos(VOID_POS).setSize(VOID_SIZE);
			}
			else {
				allUI.bonusWallpassImg->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusWallpassImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			}
			pos.y -= menuHeight * 0.8;
			tmpPos = pos;
		} else {
			allUI.bonusWallpassImg->setPos(VOID_POS).setSize(VOID_SIZE);
		}
		/* bonus detonator */
		if (player->detonator) {
			if (player->activeBonus.detonator > 0) {
				allUI.bonusDetonatorImgActive->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusDetonatorImg->setPos(VOID_POS).setSize(VOID_SIZE);
			}
			else {
				allUI.bonusDetonatorImg->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusDetonatorImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			}
			pos.y -= menuHeight * 0.8;
			tmpPos = pos;
		} else {
			allUI.bonusDetonatorImg->setPos(VOID_POS).setSize(VOID_SIZE);
		}
		/* bonus passBomb */
		if (player->passBomb) {
			if (player->activeBonus.bombpass > 0) {
				allUI.bonusBombpassImgActive->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusBombpassImg->setPos(VOID_POS).setSize(VOID_SIZE);
			}
			else {
				allUI.bonusBombpassImg->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusBombpassImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
			}
			pos.y -= menuHeight * 0.8;
			tmpPos = pos;
		} else {
			allUI.bonusBombpassImg->setPos(VOID_POS).setSize(VOID_SIZE);
		}
		/* bonus invulnerable */
		if (player->invulnerable) {
			if (player->activeBonus.shield > 0) {
				allUI.bonusShieldImgActive->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusShieldImg->setPos(VOID_POS).setSize(VOID_SIZE);
				tmpPos.x += allUI.bonusShieldImgActive->getSize().x;
			}
			else {
				allUI.bonusShieldImg->setPos(tmpPos).setSize(tmpSize);
				allUI.bonusShieldImgActive->setPos(VOID_POS).setSize(VOID_SIZE);
				tmpPos.x += allUI.bonusShieldImg->getSize().x;
			}
			std::string	invulnerable = std::to_string(player->invulnerable);
			invulnerable = invulnerable.substr(0, invulnerable.find(".")+2);
			allUI.bonusShieldText->setPos({tmpPos.x, tmpPos.y + 2}).setText(timeToString(player->invulnerable))
				.setSize(VOID_SIZE).setCalculatedSize();
		} else {
			allUI.bonusShieldImg->setPos(VOID_POS).setSize(VOID_SIZE);
			allUI.bonusShieldText->setPos(VOID_POS).setSize(VOID_SIZE);
		}
	} catch (ABaseUI::UIException const & e) {
		logErr(e.what());
	}
}

/**
 * @brief Update the blur Mask texture
 *
 * @param aMaskData mask texture raw data
 * @return true on success
 * @return false on failure
 */
bool	SceneGame::updateBlurMaskTex(std::vector<uint8_t> const &aMaskData) {
	glm::vec2 winSize = _gui->gameInfo.windowSize;

	// create the texture the first time
	if (_blurMaskTex == 0) {
		glGenTextures(1, &_blurMaskTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _blurMaskTex);
		// create the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, winSize.x, winSize.y, 0, GL_RED,
			GL_UNSIGNED_BYTE, &aMaskData[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
		// tell
		_blurShader->use();
		_blurShader->setInt("sceneTex", 0);
		_blurShader->setInt("blurMaskTex", 1);
		_blurShader->unuse();
	}
	// else just update the data
	else {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _blurMaskTex);
		// update texture data
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, winSize.x, winSize.y, GL_RED,
			GL_UNSIGNED_BYTE, &aMaskData[0]);
		glBindTexture( GL_TEXTURE_2D, 0);
	}
	glActiveTexture(GL_TEXTURE0);

	return true;
}
