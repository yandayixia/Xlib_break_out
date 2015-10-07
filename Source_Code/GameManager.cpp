#include "GameManager.h"

int GameManager::ExtraBall = 99;
int GameManager::GameScore = -1;
int GameManager::PowerUpLimit = 20;
int GameManager::LevelCount = -1;
int GameManager::Block_Need_Break = 999;
int GameManager::BonusLifeScore = 1000;
int GameManager::ToNextLife = 0;

long double GameManager::Block_Spacing = -1;

long double GameManager::Target_FPS = 60.0f;
//fake the first frame
long double GameManager::FPS = GameManager::Target_FPS;
long double GameManager::Ball_SPD = 6.0f;

bool GameManager::Trash_Orb = false;
bool GameManager::RunningGame = false;
XInfo* GameManager::XInfo_ptr = NULL;


//6*9 matrix in the standard game
std::list<BaseBlock*> GameManager::BlockArray;

//only 1 paddle at 1 time
Paddle* GameManager::HeroPaddle = NULL;

//may have more than 1 orb
std::list<Orb*> GameManager::OrbArray;

void GameManager::SpawnBlock(BaseBlock* target){
	if(target->_required){
		Block_Need_Break++;	
	}

	GameManager::BlockArray.push_back(target);
}


Orb* GameManager::SpawnOrb(){
	Orb* result = NULL;
	int _radius = 10;

	//do not spawn if life count < 0
	if(GameManager::ExtraBall < 0){
		throw "trying to creat orb while life count smaller than 0";
	}

	//if paddle exist
	if(GameManager::HeroPaddle){
		//place the orb at the mid center of the paddle
		
		double orb_x = ((GameManager::HeroPaddle)->_x + ((GameManager::HeroPaddle)->_width / 2.0f)) - Orb::_radius;
		double orb_y = (GameManager::HeroPaddle)->_y - (Orb::_radius / 2.0f);

		result = new Orb(orb_x, orb_y, Ball_SPD);
		GameManager::OrbArray.push_back(result);
	//else
	}else{
		throw "trying to creat orb while paddle not existant";
	}
	return result;
}

//duplicate the status of target, called when the ball splints
void GameManager::SpawnOrb(Orb* target){
	Orb* new_orb = new Orb(target->_x, target->_y, Ball_SPD);
	delete new_orb->_direction;
	//reverse the x_coord
	new_orb->_direction = new Vec_2( -1.0f *((target->_direction)->_x_cord),
									(target->_direction)->_y_cord);
	new_orb->_on_fire = false;

	new_orb->_held = false;
	GameManager::OrbArray.push_back(new_orb);
}

void GameManager::SpawnPaddle(){
	//if none
	if(!GameManager::HeroPaddle){
		GameManager::HeroPaddle = new Paddle(7.0f);
	}else{
		throw "calling spawn paddle twice";
	}

}

void GameManager::RemoveBlock(std::list<BaseBlock*>::iterator& n){
	// if list has no item, don't do anything
	if(GameManager::BlockArray.size() == 0){
		return;
	}

	if((*n)->_required){
		GameManager::Block_Need_Break--;
	}

	//if n is the last item, just free memory remove it
	if(*n == (GameManager::BlockArray).back()){
		delete (GameManager::BlockArray).back();
		(GameManager::BlockArray).back() = NULL;
		GameManager::BlockArray.pop_back();		
	//if n is not the last item, swap it with the last element 
	//and then remove the last element
	}else{
		BaseBlock* temp = (GameManager::BlockArray).back();
		(GameManager::BlockArray).back() = *n;
		*n = temp;
		if((GameManager::BlockArray).back()){
			delete (GameManager::BlockArray).back();
			(GameManager::BlockArray).back() = NULL;
		}
		GameManager::BlockArray.pop_back();
	}
	//prevent out of bound error (last)
	//re-check the n-th item in the list (not last)
	n--;
	
}

//set level to 1.
void GameManager::InitNewGame(XInfo& myInfo){
	GameManager::ExtraBall = 3;
	GameManager::GameScore = 0;
	
	GameManager::BlockArray.clear();
	delete GameManager::HeroPaddle;
	GameManager::HeroPaddle = NULL;
	GameManager::OrbArray.clear();

	GameManager::LevelCount = 1;
	XInfo_ptr = &myInfo;
	GameManager::FPS = GameManager::Target_FPS;
	GameManager::RunningGame = true;
	//for testing purpose
	//GameManager::LevelCount = 5;
}

