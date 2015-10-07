#ifndef __INPUT_MANAGER__
#define	__INPUT_MANAGER__

#include <string>

class InputManager{
public:
	InputManager();
	~InputManager();

	//methods
	static void InitInput();
	//on keydown, everything else is upped.
	static void KeyDown(std::string& target);
	static void KeyUp(std::string& target);
	//return false if target is an unspecified string
	static bool LookUp(std::string& target);
	//fields
	static bool _left_signal;
	static bool _right_signal;
	static bool _space_signal;
	
	static std::string LEFT_KEY;
	static std::string RIGHT_KEY;
	static std::string SPACE_KEY;
};

#endif