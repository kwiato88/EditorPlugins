#pragma once

#include <string>

namespace Plugin
{

class Editor
{
public:
    virtual ~Editor() {}

    virtual std::string getFile() const = 0;
    virtual int getLine() const = 0;
    virtual int getColumn() const = 0;

    virtual void setFile(const std::string& p_filePath) = 0;
	virtual void setLine(int p_line) = 0;
    virtual void setColumn(int p_column) = 0;
   
    virtual std::string getCurrentWord() const = 0;
};

}
