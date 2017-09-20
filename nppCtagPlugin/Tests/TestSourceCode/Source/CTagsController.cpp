#include <future>
#include <functional>
#include "CTagsController.hpp"
#include "CTagsGenerator.hpp"
#include "LocationSettetException.hpp"
#include "OpenFileException.hpp"
#include "TagsReaderException.hpp"
#include "TagNotFoundException.hpp"
#include "GenerateTagsException.hpp"

namespace CTagsPlugin
{
namespace
{
std::string getFileDir(std::string p_filePath)
{
    return p_filePath.substr(0, p_filePath.find_last_of("\\"));
}

struct UserInputError: public std::runtime_error
{
    UserInputError(const std::string& p_message)
        : std::runtime_error(p_message)
    {}
};

}  // namespace

AsyncCall::AsyncCall(const std::function<void(void)>& p_call)
    : m_call(p_call){}

bool AsyncCall::operator()()
{
    if(m_lock.try_lock())
    {
        std::async([&](){m_call(); m_lock.unlock();});
        return true;
    }
    return false;
}

CTagsController::CTagsController(
        boost::shared_ptr<Plugin::ILocationSetter> p_locationSetter,
		boost::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
        boost::shared_ptr<Plugin::IMessagePrinter> p_messagePrinter,
        boost::shared_ptr<Plugin::IPathGetter> p_pathGetter,
        SelectorFactory p_selectorFactory,
        boost::shared_ptr<Plugin::ITextReader> p_textReader,
        boost::shared_ptr<Plugin::IPathsSelector> p_dirsSelector,
        boost::shared_ptr<Plugin::IPathsSelector> p_filesSelector,
        boost::shared_ptr<ITagsReader> p_tagsReader,
        IConfiguration& p_config)
 : m_locationsNavigator(p_locationSetter, p_locationGetter),
   m_tagsNavigator(p_locationGetter, m_locationsNavigator, p_selectorFactory, p_tagsReader),
   m_messagePrinter(p_messagePrinter),
   m_locationGetter(p_locationGetter),
   m_pathGetter(p_pathGetter),
   m_textReader(p_textReader),
   m_dirsSelector(p_dirsSelector),
   m_filesSelector(p_filesSelector),
   m_config(p_config),
   m_genTags(std::bind(&CTagsController::gnerateTags, this)),
   m_tagInfo(std::bind(&CTagsController::showTagInfo, this))
{
}

void CTagsController::next()
{
    try
    {
        m_locationsNavigator.goToNextLocation();
    }
    catch (Plugin::LocationSetterException& e)
    {
        m_messagePrinter->printErrorMessage("Next Tag", e.what());
    }
}

void CTagsController::previous()
{
    try
    {
        m_locationsNavigator.goToPreviousLocaton();
    }
    catch (Plugin::LocationSetterException& e)
    {
        m_messagePrinter->printErrorMessage("Previous Tag", e.what());
    }
}

void CTagsController::find()
{
	try
	{
		m_tagsNavigator.goTo(getCurrentWord());
	}
    catch(UserInputError& e)
    {
        m_messagePrinter->printInfoMessage("Find Tag", e.what());
    }
	catch(TagsReaderException& e)
	{
        m_messagePrinter->printInfoMessage("Find Tag", e.what());
	}
	catch(Plugin::OpenFileException& e)
	{
		m_messagePrinter->printErrorMessage("Find Tag", e.what());
	}
	catch(TagNotFoundException& e)
	{
	    m_messagePrinter->printInfoMessage("Find Tag", e.what());
	}
	catch(Plugin::LocationSetterException& e)
	{
	    m_messagePrinter->printErrorMessage("Find Tag", e.what());
	}
}

void CTagsController::tagInfo()
{
    if(!m_tagInfo())
        m_messagePrinter->printInfoMessage("Tag Info", "Call skipped. Already executing");
}

void CTagsController::showTagInfo()
{
    try
    {
        m_tagsNavigator.goToChildTag(getCurrentWord());
    }
    catch(UserInputError& e)
    {
        m_messagePrinter->printInfoMessage("Tag Info", e.what());
    }
    catch(TagsReaderException& e)
	{
		m_messagePrinter->printInfoMessage("Tag Info", e.what());
	}
	catch(Plugin::OpenFileException& e)
	{
		m_messagePrinter->printErrorMessage("Tag Info", e.what());
	}
	catch(TagNotFoundException& e)
	{
	    m_messagePrinter->printInfoMessage("Tag Info", e.what());
	}
	catch(Plugin::LocationSetterException& e)
	{
	    m_messagePrinter->printErrorMessage("Tag Info", e.what());
	}
}

std::string CTagsController::getCurrentWord() const
{
    std::string l_tagName = m_textReader->getCurrentWord();
    if(l_tagName.empty())
        throw UserInputError("No tag to find");
    return l_tagName;
}

void CTagsController::setTagFiles()
{
    std::vector<std::string> l_tagFiles = 
        m_filesSelector->select(m_config.getTagsFilesPaths(), m_config.getTagsFilesPaths().front());
    if(!l_tagFiles.empty())
        m_config.setTagsFilesPaths(l_tagFiles);
    else
        m_messagePrinter->printInfoMessage("Set Tags","Tag files set can't be empty");
}

void CTagsController::clear()
{
    m_locationsNavigator.clear();
}

std::vector<std::string> CTagsController::getSourceDirs() const
{
    std::vector<std::string> l_projectDirs = m_dirsSelector->select(std::vector<std::string>(),
                                                                    getFileDir(m_locationGetter->getFile()));
	if(l_projectDirs.empty())
		throw UserInputError("Folders not specified");
    return l_projectDirs;
}

std::string CTagsController::getTargetFile() const
{
    std::string l_tagsFilePath = m_pathGetter->getFilePath("Output file");
    if(l_tagsFilePath.empty())
        throw UserInputError("Tags file not specified");
    return l_tagsFilePath;
}

void CTagsController::generateTagFile()
{
    if(!m_genTags())
        m_messagePrinter->printInfoMessage("Gen Tags", "Call skipped. Already executing");
}

void CTagsController::gnerateTags()
{
    try
    {
        const auto tagFile = getTargetFile();
        CTagsGenerator(m_config.getCtagsPath(), m_config.getSupportedExtensionFileds()).generate(tagFile, getSourceDirs());
        m_messagePrinter->printInfoMessage("Gen Tags", "Tags saved to file: " + tagFile);
    }
    catch(UserInputError& e)
    {
        m_messagePrinter->printInfoMessage("Gen Tags", e.what());
    }
    catch (GenerateTagsException& e)
    {
        m_messagePrinter->printInfoMessage("Gen Tags", e.what());
    }
}

} /* namespace CTagsPlugin */
