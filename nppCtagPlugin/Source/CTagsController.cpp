#include <functional>
#include "CTagsController.hpp"
#include "CTagsGenerator.hpp"
#include "LocationSettetException.hpp"
#include "OpenFileException.hpp"
#include "TagsReaderException.hpp"
#include "TagNotFoundException.hpp"
#include "GenerateTagsException.hpp"
#include "UserInputError.hpp"
#include "Log.hpp"

namespace CTagsPlugin
{
namespace
{
std::string getFileDir(std::string p_filePath)
{
    return p_filePath.substr(0, p_filePath.find_last_of("\\"));
}
}  // namespace

CTagsController::CTagsController(
    std::shared_ptr<Plugin::ILocationSetter> p_locationSetter,
	std::shared_ptr<Plugin::ILocationGetter> p_locationGetter,
	std::shared_ptr<Plugin::IMessagePrinter> p_messagePrinter,
	std::shared_ptr<Plugin::IPathGetter> p_pathGetter,
    SelectorFactory p_selectorFactory,
	std::unique_ptr<ITagHierarchySelector> p_hierSelector,
	std::shared_ptr<Plugin::ITextReader> p_textReader,
	std::shared_ptr<Plugin::IPathsSelector> p_dirsSelector,
	std::shared_ptr<Plugin::IPathsSelector> p_filesSelector,
	std::shared_ptr<ITagsReader> p_tagsReader,
    IConfiguration& p_config,
	GetTagSearchMatcher p_tagSearchMatcherFactory)
 : m_locationsNavigator(p_locationSetter, p_locationGetter),
   m_tagsNavigator(p_locationGetter, m_locationsNavigator, p_selectorFactory, std::move(p_hierSelector), p_tagsReader),
   m_messagePrinter(p_messagePrinter),
   m_locationGetter(p_locationGetter),
   m_pathGetter(p_pathGetter),
   m_textReader(p_textReader),
   m_dirsSelector(p_dirsSelector),
   m_filesSelector(p_filesSelector),
   m_config(p_config),
   m_tagSearchMatcherFactory(p_tagSearchMatcherFactory)
{
	m_handlers.addHandler<Command::GenerateTags, Result::Basic>(
		Command::GenerateTags::Id(), [&](const auto& p) { return handleGenerateTags(p); });
}

void CTagsController::next()
{
    try
    {
        m_locationsNavigator.goToNextLocation();
    }
    catch (Plugin::LocationSetterException& e)
    {
		LOG_WARN << "Error during go to next tag: " << typeid(e).name() << ". Details: " << e.what();
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
		LOG_WARN << "Error during go to previous tag: " << typeid(e).name() << ". Details: " << e.what();
        m_messagePrinter->printErrorMessage("Previous Tag", e.what());
    }
}

void CTagsController::find()
{
	try
	{
		m_tagsNavigator.goTo(getCurrentWord());
	}
    catch(Plugin::UserInputError& e)
    {
		LOG_WARN << "Error during find tag: " << typeid(e).name() << ". Details: " << e.what();
        m_messagePrinter->printInfoMessage("Find Tag", e.what());
    }
	catch(TagsReaderException& e)
	{
		LOG_WARN << "Error during find tag: " << typeid(e).name() << ". Details: " << e.what();
        m_messagePrinter->printInfoMessage("Find Tag", e.what());
	}
	catch(Plugin::OpenFileException& e)
	{
		LOG_WARN << "Error during find tag: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printErrorMessage("Find Tag", e.what());
	}
	catch(TagNotFoundException& e)
	{
		LOG_WARN << "Error during find tag: " << typeid(e).name() << ". Details: " << e.what();
	    m_messagePrinter->printInfoMessage("Find Tag", e.what());
	}
	catch(Plugin::LocationSetterException& e)
	{
		LOG_WARN << "Error during find tag: " << typeid(e).name() << ". Details: " << e.what();
	    m_messagePrinter->printErrorMessage("Find Tag", e.what());
	}
}

void CTagsController::tagInfo()
{
	try
	{
		showTagInfo(getCurrentWord());
	}
	catch (Plugin::UserInputError& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Tag Info", e.what());
	}
}

void CTagsController::showTagInfo(const std::string& p_tagName)
{
    try
    {
        m_tagsNavigator.goToChildTag(p_tagName);
    }
    catch(TagsReaderException& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Tag Info", e.what());
	}
	catch(Plugin::OpenFileException& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printErrorMessage("Tag Info", e.what());
	}
	catch(TagNotFoundException& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
	    m_messagePrinter->printInfoMessage("Tag Info", e.what());
	}
	catch(Plugin::LocationSetterException& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
	    m_messagePrinter->printErrorMessage("Tag Info", e.what());
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printErrorMessage("ERROR", std::string("unknown exception") + e.what());
	}
}

std::string CTagsController::getCurrentWord() const
{
    std::string l_tagName = m_textReader->getCurrentWord();
    if(l_tagName.empty())
        throw Plugin::UserInputError("No tag name to find specified");
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
		throw Plugin::UserInputError("Source folders not specified");
    return l_projectDirs;
}

std::string CTagsController::getTargetFile() const
{
    std::string l_tagsFilePath = m_pathGetter->getFilePath("Output file");
    if(l_tagsFilePath.empty())
        throw Plugin::UserInputError("Target tags file not specified");
    return l_tagsFilePath;
}

void CTagsController::generateTags()
{
	try
	{
		gnerateTags(getTargetFile(), getSourceDirs());
	}
	catch (Plugin::UserInputError& e)
	{
		m_messagePrinter->printInfoMessage("Gen Tags", e.what());
	}
	catch (GenerateTagsException& e)
	{
		LOG_WARN << "Error during tags generation: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Gen Tags", e.what());
	}
}

