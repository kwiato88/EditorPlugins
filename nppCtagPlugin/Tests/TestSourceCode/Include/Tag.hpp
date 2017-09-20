#ifndef TAG_HPP
#define TAG_HPP

#include <string>

namespace CTagsPlugin
{

struct Tag
{
    std::string name;
    std::string path;
    std::string addr;
	bool isFileScoped;

    virtual ~Tag(){};

    virtual Tag* clone() const
    {
        return new Tag(*this);
    }
    void assign(const Tag& p_tag)
    {
        name = p_tag.name;
        path = p_tag.path;
        addr = p_tag.addr;
		isFileScoped = p_tag.isFileScoped;
    }
    virtual size_t size() const
    {
        return ((name.size()+path.size()+addr.size()) * sizeof(std::string::value_type));
    }
};

} // namespace CTagsPlugin

#endif // TAG_HPP
