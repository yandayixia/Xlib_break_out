#ifndef __GAME_MANAGER__
#define __GAME_MANAGER__

#include <list>
#include <iostream>
#include "GameObject.h"
#include "BaseBlock.h"
#include "IronBlock.h"
#include "Paddle.h"
#include "Orb.h"

class BaseBlock;
class Orb;
class Paddle;

class GameManager{
public:
	//******spawning methods
	static void SpawnBlock(BaseBlock* target);
	static void SpawnPaddle();
	//overlaod
	static Orb* SpawnOrb();
	static void SpawnOrb(Orb* target);
	
	//******removing methods
	static void RemoveBlock(std::list<BaseBlock*>::iterator& n);

	//******functional methods
	static void InitNewGame(XInfo& myInfo);
	static void AddPoints(int amount);
	//fix the posistion of target on to hero Paddle
	static void StickBallToPaddle(Orb* target);
	//DropOrb should update 
	static void DropOrb();
	/*The compound method for checking:
	 *=> Fallen balls
	 *=> Lose Condition
	 *=> Win Condition
	 *=> Event trigging
	*/
	static void UpdateManager();
	static void InitTargetFPS(double target);

	//cal upon victory
	static void NextLevel();
	static void SetLevel(int target = -1);
	static void ClearResource();
	static void LoadLevelData();

	//******data:
	static bool RunningGame;
	static int ExtraBall;
	static int GameScore;
	static int PowerUpLimit;

	static int LevelCount;
	static int BonusLifeScore;
	static int ToNextLife;

	static long double Block_Spacing;	
	static long double Ball_SPD; 
	static long double Target_FPS;
	static long double FPS;//pixel per frame
	
	//6*9 matrix in the standard game
	static std::list<BaseBlock*> BlockArray;
	//only 1 paddle at 1 time
	static Paddle* HeroPaddle;
	//may have more than 1 orb
	static std::list<Orb*> OrbArray;

private:
	static bool Trash_Orb;
	static int Block_Need_Break;
	//if Trash_orb is true, delete all the out of screen balls.
	static void TrashOrb();

	static XInfo* XInfo_ptr;
};

#endif