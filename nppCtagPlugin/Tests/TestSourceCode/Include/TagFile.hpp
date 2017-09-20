#ifndef TAGFILE_HPP
#define TAGFILE_HPP

#include <fstream>
#include <string>

namespace CTagsPlugin
{

class TagFile
{
public:
    class Iterator
    {
    public:
        Iterator();
        Iterator(long p_pos, std::ifstream* p_file);
        Iterator(const Iterator& p_iter);

        Iterator& operator=(const Iterator& p_iter);
        long operator-(const Iterator& p_iter) const;
        Iterator operator-(long p_linesAmount) const;
        Iterator operator+(long p_linesAmount) const;
        Iterator& operator--();
        Iterator& operator++();
        void advance(long p_charCount);
        bool operator<(const Iterator& p_iter) const;
        bool operator<=(const Iterator& p_iter) const;
        bool operator!=(const Iterator& p_iter) const;
        bool operator==(const Iterator& p_iter) const;
        std::string operator*() const;
        std::string readLine() const;
        long filePosition() const;

    private:
        void decreaseLine();
        void advanceLine();

        long m_pos;
        std::ifstream* m_file;
    };

    /**
     * @throws Plugin::OpenFileException
     */
    TagFile(const std::string& p_filePath);
    Iterator begin();
    Iterator end();

    bool eof() const;
    std::string getLine();

private:
    std::ifstream m_file;
};

} // namespace CTagsPlugin

#endif // TAGFILE_HPP