void CTagsController::gnerateTags(std::string p_outFile, std::vector<std::string> p_sourceDirs)
{
	LOG_INFO << "Generate tags to file: " << p_outFile;
    CTagsGenerator(m_config.getCtagsPath(), m_config.getSupportedExtensionFileds()).generate(p_outFile, p_sourceDirs);
	m_messagePrinter->printInfoMessage("Gen Tags", "Tags saved to file: " + p_outFile);
}

Result::Basic CTagsController::handleGenerateTags(const Command::GenerateTags& p_com)
{
	try
	{
		gnerateTags(p_com.tagFilePath, p_com.sourceDirsPaths);
		return Result::Basic{ Result::Result::Success };
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during tags generation: " << typeid(e).name() << ". Details: " << e.what();
		return Result::Basic{ Result::Result::Failure };
	}
}

void CTagsController::tagsSearch()
{
	try
	{
		m_tagsNavigator.goToTag(m_tagSearchMatcherFactory());
	}
	catch(Plugin::UserInputError& e)
	{
		LOG_WARN << "Error during tag search: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Tag search", e.what());
	}
	catch (TagsReaderException& e)
	{
		LOG_WARN << "Error during tag search: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Tag search", e.what());
	}
	catch (Plugin::OpenFileException& e)
	{
		LOG_WARN << "Error during tag search: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printErrorMessage("Tag search", e.what());
	}
	catch (TagNotFoundException& e)
	{
		LOG_WARN << "Error during tag search: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Tag search", e.what());
	}
	catch (Plugin::LocationSetterException& e)
	{
		LOG_WARN << "Error during tag search: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printErrorMessage("Tag search", e.what());
	}
}

void CTagsController::tagHierarchy()
{
	try
	{
		m_tagsNavigator.goToTagInHierarchy(getCurrentWord());
	}
	catch (Plugin::UserInputError& e)
	{
		LOG_WARN << "Error during tag hierarchy: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Tag hierarchy", e.what());
	}
	catch (TagsReaderException& e)
	{
		LOG_WARN << "Error during tag hierarchy: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Tag hierarchy", e.what());
	}
	catch (Plugin::OpenFileException& e)
	{
		LOG_WARN << "Error during tag hierarchy: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printErrorMessage("Tag hierarchy", e.what());
	}
	catch (TagNotFoundException& e)
	{
		LOG_WARN << "Error during tag hierarchy: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printInfoMessage("Tag hierarchy", e.what());
	}
	catch (Plugin::LocationSetterException& e)
	{
		LOG_WARN << "Error during tag hierarchy: " << typeid(e).name() << ". Details: " << e.what();
		m_messagePrinter->printErrorMessage("Tag hierarchy", e.what());
	}
}

void CTagsController::handleTransaction(long p_id, Messaging::Transaction& p_trans)
{
	m_handlers.handle(p_id, p_trans);
}

} /* namespace CTagsPlugin */
