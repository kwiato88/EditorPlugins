#ifndef TAG_HPP
#define TAG_HPP

#include <string>
#include <ostream>
#include <vector>
#include "Fields.hpp"

#include <map>
namespace CTagsPlugin
{

class TagAttributes
{
public:
	enum class Attribute
	{
		Name,
		Path,
		Addr,
		Kind,
		Access,
		Type,
		Signature
	};
	TagAttributes& set(Attribute p_attribute, const std::string& p_value);
	std::string get(Attribute p_attribute) const;
	TagAttributes& operator+=(const TagAttributes& p_other);

private:
	std::map<Attribute, std::string> m_attributes;
};

struct ITagPrinter
{
	virtual ~ITagPrinter() {}
	virtual void print(const TagAttributes& p_attr) = 0;
};

struct Tag
{
    std::string name;
    std::string path;
    std::string addr;
	bool isFileScoped;

	virtual ~Tag();

	bool equals(const Tag& p_tag) const;
	virtual void assign(const Tag& p_tag);
	virtual Tag* clone() const;

	virtual bool isTagWithName(const std::string& p_name) const;
	virtual bool isComplex() const;
	virtual bool isChild(const Tag& p_parrent) const;
	virtual bool isDerived(const Tag& p_base) const;

	virtual void describe(std::ostream& p_out) const;
	std::string toString() const;
	void print(ITagPrinter& p_printer) const;

protected:
	virtual bool isEqual(const Tag& p_tag) const;
	void describeMembers(std::ostream& p_out) const;
	virtual TagAttributes getAttributes() const;
};

} // namespace CTagsPlugin

#endif // TAG_HPP