// GP3Labs.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "pch.h"
#include "Application.h"

int main(int argc, char *argv[])
{
	Application::GetInstance() -> Run(); 
	// GetInstance essentially allows only a single instance of Application to run
	
	return 0;
}
