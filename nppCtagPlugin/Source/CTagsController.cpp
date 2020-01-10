#include <functional>
#include <fstream>
#include "CTagsController.hpp"
#include "CTagsGenerator.hpp"
#include "OpenFileException.hpp"
#include "ComplexTagWithMatchingName.hpp"
#include "ShellCommandException.hpp"
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
std::string swapExtension(const std::string& p_path, const std::string& p_newExt)
{
	return p_path.substr(0, p_path.rfind(".")) + "." + p_newExt;
}
}  // namespace

CTagsController::CTagsController(
	Plugin::Editor& p_editor,
	Plugin::UI& p_ui,
	Plugin::UIFileSystem& p_files,
	std::unique_ptr<ITagsSelector> p_tagsSelector,
	std::unique_ptr<ITagHierarchySelector> p_hierSelector,
	std::shared_ptr<ITagsReader> p_tagsReader,
    IConfiguration& p_config,
	GetTagSearchMatcher p_tagSearchMatcherFactory,
	Plugin::CommandFactory p_cmdFactory)
 : m_editor(p_editor),
   m_ui(p_ui),
   m_files(p_files),
   m_config(p_config),
   m_tagSearchMatcherFactory(p_tagSearchMatcherFactory),
   m_locationsNavigator(m_editor),
   m_tagsNavigator(m_locationsNavigator, m_editor,
	   std::move(p_tagsSelector), std::move(p_hierSelector),
	   p_tagsReader, p_config),
   m_cmdFactory(p_cmdFactory)
{
	m_handlers.addHandler<Command::GenerateTags, Result::Basic>(
		Command::GenerateTags::Id(), [&](const auto& p) { return handleGenerateTags(p); });
    m_handlers.addHandler<Command::SetTagFiles, Result::Basic>(
		Command::SetTagFiles::Id(), [&](const auto& p) { return handleSetTagFiles(p); });
    m_handlers.addHandler<Command::GetTagFiles, Result::TagFiles>(
		Command::GetTagFiles::Id(), [&](const auto& p) { return handleGetTagFiles(p); });
	m_handlers.addHandler<Command::Test, Result::Test>(
		Command::Test::Id(), [&](const auto& p) { return handleTestCommand(p); });
}

Result::Test CTagsController::handleTestCommand(const Command::Test& p_cmd)
{
	LOG_INFO << "Received test command with value " << p_cmd.value << ". Sending result with the same value.";
	return Result::Test{ p_cmd.value };
}

void CTagsController::next()
{
    try
    {
        m_locationsNavigator.goToNextLocation();
    }
    catch (Plugin::OpenFileException& e)
    {
		LOG_WARN << "Error during go to next tag: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Next Tag", e.what());
    }
}

void CTagsController::previous()
{
    try
    {
        m_locationsNavigator.goToPreviousLocaton();
    }
    catch (Plugin::OpenFileException& e)
    {
		LOG_WARN << "Error during go to previous tag: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Previous Tag", e.what());
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
        m_ui.infoMessage("Find Tag", e.what());
    }
	catch(TagsReaderException& e)
	{
		LOG_WARN << "Error during find tag: " << typeid(e).name() << ". Details: " << e.what();
        m_ui.infoMessage("Find Tag", e.what());
	}
	catch(TagNotFoundException& e)
	{
		LOG_WARN << "Error during find tag: " << typeid(e).name() << ". Details: " << e.what();
	    m_ui.infoMessage("Find Tag", e.what());
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during find tag: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Find Tag", e.what());
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
		m_ui.infoMessage("Tag Info", e.what());
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
		m_ui.infoMessage("Tag Info", e.what());
	}
	catch(TagNotFoundException& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
	    m_ui.infoMessage("Tag Info", e.what());
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Tag Info", e.what());
	}
}

std::string CTagsController::getCurrentWord() const
{
    std::string l_tagName = m_editor.getCurrentWord();
    if(l_tagName.empty())
        throw Plugin::UserInputError("No tag name to find specified");
    return l_tagName;
}

void CTagsController::setTagFiles()
{
	std::vector<std::string> l_tagFiles =
		m_files.selectFilesPaths(m_config.getTagsFilesPaths(), m_config.getTagsFilesPaths().front());
    if(!l_tagFiles.empty())
        m_config.setTagsFilesPaths(l_tagFiles);
    else
        m_ui.infoMessage("Set Tags","Tag files set can't be empty");
}

Result::Basic CTagsController::handleSetTagFiles(const Command::SetTagFiles& p_cmd)
{
    if(!p_cmd.filesPaths.empty())
    {
        m_config.setTagsFilesPaths(p_cmd.filesPaths);
        return Result::Basic{ Result::Result::Success };
    }
    else
    {
		LOG_WARN << "Tags files paths not set. Need to have at least one tag file.";
        return Result::Basic{ Result::Result::Failure };
    }
}

Result::TagFiles CTagsController::handleGetTagFiles(const Command::GetTagFiles&)
{
    return Result::TagFiles{ m_config.getTagsFilesPaths() };
}

void CTagsController::clear()
{
    m_locationsNavigator.clear();
}

std::vector<std::string> CTagsController::getSourceDirs() const
{
    std::vector<std::string> l_projectDirs = m_files.selectDirsPaths(std::vector<std::string>(),
                                                                     getFileDir(m_editor.getFile()));
	if(l_projectDirs.empty())
		throw Plugin::UserInputError("Source folders not specified");
    return l_projectDirs;
}