void GameManager::AddPoints(int amount){
	if(GameManager::GameScore + amount >= 0){
		GameManager::GameScore += amount;

        if(ToNextLife + amount >= 0){
            ToNextLife += amount;
            if(GameManager::ToNextLife >= GameManager::BonusLifeScore){
                GameManager::ToNextLife -= GameManager::BonusLifeScore;
                GameManager::ExtraBall++;
            }
        }else{
            ToNextLife = 0;
        }

        
	}else{
		GameManager::GameScore = 0;
	}
}

void GameManager::StickBallToPaddle(Orb* target){
	
	if(target == NULL){
		throw "trying to stick non-exisatant ball!";
	}

	if(GameManager::HeroPaddle){
		GameManager::HeroPaddle->HoldThisBall(target);	
	}else{
		throw "Trying to stick ball to non-existant paddle!";
	}
	
}

//just update the orbs
void GameManager::DropOrb(){
	GameManager::Trash_Orb = true;
}


void GameManager::UpdateManager(){
	if(!GameManager::Trash_Orb){
		bool on_screen = false;
		//double check OrbArray length in response of double drop, see if there is any ball
		//on screen
		for(std::list<Orb*>::iterator i = GameManager::OrbArray.begin();
				i != OrbArray.end();
				i++){
			double center_x = (*i)->GetX() + (*i)->GetWidth() / 2.0f;
			double center_y = (*i)->GetY() + (*i)->GetHeight() / 2.0f;

			if( (center_x >= 0) &&
				(center_x <= XInfo_ptr->width)){
				if( (center_y >= 0) &&
					(center_y <= XInfo_ptr->height)){
					on_screen = true;
				}

			}
		}
		if(!on_screen){
			GameManager::Trash_Orb = true;
		}

	}
	

	GameManager::TrashOrb();
	if(Block_Need_Break <= 0){
		GameManager::NextLevel();
	}
}

//Does all the heavy lifting
void GameManager::TrashOrb(){
	if(GameManager::Trash_Orb){
		//loop through the entire Orbarray, swap with last and delete any balls that are fallen
		for(std::list<Orb*>::iterator i = GameManager::OrbArray.begin();
			i != OrbArray.end();
			i++){
			//if fallen, swap and free
			if((*i)->_fallen){
				//swap with last and freee memeory
				Orb* temp = (GameManager::OrbArray).back();
				(GameManager::OrbArray).back() = *i;
				*i = temp;
				if((GameManager::OrbArray).back()){
					delete (GameManager::OrbArray).back();
					(GameManager::OrbArray).back() = NULL;
				}
				GameManager::OrbArray.pop_back();
				break;
			}
		}
		// now check if we are out of balls:
		if(GameManager::OrbArray.size() <= 0){
			//reduce life and reset
			if(GameManager::ExtraBall > 0){
				GameManager::ExtraBall--;
				GameManager::StickBallToPaddle(GameManager::SpawnOrb());
			//GAME OVER
			}else{
				//set the running game flag to false
				GameManager::RunningGame = false;
				//release all memory:
				GameManager::ClearResource();
			}
		}
		//set back the trash Orb flag:
		GameManager::Trash_Orb = false;
	}
}

void GameManager::InitTargetFPS(double target){
	GameManager::Target_FPS = target;
	GameManager::FPS = Target_FPS;
}

//if level 3 is beaten, display win screen.
void GameManager::NextLevel(){
	if(GameManager::LevelCount < 5){
		LevelCount++;
		GameManager::SetLevel();
        GameManager::ExtraBall++;
	}else{
		//set the running game flag to false
		GameManager::RunningGame = false;
		//release all memory:
		GameManager::ClearResource();
	}
}

void GameManager::SetLevel(int target){
	if((target > 0) &&
		(target <= 5)){
		GameManager::LevelCount = target;
	}


	GameManager::ClearResource();
	GameManager::LoadLevelData();
	GameManager::SpawnPaddle();
	GameManager::StickBallToPaddle(GameManager::SpawnOrb());
}

