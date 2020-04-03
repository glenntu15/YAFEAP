#pragma once
class ErrorManager
{
public:

	ErrorManager();
	~ErrorManager();

	//
	// Make this a singleton
	//
	static ErrorManager& getInstance()
	{
		static ErrorManager instance;
		return instance;
	}

	void inp_DupError(char* cline);
	
private: 
	int errcount;
};