std::string CTagsController::getTargetFile() const
{
    std::string l_tagsFilePath = m_files.getFilePath("Output file");
    if(l_tagsFilePath.empty())
        throw Plugin::UserInputError("Target tags file not specified");
    return l_tagsFilePath;
}

void CTagsController::generateTags()
{
	try
	{
		auto tagFile = getTargetFile();
		gnerateTags(tagFile, getSourceDirs());
		m_ui.infoMessage("Gen Tags", "Tags saved to file: " + tagFile);
	}
	catch (Plugin::UserInputError& e)
	{
		LOG_WARN << "Error during tags generation: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.infoMessage("Gen Tags", e.what());
	}
	catch (GenerateTagsException& e)
	{
		LOG_WARN << "Error during tags generation: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.infoMessage("Gen Tags", e.what());
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during tag info: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Gen Tags", e.what());
	}
}

void CTagsController::gnerateTags(std::string p_outFile, std::vector<std::string> p_sourceDirs)
{
	LOG_INFO << "Generate tags to file: " << p_outFile;
    CTagsGenerator(m_cmdFactory, m_config.getCtagsPath(), m_config.getSupportedExtensionFileds()).generate(p_outFile, p_sourceDirs);
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
		m_ui.infoMessage("Tag search", e.what());
	}
	catch (TagsReaderException& e)
	{
		LOG_WARN << "Error during tag search: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.infoMessage("Tag search", e.what());
	}
	catch (TagNotFoundException& e)
	{
		LOG_WARN << "Error during tag search: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.infoMessage("Tag search", e.what());
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during tag search: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Tag search", e.what());
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
		m_ui.infoMessage("Tag hierarchy", e.what());
	}
	catch (TagsReaderException& e)
	{
		LOG_WARN << "Error during tag hierarchy: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.infoMessage("Tag hierarchy", e.what());
	}
	catch (TagNotFoundException& e)
	{
		LOG_WARN << "Error during tag hierarchy: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.infoMessage("Tag hierarchy", e.what());
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during tag hierarchy: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Tag hierarchy", e.what());
	}
}

void CTagsController::handleTransaction(long p_id, Messaging::Transaction& p_trans)
{
	m_handlers.handle(p_id, p_trans);
}

void CTagsController::onTagsLoaded()
{
	m_tagsNavigator.onTagsLoaded();
}

std::string CTagsController::getPlantUmlScriptPath() const
{
	auto path = m_files.getFilePath("Select plant UML file");
	if (path.empty())
		throw Plugin::UserInputError("Plant UML file not specified");
	return path;
}

std::string CTagsController::getTagNamePattern() const
{
	auto pattern = m_ui.query("Tag name pattern to include(regex)", "pattern", "");
	if (pattern.empty())
		throw Plugin::UserInputError("Pattern not specified");
	return pattern;
}

void CTagsController::tagClassDiagram()
{
	try
	{
		//TODO: consider removing info messages
		auto plantUmlFilePath(getPlantUmlScriptPath());
		std::ofstream file(plantUmlFilePath);
		m_tagsNavigator.exportClassDiagram(file, getCurrentWord());
		file.close();
		m_ui.infoMessage("Class diagram", "Diagram script saved to file: " + plantUmlFilePath);
		m_cmdFactory(
			"java",
			std::string("-jar ") + m_config.getPlantUmlPath() + " " + plantUmlFilePath
		)->execute();
		m_ui.infoMessage("Class diagram", "Diagram PNG saved");
		m_cmdFactory(swapExtension(plantUmlFilePath, "png"), "")->execute();
	}
	catch (Plugin::UserInputError& e)
	{
		LOG_WARN << "Error during class diagram generation: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.infoMessage("Class diagram", e.what());
	}
	catch (Plugin::ShellCommandException& e)
	{
		LOG_WARN << "Error during class diagram generation: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Class diagram", std::string("Failed to execute plant UML. Details: ") + e.what());
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during class diagram generation: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Class diagram", e.what());
	}
}

void CTagsController::classDiagram()
{
	try
	{
		auto plantUmlFilePath(getPlantUmlScriptPath());
		std::ofstream file(plantUmlFilePath);
		m_tagsNavigator.exportClassDiagram(file, ComplexTagWithMatchingName(getTagNamePattern()));
		file.close();
		m_ui.infoMessage("Class diagram", "Diagram script saved to file: " + plantUmlFilePath);
		m_cmdFactory(
			"java",
			std::string("-jar ") + m_config.getPlantUmlPath() + " " + plantUmlFilePath
		)->execute();
		m_ui.infoMessage("Class diagram", "Diagram PNG saved");
	}
	catch (Plugin::UserInputError& e)
	{
		LOG_WARN << "Error during class diagram generation: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.infoMessage("Class diagram", e.what());
	}
	catch (Plugin::ShellCommandException& e)
	{
		LOG_WARN << "Error during class diagram generation: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Class diagram", std::string("Failed to execute plant UML. Details: ") + e.what());
	}
	catch (std::exception& e)
	{
		LOG_WARN << "Error during class diagram generation: " << typeid(e).name() << ". Details: " << e.what();
		m_ui.errorMessage("Class diagram", e.what());
	}
}

} /* namespace CTagsPlugin */
