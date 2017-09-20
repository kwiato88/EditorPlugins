#include <stdexcept>

#include "TagFile.hpp"
#include "OpenFileException.hpp"

namespace CTagsPlugin
{
namespace
{

long nextLineBegin(long pos, std::ifstream& file)
{
	file.seekg(pos);
    char currentChar = file.peek();
    while(currentChar != '\n')
    {
        file.seekg(++pos);
        currentChar = file.peek();
    }
    // on windows current char is in fact CR(\r, but read as \n) so skip also LF(\n)
    return pos +2; // +1 unix
}

bool isPositionNewLine(long pos, std::ifstream& file)
{
    file.seekg(pos);
    return file.peek() == '\n';
}

long findBeginLinePos(long pos, std::ifstream& file)
{
    if(pos==0) return 0;
    file.seekg(pos);
    char currentChar = file.peek();
    while(currentChar != '\n')
    {
        file.seekg(--pos);
        currentChar = file.peek();
    }
    // windows end of line CR+LF is read as double \n
    // position is \r so skip also \n
    if(isPositionNewLine(pos+1, file)) // not needed in unix
        return pos+2;
    return pos +1;
}

long previousLineBegin(long pos, std::ifstream& file)
{
    // on windows position -3 is last char in previous line because of CR+LF end of line (2 bytes)
    return findBeginLinePos(pos-3, file); // -2 unix
}

} // namespace

TagFile::Iterator::Iterator()
 : m_pos(0), m_file(0)
{
}

TagFile::Iterator::Iterator(long p_pos, std::ifstream* p_file)
 : m_pos(findBeginLinePos(p_pos, *p_file)), m_file(p_file)
{
}

TagFile::Iterator::Iterator(const Iterator& p_iter)
 : m_pos(p_iter.m_pos), m_file	(p_iter.m_file)
{
}

TagFile::Iterator& TagFile::Iterator::operator=(const Iterator& p_iter)
{
    m_pos = p_iter.m_pos;
    m_file = p_iter.m_file;
    return *this;
}

long TagFile::Iterator::filePosition() const
{
    return m_pos;
}
long TagFile::Iterator::operator-(const Iterator& p_iter) const
{
    return m_pos - p_iter.m_pos;
}

void TagFile::Iterator::decreaseLine()
{
    m_pos = previousLineBegin(m_pos, *m_file);
}

TagFile::Iterator TagFile::Iterator::operator-(long p_amount) const
{
	Iterator iter = *this;
    if(m_pos == 0) // TODO: TagFileReader: consider moving it to decreaseLine
    {
        m_file->seekg(0,std::ios::end);
        iter = Iterator(m_file->tellg(), m_file);
		--p_amount;
    }
    for(int i=0; i<p_amount; ++i)
        iter.decreaseLine();
    return iter;
}


TagFile::Iterator& TagFile::Iterator::operator--()
{
    if(m_pos == 0)
    {  
        m_file->seekg(0,std::ios::end);
        m_pos = findBeginLinePos(m_file->tellg(), *m_file);
        return *this;
    }
    decreaseLine();
    return *this;
}

void TagFile::Iterator::advance(long p_charCount)
{
    m_pos = findBeginLinePos(m_pos + p_charCount, *m_file);
}

void TagFile::Iterator::advanceLine()
{
	//TODO: TagFileReader: protection against advance over end of file. It breaks MT. Why???
	// does it prevent begin interator from being advanced?
	/* 
	if (m_pos == 0)
		return;
	m_file->seekg(0, std::ios::end);
	auto lastLineBeginPos = findBeginLinePos(m_file->tellg(), *m_file);
	if (m_pos == lastLineBeginPos)
		m_pos = 0;
	*/
	m_pos = nextLineBegin(m_pos, *m_file);
}

TagFile::Iterator TagFile::Iterator::operator+(long p_amount) const
{
    Iterator iter = *this;
    for(int i=0; i<p_amount; ++i)
        iter.advanceLine();
    return iter;
}

TagFile::Iterator& TagFile::Iterator::operator++()
{
    advanceLine();
    return *this;
}

std::string TagFile::Iterator::readLine() const
{
    std::string line;
    m_file->seekg(m_pos);
    std::getline(*m_file, line);
    return line;
}

std::string TagFile::Iterator::operator*() const
{
    return readLine();
}

char TagFile::Iterator::peek() const
{
	return m_file->peek();
}

bool TagFile::Iterator::operator<(const Iterator& p_iter) const
{
    return m_pos < p_iter.m_pos;
}

bool TagFile::Iterator::operator<=(const Iterator& p_iter) const
{
    return m_pos <= p_iter.m_pos;
}

bool TagFile::Iterator::operator!=(const Iterator& p_iter) const
{
    return m_pos != p_iter.filePosition();
}

bool TagFile::Iterator::operator==(const Iterator& p_iter) const
{
    return !(this->operator!=(p_iter));
}

TagFile::TagFile(const std::string& p_filePath)
{
    m_file.open(p_filePath.c_str());
    if(!m_file.is_open())
        throw Plugin::OpenFileException("Can't open tag file " + p_filePath);
}

TagFile::Iterator TagFile::begin()
{
    m_file.seekg(0, std::ios::beg);
    return Iterator(m_file.tellg(), &m_file);
}

TagFile::Iterator TagFile::end()
{
    return Iterator(0, &m_file);
}

bool TagFile::eof() const
{
    return m_file.eof();
}

std::string TagFile::getLine()
{
    std::string line;
    std::getline(m_file, line);
    return line;
}

char TagFile::peek()
{
	return m_file.peek();
}

} // namespace CTagsPlugin
