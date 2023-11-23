#include <iostream>
#include <string>

int indentifyCommand(const std::string& command)
{
	std::string temp;
	temp = command;
	temp = strtok(temp, " ");
	return temp;
}

int main()
{
	std::string command = "SELECT A FROM B";
	std::string rezultat = indentifyCommand(command);
	std::cout << rezultat;
	//return 0;
}
