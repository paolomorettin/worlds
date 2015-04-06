#include "InputControl.h"

InputControl::InputControl(void)
{
	mouseMoved = new bool[2];
	mouseX = new int[2];
	mouseY = new int[2];

	for(int i = 0; i < 2; i++)
	{
		mouseX[i] = 0;
		mouseY[i] = 0;
		mouseMoved[i] = false;
	}
	mouseIndex = 0;
}
InputControl::InputControl(const InputControl& inputControl)
{
	mouseMoved = new bool[2];
	mouseX = new int[2];
	mouseY = new int[2];

	pressedKey = inputControl.pressedKey;

	for(int i = 0; i < 2; i++)
	{
		mouseMoved[i] = inputControl.mouseMoved[i];
		mouseX[i] = inputControl.mouseX[i];
		mouseY[i] = inputControl.mouseY[i];
	}
	mouseIndex = inputControl.mouseIndex;
}
InputControl::~InputControl(void)
{
	delete[](mouseY);
	delete[](mouseX);
	delete[](mouseMoved);
}

void InputControl::keyboardInput(unsigned char key, int x, int y)
{
	pressedKey.push_back(key);
}
void InputControl::mousePositionInput(int x, int y)
{
	mouseX[mouseIndex] = x;
	mouseY[mouseIndex] = y;
	mouseMoved[mouseIndex] = true;
}

void InputControl::newCicle()
{
	pressedKey.clear();

	mouseIndex = !mouseIndex; //switch 0 and 1
	mouseMoved[mouseIndex] = false;
	mouseX[mouseIndex] = mouseX[!mouseIndex]; //clone the previous value
	mouseY[mouseIndex] = mouseY[!mouseIndex];
}

std::vector<unsigned char> InputControl::getPressedKey()
{
	return pressedKey;
}
void InputControl::getCurrentMousePosition(int &Xout, int &Yout)
{
	Xout = mouseX[mouseIndex];
	Yout = mouseY[mouseIndex];
}
void InputControl::getPreviousMousePosition(int &Xout, int &Yout)
{
	Xout = mouseX[!mouseIndex];
	Yout = mouseY[!mouseIndex];
}
void InputControl::getDeltaMousePosition(int &Xout, int &Yout)
{
	if(!mouseMoved[!mouseIndex])
	{
		Xout = 0;
		Yout = 0;
	}
	else
	{
		Xout = mouseX[mouseIndex] - mouseX[!mouseIndex];
		Yout = mouseY[mouseIndex] - mouseY[!mouseIndex];
	}
}