//free all memory
void GameManager::ClearResource(){
	//delete all blocks:
	for(std::list<BaseBlock*>::iterator i = GameManager::BlockArray.begin();
		i != BlockArray.end();
		i++){
			delete *i;
			*i = NULL;
	}
	GameManager::BlockArray.clear();
	//delete all orbs:
	for(std::list<Orb*>::iterator i = GameManager::OrbArray.begin();
		i != OrbArray.end();
		i++){
			delete *i;
			*i = NULL;
	}
	GameManager::OrbArray.clear();
	//delete Hero Paddle
	delete HeroPaddle;
	HeroPaddle = NULL;
	
	/*
	std::cout << "block size:" << GameManager::BlockArray.size() << std::endl;
	std::cout << "orb size:" << GameManager::OrbArray.size()<< std::endl;
	std::cout << "paddle add:" << HeroPaddle << std::endl;
	*/

	GameManager::Block_Need_Break = 0;
}

void GameManager::LoadLevelData(){
	if(GameManager::Block_Spacing <= 0){
        throw "Block_Spacing initialization failed!";
    }

    XInfo& myInfo = *XInfo_ptr;

    int next_x = -1;
    int next_y = -1;

    int color_index;
    int num_of_row;
    int block_per_row;

    double top_dis;
    double side_dis;
    double relative_block_space;

    if(GameManager::LevelCount == 1){
        color_index = -1;
        num_of_row = 3;
        block_per_row = 13;

        top_dis = 100.0f;
        side_dis = 50.0f;
        relative_block_space = (myInfo.default_width - side_dis * 2 - BaseBlock::global_width * block_per_row) / (block_per_row -1);
        //Begin with 6 blocks of space on top
        next_y = top_dis;
        //double loop to draw the 9 by 6 matrix
        for(int y= 0; y< num_of_row; y++){
            //leave 6 blocks of spacing on left& right
            next_x = side_dis;
            color_index = 1;
            for(int x= 0; x<block_per_row; x++){
                BaseBlock* temp = NULL;
                //first row red
                if(y < 2){
                    temp = new BaseBlock(1, next_x, next_y);
                //second row orange
                }else if(y < 3){
                    temp = new BaseBlock(2, next_x, next_y);
                }

                GameManager::SpawnBlock(temp);
                next_x += BaseBlock::global_width + relative_block_space;
                /*
                //DEBUG Block position
                cout << "-------" << endl;
                cout << "x: " << next_x << endl;
                cout << "y: " << next_y << endl;
                */
            }
            next_y += (BaseBlock::global_height + GameManager::Block_Spacing);
        }
    }else if(GameManager::LevelCount == 2){
        color_index = -1;
        num_of_row = 5;
        block_per_row = 13;

        top_dis = 100.0f;
        side_dis = 50.0f;
        relative_block_space = (myInfo.default_width - side_dis * 2 - BaseBlock::global_width * block_per_row) / (block_per_row -1);
        //Begin with 6 blocks of space on top
        next_y = top_dis;
        //double loop to draw the 9 by 6 matrix
        for(int y= 0; y< num_of_row; y++){
            //leave 6 blocks of spacing on left& right
            next_x = side_dis;
            color_index = 2;
            for(int x= 0; x<block_per_row; x++){
                BaseBlock* temp = NULL;
                //first row red
                if(y < 2){
                    temp = new BaseBlock(1, next_x, next_y);
                //second row orange
                }else if(y < 3){
                    temp = new BaseBlock(2, next_x, next_y);
                //Third row orange
                }else if(y < 4){
                    temp = new BaseBlock(3, next_x, next_y); 
                //forth row green   
                }else{
                    temp = new BaseBlock(4, next_x, next_y);    
                }
                
                GameManager::SpawnBlock(temp);
                next_x += BaseBlock::global_width + relative_block_space;
                /*
                //DEBUG Block position
                cout << "-------" << endl;
                cout << "x: " << next_x << endl;
                cout << "y: " << next_y << endl;
                */
            }
            next_y += (BaseBlock::global_height + GameManager::Block_Spacing);
        }
    }else if(GameManager::LevelCount == 3){
    	color_index = -1;
        num_of_row = 13;
        block_per_row = 13;

        top_dis = 50.0f;
        side_dis = 50.0f;
        relative_block_space = (myInfo.default_width - side_dis * 2 - BaseBlock::global_width * block_per_row) / (block_per_row -1);
        
        std::cout << "relative_block_space: " << relative_block_space << std::endl;

        //Begin with 6 blocks of space on top
        next_y = top_dis;
        //double loop to draw the y by x matrix
        for(int y= 0; y< num_of_row; y++){
            //leave 6 blocks of spacing on left& right
            next_x = side_dis;
            for(int x= 0; x<block_per_row; x++){
                BaseBlock* temp = NULL;
                
                if((y >= num_of_row - 1) || 
                    (x == 0) ||
                    (x == block_per_row -1)){
                	temp = new IronBlock(next_x, next_y);
                }else if(y <= 1){

                }else if(y < 4){
                	temp = new BaseBlock(5, next_x, next_y);
                }else if(y < 6){
                	temp = new BaseBlock(4, next_x, next_y);
                }else if(y < 8){
                	temp = new BaseBlock(3, next_x, next_y);
                }else if(y < 10){
                	temp = new BaseBlock(2, next_x, next_y);
                }else{
                	//row red
                	temp = new BaseBlock(1, next_x, next_y);
                }
                if(temp != NULL){
                	GameManager::SpawnBlock(temp);	
                }
            	
                
                next_x += BaseBlock::global_width + relative_block_space;
                /*
                //DEBUG Block position
                cout << "-------" << endl;
                cout << "x: " << next_x << endl;
                cout << "y: " << next_y << endl;
                */
            }
            next_y += (BaseBlock::global_height + GameManager::Block_Spacing);
        }
    }else if(GameManager::LevelCount == 4){
        color_index = -1;
        num_of_row = 9;
        block_per_row = 13;

        top_dis = 60.0f;
        side_dis = 50.0f;
        relative_block_space = (myInfo.default_width - side_dis * 2 - BaseBlock::global_width * block_per_row) / (block_per_row -1);
        //Begin with 6 blocks of space on top
        next_y = top_dis;
        //double loop to draw the 9 by 6 matrix
        for(int y= 0; y< num_of_row; y++){
            //leave 6 blocks of spacing on left& right
            next_x = side_dis;
            color_index = 2;
            for(int x= 0; x<block_per_row; x++){
                BaseBlock* temp = NULL;
                //first row red
                if((y == 0) ||
                	(y == num_of_row -1) ||
                	(x == 0) ||
                	(x == block_per_row -1) ){
                	temp = new IronBlock(next_x, next_y);
                }else if(y < 3){
                    temp = new BaseBlock(6, next_x, next_y);
                }else if(y < 6){
                    temp = new BaseBlock(5, next_x, next_y);
                }else if(y < 8){
                    temp = new BaseBlock(4, next_x, next_y);
                }else{
                        
                }
                
                GameManager::SpawnBlock(temp);
                next_x += BaseBlock::global_width + relative_block_space;
                /*
                //DEBUG Block position
                cout << "-------" << endl;
                cout << "x: " << next_x << endl;
                cout << "y: " << next_y << endl;
                */
            }
            next_y += (BaseBlock::global_height + GameManager::Block_Spacing);
        }
    }else if(GameManager::LevelCount == 5){
        color_index = -1;
        num_of_row = 7;
        block_per_row = 13;

        top_dis = 40.0f;
        side_dis = 40.0f;
        relative_block_space = (myInfo.default_width - side_dis * 2 - BaseBlock::global_width * block_per_row) / (block_per_row -1);
        //Begin with 6 blocks of space on top
        next_y = top_dis;
        //double loop to draw the 9 by 6 matrix
        for(int y= 0; y< num_of_row; y++){
            //leave 6 blocks of spacing on left& right
            next_x = side_dis;
            color_index = 2;
            for(int x= 0; x<block_per_row; x++){
                BaseBlock* temp = NULL;
                //first row red
                temp = new IronBlock(next_x, next_y);

                temp->_required = true;
                GameManager::SpawnBlock(temp);
                next_x += BaseBlock::global_width + relative_block_space;
                /*
                //DEBUG Block position
                cout << "-------" << endl;
                cout << "x: " << next_x << endl;
                cout << "y: " << next_y << endl;
                */
            }
            next_y += (BaseBlock::global_height + GameManager::Block_Spacing);
        }
    }	
}