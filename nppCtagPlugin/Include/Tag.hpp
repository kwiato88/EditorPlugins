#ifndef TAG_HPP
#define TAG_HPP

#include <string>
#include <ostream>
#include <vector>
#include <boost/optional/optional.hpp>
#include "Fields.hpp"
#include "FillBaseTag.hpp"
#include "TagHolder.hpp"
#include "ObjectsCounter.hpp"

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

class ExtensionFields;
class ITagsReader;
class TestTagBuilder;

class Tag : public Meas::ObjectsCounter<Tag>
{
public:
	friend void fillBaseTag(Tag& p_outTag, const std::string& p_TagFileLine, const ExtensionFields& p_fields);
	friend class TestTagBuilder;

	class Addr
	{
	public:
		Addr(const std::string& p_path, const std::string& p_addr);
		std::string filePath() const;
		int lineNoInFile() const;
		int cloNoInFile() const;
	private:
		bool isLineNumber() const;
		void updatePosition() const;
		std::pair<int, int> findPosition() const;

		std::string path;
		std::string addr;
		mutable boost::optional<std::pair<int, int>> positionInFile;
	};

	Addr getAddr() const;
	std::string getName() const;

	virtual ~Tag();


	bool isLocalIn(const std::string& p_filePath) const;

	bool equals(const Tag& p_tag) const;
	virtual void assign(const Tag& p_tag);
	virtual Tag* clone() const;

	virtual bool isTagWithName(const std::string& p_name) const;
	virtual bool isComplex() const;
	virtual bool isChild(const Tag& p_parrent) const;
	virtual bool isDerived(const Tag& p_base) const;
	virtual std::vector<TagHolder> baseTags(const ITagsReader&) const;

	virtual void describe(std::ostream& p_out) const;
	std::string toString() const;
	void print(ITagPrinter& p_printer) const;

protected:
	std::string name;
	std::string addr;
	std::string path;
	bool isFileScoped;

	virtual bool isEqual(const Tag& p_tag) const;
	void describeMembers(std::ostream& p_out) const;
	virtual TagAttributes getAttributes() const;
};

} // namespace CTagsPlugin

#endif // TAG_HPP
