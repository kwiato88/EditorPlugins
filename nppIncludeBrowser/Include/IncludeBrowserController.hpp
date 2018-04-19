#ifndef INCLUDE_BROWSER_CONTROLLER
#define INCLUDE_BROWSER_CONTROLLER

#include <memory>
#include <set>

#include "Editor.hpp"
#include "UI.hpp"
#include "IPathGetter.hpp"
#include "IFileHierarchySelector.hpp"
#include "IncludeBrowser.hpp"
#include "MutilpeFilesIncludeBrowser.hpp"
#include "Transaction.hpp"
#include "MessageHandler.hpp"
#include "Commands.hpp"
#include "Results.hpp"

namespace IncludeBrowser
{

class Controller
{
public:
    Controller(Plugin::Editor& p_editor,
			   Plugin::UI& p_ui,
               std::shared_ptr<Plugin::IPathGetter> p_pathGetter,
			   std::shared_ptr<IFileHierarchySelector> p_fileSelector);

    void parse();
    void showIncluders();
    void showIncluded();
    void clear();
	void handleTransaction(long p_id, Messaging::Transaction& p_trans);

private:
    static Browser::ExtensionParserMap buildParsers();
    static std::shared_ptr<IBrowser> buildBrowser(const std::string& p_dirPath);
    void parseIncludes(const std::string& p_sourceDir);
    std::string getSourceDir() const;
    std::string getCurrentFileName() const;
    std::string selectFile(const std::vector<std::string>& p_files) const;
	std::string selectFile(const FileHierarchy& p_files) const;
    std::string selectFileFromMultiple(const std::vector<std::string>& p_files) const;
	std::vector<std::string> findFiles(const std::string& p_fileName) const;
    void goToFile(const std::string& p_file);

	Result::Basic handleClear(const Command::Clear&);
	Result::Basic handleParse(const Command::Parse&);
	template<typename Command, typename Result, typename Handler>
	void addHandler(Handler p_handler)
	{
		m_handlers.addHandler<Command, Result>(Command::Id(), std::bind(p_handler, this, std::placeholders::_1));
	}

	Plugin::Editor& m_editor;
	Plugin::UI& m_ui;
    MultipleFilesBrowser m_browser;
    std::set<std::string> m_sourceDirs;
    std::shared_ptr<Plugin::IPathGetter> m_pathGetter;
	std::shared_ptr<IFileHierarchySelector> m_fileSelector;
	Messaging::Handlers m_handlers;
};

} // namespace IncludeBrowser

#endif // INCLUDE_BROWSER_CONTROLLER
