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
	int getErrorCount() { return errcount; }
	void inp_DupError(char* cline);
	void ItemNotFound(const char* item, int id);
	void inp_DupForceError(char* cline);
	void inp_Unrecognized(char* cline);
	void inp_FormatError(char* cline);
	
private: 
	int errcount;
	int warncount;
};

