/* 

Made By AwesomeMc101 for Lunar Guard Technologies
5.22.2022

*/

#include <string>
#include <vector>
#include <functional>

class glostaItem
{
public:
	std::vector<std::string> gfunctions;
	//std::vector<void*> gfunctionCall;
};

void pushGFunctions(glostaItem* gs)
{
	gs->gfunctions.push_back("print");
	gs->gfunctions.push_back("strlen");
	gs->gfunctions.push_back("add");
	gs->gfunctions.push_back("sub");
}

void setTypeClass(glostaItem* gs)
{
	for (int i = 0; i < gs->gfunctions.size(); i++)
	{
		std::string old = gs->gfunctions[i];
		std::string newt = "glosta::" + old;
		gs->gfunctions[i] == newt;
	}
}

bool isGFunction(glostaItem* gs, std::string func)
{
	if (func[0] == 'g' && func[1] == 'l' && func[2] == 'o'
		&& func[3] == 's' && func[4] == 't' && func[5] == 'a'
		&& func[6] == ':' && func[7] == ':'
		)
	{
		return true;
	}
	return false;
}

bool isVarName(std::vector<std::string> keys, std::string var)
{
	var.erase(std::remove(var.begin(), var.end(), ')'), var.end());
	for (std::string key : keys)
	{
		if (key == var)
		{
			return true;
		}
	}
	return false;
}
