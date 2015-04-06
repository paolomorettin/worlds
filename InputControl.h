#pragma once
#include<vector>

class InputControl
{
public:
	InputControl(void);
	InputControl(const InputControl& inputControl);
	~InputControl(void);

	void keyboardInput(unsigned char key, int x, int y); //use   void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
	void mousePositionInput(int x, int y);				 //use   void glutMotionFunc(void (*func)(int x, int y));

	void newCicle(); //reset pressed key and update the mouse system
	
	std::vector<unsigned char> getPressedKey();
	void getCurrentMousePosition(int &Xout, int &Yout);
	void getPreviousMousePosition(int &Xout, int &Yout);
	void getDeltaMousePosition(int &Xout, int &Yout);

private:
	std::vector<unsigned char> pressedKey;

	bool *mouseMoved; //size = 2
	int *mouseX, *mouseY; //size = 2
	int mouseIndex;
};

