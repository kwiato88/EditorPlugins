#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstdlib>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include "Project.hpp"
#include "Workspace.hpp"

#include "UiMock.hpp"
#include "ITagsMock.hpp"
#include "IIncludesMock.hpp"
#include "IFilesMock.hpp"

namespace ProjectMgmt
{
using namespace ::testing;

using Strings = std::vector<std::string>;

class TagsProxy : public ITags
{
public:
	TagsProxy(ITags& p_tags) : tags(p_tags) {}
	void generateTags(const std::string& p_tagFilePath, const std::vector<std::string>& p_sourceDirsPaths)
	{
		tags.generateTags(p_tagFilePath, p_sourceDirsPaths);
	}
	void setTagFiles(const std::vector<std::string>& p_filesPaths)
	{
		tags.setTagFiles(p_filesPaths);
	}
	std::vector<std::string> getTagFiles()
	{
		return tags.getTagFiles();
	}
private:
	ITags& tags;
};

class IncludesProxy : public IIncludes
{
public:
	IncludesProxy(IIncludes& p_inc) : inc(p_inc) {}
	void parse(const std::string& p_dirPath)
	{
		inc.parse(p_dirPath);
	}
	void clear()
	{
		inc.clear();
	}
private:
	IIncludes& inc;
};

class FilesProxy : public IFiles
{
public:
	FilesProxy(IFiles& p_files) : files(p_files) {}
	void setSearchDirs(const std::vector<std::string>& p_dirs)
	{
		files.setSearchDirs(p_dirs);
	}
	std::vector<std::string> getSearchDirs()
	{
		return files.getSearchDirs();
	}
private:
	IFiles& files;
};

struct ProjectMgmtMT : public Test
{
	ProjectMgmtMT()
	{
		char* envVar = std::getenv("projectRootPath");
		if(envVar == nullptr)
			throw std::runtime_error("Failed to startup test. Env var 'projectRootPath' is not set");
		rootPath = envVar;
		testsRootPath = rootPath + "nppProjectPlugin\\Tests\\";
		projectsDirPath = testsRootPath + "Projects\\";
		EXPECT_CALL(tagsNiceMock, getTagFiles()).WillRepeatedly(Return(Strings()));
		EXPECT_CALL(filesNiceMock, getSearchDirs()).WillRepeatedly(Return(Strings()));
	}

	std::string rootPath;
	std::string testsRootPath;
	std::string projectsDirPath;

	StrictMock<ITagsMock> tagsMock;
	NiceMock<ITagsMock> tagsNiceMock;
	StrictMock<IIncludesMock> incMock;
	NiceMock<IIncludesMock> incNiceMock;
	StrictMock<IFilesMock> filesMock;
	NiceMock<IFilesMock> filesNiceMock;
	StrictMock<Plugin::UiMock> uiMock;
};

struct ProjectMT : public ProjectMgmtMT
{
	Project loadProject(const std::string& p_projectFile)
	{
		boost::property_tree::ptree projectData;
		boost::property_tree::json_parser::read_json(projectsDirPath + p_projectFile, projectData);
		
		return Project(projectData, tagsNiceMock, incNiceMock, filesNiceMock);
	}
	void saveProject(const Project& p_project, const std::string& p_projectFile)
	{
		boost::property_tree::json_parser::write_json(projectsDirPath + p_projectFile, p_project.exportData());
	}
};

TEST_F(ProjectMT, shouldLoadProjectFromConfigFile)
{
	ASSERT_EQ(
		(Project{ "NewProject",
				{
					Elem{ "d:\\dir1", "d:\\dir1\\file1.txt" },
					Elem{ "d:\\dir2", "d:\\dir2\\file2.txt" }
				}}),
		loadProject("projectFile.json"));
}

TEST_F(ProjectMT, loadedProjectShouldBeTheSameAsSaved)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incNiceMock, false, true, true, false },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\file2.txt", tagsNiceMock, incNiceMock, true, false, false, true }
		}
	};
	
	saveProject(project, "tmpProjectFile.json");
	ASSERT_EQ(project, loadProject("tmpProjectFile.json"));
}

TEST_F(ProjectMT, shouldSaveAndLoadProjectWithoutItems)
{
	Project project{ "ProjectName",{} };

	saveProject(project, "tmpProjectFile.json");
	ASSERT_EQ(project, loadProject("tmpProjectFile.json"));
}

TEST_F(ProjectMT, shouldThrowWhenProjectNameIsNotDefinedInFile)
{
	ASSERT_THROW(loadProject("projectWithNoName.json"), std::runtime_error);
}

TEST_F(ProjectMT, shouldThrowWhenItemHasNoPathsDefined)
{
	ASSERT_THROW(loadProject("itemWithoutPaths.json"), std::runtime_error);
}

TEST_F(ProjectMT, shoukdNotCreateItemWithTagNavigationButWithNoTagFile)
{
	ASSERT_THROW(Elem("source", "", tagsNiceMock, incNiceMock, false, true), std::runtime_error);
}

TEST_F(ProjectMT, shoukdNotCreateItemWithTagGenerationButWithNoTagFile)
{
	ASSERT_THROW(Elem("source", "", tagsNiceMock, incNiceMock, true, false), std::runtime_error);
}

TEST_F(ProjectMT, shoukdNotCreateItemWithTagGenerationButWithNoSource)
{
	ASSERT_THROW(Elem("", "tagFile", tagsNiceMock, incNiceMock, true, false), std::runtime_error);
}

TEST_F(ProjectMT, shoukdNotCreateItemWithIncludesNavigationButWithNoSource)
{
	ASSERT_THROW(Elem("", "tagFile", tagsNiceMock, incNiceMock, false, false, true), std::runtime_error);
}

TEST_F(ProjectMT, shoukdNotCreateItemFileSearchingButWithNoSource)
{
	ASSERT_THROW(Elem("", "tagFile", tagsNiceMock, incNiceMock, false, false, false, true), std::runtime_error);
}

TEST_F(ProjectMT, shouldSetTagFilesPathsWhenRefreshCodeNavigation)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsMock, incNiceMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsMock, incNiceMock }
		},
		tagsMock,
		incNiceMock,
		filesNiceMock
	};

	EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir1\\file1.txt", "d:\\dir2\\dir3\\file2.txt" })));
	project.refershCodeNavigation();
}

TEST_F(ProjectMT, shouldIgnoreItemsWithDisabledTagsNavigationDuringRefreshCodeNavigation)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsMock, incNiceMock, true, false },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsMock, incNiceMock }
		},
		tagsMock,
		incNiceMock,
		filesNiceMock
	};

	EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir2\\dir3\\file2.txt" })));
	project.refershCodeNavigation();
}

TEST_F(ProjectMT, shouldParseIncludesWhenRefreshCodeNavigation)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsNiceMock, incMock }
		},
		tagsNiceMock,
		incMock,
		filesNiceMock
	};

	EXPECT_CALL(incMock, clear());
	EXPECT_CALL(incMock, parse("d:\\dir1\\dir"));
	EXPECT_CALL(incMock, parse("d:\\dir2\\dir"));
	project.refershCodeNavigation();
}

TEST_F(ProjectMT, shouldIgnoreItemsWithDisabledIncludesNavigationWhenRefreshCodeNavigation)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incMock, true, true, false },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsNiceMock, incMock }
		},
		tagsNiceMock,
		incMock,
		filesNiceMock
	};

	EXPECT_CALL(incMock, clear());
	EXPECT_CALL(incMock, parse("d:\\dir2\\dir"));
	project.refershCodeNavigation();
}

TEST_F(ProjectMT, shouldSetSearchDirsPathsWhenRefreshCodeNavigation)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incNiceMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsNiceMock, incNiceMock }
		},
		tagsNiceMock,
		incNiceMock,
		filesMock
	};

	EXPECT_CALL(filesMock, setSearchDirs(Strings({ "d:\\dir1\\dir", "d:\\dir2\\dir" })));
	project.refershCodeNavigation();
}

TEST_F(ProjectMT, shouldIgnoreItemsWithDisabledFileSearchingDuringRefreshCodeNavigation)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incNiceMock, true, true, true, false },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsNiceMock, incNiceMock }
		},
		tagsNiceMock,
		incNiceMock,
		filesMock
	};

	EXPECT_CALL(filesMock, setSearchDirs(Strings({ "d:\\dir2\\dir" })));
	project.refershCodeNavigation();
}

TEST_F(ProjectMT, shouldGenerateTagsAndSetTagFilesDuringRefresh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsMock, incNiceMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsMock, incNiceMock }
		},
		tagsMock,
		incNiceMock,
		filesNiceMock
	};

	EXPECT_CALL(tagsMock, generateTags("d:\\dir1\\file1.txt", Strings({ "d:\\dir1\\dir" })));
	EXPECT_CALL(tagsMock, generateTags("d:\\dir2\\dir3\\file2.txt", Strings({ "d:\\dir2\\dir" })));
	EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir1\\file1.txt", "d:\\dir2\\dir3\\file2.txt" })));
	project.refresh();
}

TEST_F(ProjectMT, shouldSupprotSpacesDuringRefersh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1 q\\dir w", "d:\\dir 2\\file 1.txt", tagsMock, incNiceMock },
		},
		tagsMock,
		incNiceMock,
		filesNiceMock
	};

	EXPECT_CALL(tagsMock, generateTags("d:\\dir 2\\file 1.txt", Strings({ "d:\\dir1 q\\dir w" })));
	EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir 2\\file 1.txt" })));
	project.refresh();
}

TEST_F(ProjectMT, shouldIgnoreItemWithDisabledTagsGenerationDuringRefresh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\file1.txt", tagsMock, incNiceMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsMock, incNiceMock, false }
		},
		tagsMock,
		incNiceMock,
		filesNiceMock
	};

	EXPECT_CALL(tagsMock, generateTags("d:\\file1.txt", Strings({ "d:\\dir1\\dir" })));
	EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\file1.txt", "d:\\dir2\\dir3\\file2.txt" })));
	project.refresh();
}

TEST_F(ProjectMT, shouldParseIncludesDuringRefresh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsNiceMock, incMock }
		},
		tagsNiceMock,
		incMock,
		filesNiceMock
	};

	EXPECT_CALL(incMock, clear());
	EXPECT_CALL(incMock, parse("d:\\dir1\\dir"));
	EXPECT_CALL(incMock, parse("d:\\dir2\\dir"));
	project.refresh();
}

TEST_F(ProjectMT, shouldIgnoreItemWithDisabledIncludesNavigationDuringRefresh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incMock, true, true, false },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsNiceMock, incMock }
		},
		tagsNiceMock,
		incMock,
		filesNiceMock
	};

	EXPECT_CALL(incMock, clear());
	EXPECT_CALL(incMock, parse("d:\\dir2\\dir"));
	project.refresh();
}

TEST_F(ProjectMT, shouldSetSearchDirsPathsDuringRefresh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incNiceMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsNiceMock, incNiceMock }
		},
		tagsNiceMock,
		incNiceMock,
		filesMock
	};

	EXPECT_CALL(filesMock, setSearchDirs(Strings({ "d:\\dir1\\dir", "d:\\dir2\\dir" })));
	project.refresh();
}

TEST_F(ProjectMT, shouldIgnoreItemsWithDisabledFileSearchingDuringRefresh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsNiceMock, incNiceMock, true, true, true, false },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsNiceMock, incNiceMock }
		},
		tagsNiceMock,
		incNiceMock,
		filesMock
	};

	EXPECT_CALL(filesMock, setSearchDirs(Strings({ "d:\\dir2\\dir" })));
	project.refresh();
}

struct WorkspaceMT : public ProjectMgmtMT
{
	~WorkspaceMT()
	{
		if (!projectDirToCleanup.empty())
			deleteProjectDirIfExists(projectDirToCleanup);
	}

	bool doesDirExist(const std::string& p_path)
	{
		return boost::filesystem::exists(p_path) && boost::filesystem::is_directory(p_path);
	}
	bool doesFileExist(const std::string& p_path)
	{
		return boost::filesystem::exists(p_path) && boost::filesystem::is_regular_file(p_path);
	}
	void deleteIfExists(const std::string& p_path)
	{
		if (boost::filesystem::exists(p_path))
			boost::filesystem::remove(p_path);
	}
	void deleteProjectDirIfExists(const std::string& p_rojectDirPath)
	{
		deleteIfExists(p_rojectDirPath + "\\project.json");
		deleteIfExists(p_rojectDirPath);
	}
	Workspace buildWorkspace(const std::string& p_workspaceDir)
	{
		return Workspace(std::make_unique<TagsProxy>(tagsMock),
			std::make_unique<IncludesProxy>(incMock),
			std::make_unique<FilesProxy>(filesMock),
			uiMock, std::bind(&WorkspaceMT::createProject, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5),
			testsRootPath + p_workspaceDir);
	}
	Workspace buildWorkspaceWithNiceMocks(const std::string& p_workspaceDir)
	{
		return Workspace(std::make_unique<TagsProxy>(tagsNiceMock),
			std::make_unique<IncludesProxy>(incNiceMock),
			std::make_unique<FilesProxy>(filesNiceMock),
			uiMock, std::bind(&WorkspaceMT::createProject, this,
				std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5),
			testsRootPath + p_workspaceDir);
	}
	std::unique_ptr<Project> loadProject(const std::string& p_projectFilePath)
	{
		boost::property_tree::ptree projectData;
		boost::property_tree::json_parser::read_json(p_projectFilePath, projectData);

		return std::make_unique<Project>(projectData, tagsMock, incMock, filesMock);
	}
	std::unique_ptr<Project> createProject(const Project&, ITags&, IIncludes&, IFiles&, GetTagFilePath)
	{
		return std::move(modifiedProject);
	}

	std::string projectDirToCleanup;
	std::unique_ptr<Project> modifiedProject;
};

TEST_F(WorkspaceMT, shouldPrintMessageWhenWorkspaceDirDoesNotExist)
{
	ASSERT_FALSE(doesDirExist(testsRootPath + "notExistingDir"));
	Workspace sut(buildWorkspaceWithNiceMocks("notExistingDir"));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldPrintMessageWhenWorkspacePathIsNotADir)
{
	ASSERT_TRUE(doesFileExist(testsRootPath + "WorkspaceFile"));
	Workspace sut(buildWorkspaceWithNiceMocks("WorkspaceFile"));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldOpenSelectedProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspaceWithNiceMocks("Workspace"));
	
	EXPECT_CALL(uiMock, selectRow(
		Strings({ "Project", "Path" }),
		std::vector<Plugin::UI::Row>(
		{
			Plugin::UI::Row({"FirstProject", testsRootPath + "Workspace\\FirstProject"}),
			Plugin::UI::Row({ "ModifiedFirstProject1", testsRootPath + "Workspace\\ModifiedFirstProject1" }),
			Plugin::UI::Row({ "ProjectWithIncBrowser", testsRootPath + "Workspace\\ProjectWithIncBrowser" }),
			Plugin::UI::Row({ "ProjectWithSearchFiles", testsRootPath + "Workspace\\ProjectWithSearchFiles" }),
			Plugin::UI::Row({"SecondProject", testsRootPath + "Workspace\\SecondProject" })
		}),
		Plugin::UI::Row({ "", "" })))
		.WillOnce(Return(Plugin::UI::Row({ "SecondProject", testsRootPath + "Workspace\\SecondProject" })));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldNotOpenProjectWhenOneAlreadyOpened)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspaceWithNiceMocks("Workspace"));
	
	EXPECT_CALL(uiMock, selectRow(_, _,	_))
		.WillOnce(Return(Plugin::UI::Row({ "FirstProject", testsRootPath + "Workspace\\FirstProject" })));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
	
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldPrintMessageWhenNoProjectFile)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(buildWorkspaceWithNiceMocks("WorkspaceWithInvalidProjects"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "NoProjectFile", testsRootPath + "WorkspaceWithInvalidProjects\\NoProjectFile" })));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldPrintMessageWhenProjectFileIsInvalid)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(buildWorkspaceWithNiceMocks("WorkspaceWithInvalidProjects"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "InvaliProjectFile", testsRootPath + "WorkspaceWithInvalidProjects\\InvaliProjectFile" })));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldPrintMessageWhenProjectNameIsNoSameAsProjectDir)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(buildWorkspaceWithNiceMocks("WorkspaceWithInvalidProjects"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "WrongProjectName", testsRootPath + "WorkspaceWithInvalidProjects\\WrongProjectName" })));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldBeAbleToOpenProjectWhenPreviousOpenFailed)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(buildWorkspaceWithNiceMocks("WorkspaceWithInvalidProjects"));
	InSequence dummy;

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "NoProjectFile", testsRootPath + "WorkspaceWithInvalidProjects\\NoProjectFile" })));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "ValidProject", testsRootPath + "WorkspaceWithInvalidProjects\\ValidProject" })));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldOpenAndCloseProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspaceWithNiceMocks("Workspace"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "SecondProject", testsRootPath + "Workspace\\SecondProject" })));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, shouldSetTagFilesPathsWhenOpenProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "SecondProject", testsRootPath + "Workspace\\SecondProject" })));
	{
		InSequence seq;
		EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
		EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir12\\file12.txt", "d:\\dir43\\file2.txt" })));
		EXPECT_CALL(tagsMock, setTagFiles(Strings()));
	}
	EXPECT_CALL(incMock, clear()).Times(2);
	EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
	EXPECT_CALL(filesMock, setSearchDirs(Strings({}))).Times(2);
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldRestoreTagFilesPathsClearIncludesClearSearchDirsAfterCloseProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	Strings originalTagFilesPaths { "D:\\tagFile1.txt", "D:\\tagFile2.txt", "D:\\dir\\TagFile3.txt" };
	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "SecondProject", testsRootPath + "Workspace\\SecondProject" })));
	{
		InSequence seq;
		EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(originalTagFilesPaths));
		EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir12\\file12.txt", "d:\\dir43\\file2.txt" })));
		EXPECT_CALL(tagsMock, setTagFiles(originalTagFilesPaths));
	}
	EXPECT_CALL(incMock, clear()).Times(2);
	EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
	EXPECT_CALL(filesMock, setSearchDirs(Strings({}))).Times(2);
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));

	sut.openProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, shouldParseIncludesWhenOpenProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "ProjectWithIncBrowser", testsRootPath + "Workspace\\ProjectWithIncBrowser" })));
	{
		InSequence seq;
		EXPECT_CALL(incMock, clear());
		EXPECT_CALL(incMock, parse("d:\\dir12"));
		EXPECT_CALL(incMock, parse("d:\\dir44\\dir"));
		EXPECT_CALL(incMock, clear());
	}
	EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
	EXPECT_CALL(tagsMock, setTagFiles(Strings())).Times(2);
	EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
	EXPECT_CALL(filesMock, setSearchDirs(Strings({}))).Times(2);
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldClearIncludesWhenCloseProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "ProjectWithIncBrowser", testsRootPath + "Workspace\\ProjectWithIncBrowser" })));
	{
		InSequence seq;
		EXPECT_CALL(incMock, clear());
		EXPECT_CALL(incMock, parse("d:\\dir12"));
		EXPECT_CALL(incMock, parse("d:\\dir44\\dir"));
		EXPECT_CALL(incMock, clear());
	}
	EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
	EXPECT_CALL(tagsMock, setTagFiles(Strings())).Times(2);
	EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
	EXPECT_CALL(filesMock, setSearchDirs(Strings({}))).Times(2);
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, shouldSetSearchDirsPathsWhenOpenProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "ProjectWithSearchFiles", testsRootPath + "Workspace\\ProjectWithSearchFiles" })));
	{
		InSequence seq;
		EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
		EXPECT_CALL(filesMock, setSearchDirs(Strings({"d:\\dir12", "d:\\dir44\\dir"})));
		EXPECT_CALL(filesMock, setSearchDirs(Strings()));
	}
	EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
	EXPECT_CALL(tagsMock, setTagFiles(Strings())).Times(2);
	EXPECT_CALL(incMock, clear()).Times(2);
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldRestoreSearchDirsPathsAfterCloseProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	Strings originalSearchDirsPaths{ "D:\\dir", "D:\\dir2\\dir3", "D:\\dir44" };
	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "ProjectWithSearchFiles", testsRootPath + "Workspace\\ProjectWithSearchFiles" })));
	{
		InSequence seq;
		EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(originalSearchDirsPaths));
		EXPECT_CALL(filesMock, setSearchDirs(Strings({ "d:\\dir12", "d:\\dir44\\dir" })));
		EXPECT_CALL(filesMock, setSearchDirs(originalSearchDirsPaths));
	}
	EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
	EXPECT_CALL(tagsMock, setTagFiles(Strings())).Times(2);
	EXPECT_CALL(incMock, clear()).Times(2);
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));

	sut.openProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, closingNewProjectShouldCreateProjectDirAndFile)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	Workspace sut(buildWorkspaceWithNiceMocks("Workspace"));

	std::string projectDir = testsRootPath + "Workspace\\NewProject";
	projectDirToCleanup = projectDir;
	deleteProjectDirIfExists(projectDir);
	EXPECT_CALL(uiMock, query(_, "NewProject", "")).WillOnce(Return("NewProject"));
	sut.newProject();
	ASSERT_TRUE(doesDirExist(projectDir));
	sut.closeProject();
	ASSERT_TRUE(doesFileExist(projectDir + "\\project.json"));
}

TEST_F(WorkspaceMT, shouldNotCreateNewProjectWhenOneWithGivenNameAlreadyExists)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	Workspace sut(buildWorkspaceWithNiceMocks( "Workspace"));

	std::string projectDir = testsRootPath + "Workspace\\NewProject";
	projectDirToCleanup = projectDir;
	deleteProjectDirIfExists(projectDir);
	EXPECT_CALL(uiMock, query(_, _, _)).WillRepeatedly(Return("NewProject"));
	sut.newProject();
	sut.closeProject();
	ASSERT_TRUE(doesDirExist(projectDir));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.newProject();
}

TEST_F(WorkspaceMT, shouldNotCreateNewProjectIfWorkspaceIsInvalid)
{
	ASSERT_FALSE(doesDirExist(testsRootPath + "NotExistingWorkspace"));
	Workspace sut(buildWorkspaceWithNiceMocks("NotExistingWorkspace"));
	EXPECT_CALL(uiMock, query(_, _, _)).WillOnce(Return("NewProject"));
	EXPECT_CALL(uiMock, errorMessage(_, _)).Times(AtLeast(0));
	sut.newProject();
	ASSERT_FALSE(doesDirExist(testsRootPath + "NotExistingWorkspace"));
	ASSERT_FALSE(doesDirExist(testsRootPath + "NotExistingWorkspace\\NewProject"));
}

TEST_F(WorkspaceMT, shouldClearTagFilesPathsParsedIncludesClearSearchDirsWhenCreateNewProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));
	projectDirToCleanup = testsRootPath + "Workspace\\NewProject";

	EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
	EXPECT_CALL(tagsMock, setTagFiles(Strings({}))).Times(2);
	EXPECT_CALL(incMock, clear()).Times(2);
	EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
	EXPECT_CALL(filesMock, setSearchDirs(Strings({}))).Times(2);
	EXPECT_CALL(uiMock, query(_, _, _)).WillOnce(Return("NewProject"));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.newProject();
}

TEST_F(WorkspaceMT, shouldRestoreTagFilesPathsClearIncludesClearSearchDirsAfterCloseNewProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));
	projectDirToCleanup = testsRootPath + "Workspace\\NewProject";
	
	Strings originalTagFilesPaths{ "D:\\tagFile1.txt", "D:\\tagFile2.txt", "D:\\dir\\TagFile3.txt" };
	{
		InSequence seq;
		EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(originalTagFilesPaths));
		EXPECT_CALL(tagsMock, setTagFiles(Strings({})));
		EXPECT_CALL(tagsMock, setTagFiles(originalTagFilesPaths));
	}
	EXPECT_CALL(incMock, clear()).Times(2);
	EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
	EXPECT_CALL(filesMock, setSearchDirs(Strings({}))).Times(2);
	EXPECT_CALL(uiMock, query(_, _, _)).WillOnce(Return("NewProject"));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	
	sut.newProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, shouldGenerateTagsDuringRefreshProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "SecondProject", testsRootPath + "Workspace\\SecondProject" })));
	{
		InSequence seq;
		EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
		EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir12\\file12.txt", "d:\\dir43\\file2.txt" })));
		EXPECT_CALL(tagsMock, generateTags("d:\\dir12\\file12.txt", Strings({ "d:\\dir12" })));
		EXPECT_CALL(tagsMock, generateTags("d:\\dir43\\file2.txt", Strings({ "d:\\dir44\\dir" })));
		EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir12\\file12.txt", "d:\\dir43\\file2.txt" })));
		EXPECT_CALL(tagsMock, setTagFiles(Strings()));
	}
	EXPECT_CALL(incMock, clear()).Times(3);
	EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
	EXPECT_CALL(filesMock, setSearchDirs(Strings({}))).Times(3);
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));

	sut.openProject();
	sut.refreshProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, shouldParseIncludesDuringRefreshProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "ProjectWithIncBrowser", testsRootPath + "Workspace\\ProjectWithIncBrowser" })));
	{
		InSequence seq;
		EXPECT_CALL(incMock, clear());
		EXPECT_CALL(incMock, parse("d:\\dir12"));
		EXPECT_CALL(incMock, parse("d:\\dir44\\dir"));
		EXPECT_CALL(incMock, clear());
		EXPECT_CALL(incMock, parse("d:\\dir12"));
		EXPECT_CALL(incMock, parse("d:\\dir44\\dir"));
		EXPECT_CALL(incMock, clear());
	}
	EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
	EXPECT_CALL(tagsMock, setTagFiles(Strings())).Times(3);
	EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(Strings()));
	EXPECT_CALL(filesMock, setSearchDirs(Strings({}))).Times(3);
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));

	sut.openProject();
	sut.refreshProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, shouldRestoreDataAfterCloseModifiedProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	Strings originalSearchDirsPaths{ "D:\\dir", "D:\\dir2\\dir3", "D:\\dir44" };
	Strings originalTagFilesPaths{ "D:\\file1.txt", "D:\\file2.txt", "D:\\file3.txt" };
	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "FirstProject", testsRootPath + "Workspace\\FirstProject" })));
	{
		InSequence seq;
		EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(originalSearchDirsPaths));
		EXPECT_CALL(filesMock, setSearchDirs(Strings({ "d:\\dir1", "d:\\dir2" })));
		EXPECT_CALL(filesMock, setSearchDirs(originalSearchDirsPaths));
	}
	{
		InSequence seq;
		EXPECT_CALL(incMock, clear());
		EXPECT_CALL(incMock, parse("d:\\dir1"));
		EXPECT_CALL(incMock, parse("d:\\dir2"));
		EXPECT_CALL(incMock, clear());
	}
	{
		InSequence seq;
		EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(originalTagFilesPaths));
		EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir1\\file1.txt", "d:\\dir2\\file2.txt" })));
		EXPECT_CALL(tagsMock, setTagFiles(originalTagFilesPaths));
	}
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));

	sut.openProject();
	modifiedProject = std::move(loadProject(testsRootPath + "Workspace\\FirstProject\\project.json"));
	sut.modifyProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, shouldRefreshProjectWithModifiedPaths)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));

	Strings originalSearchDirsPaths{ "D:\\dir", "D:\\dir2\\dir3", "D:\\dir44" };
	Strings originalTagFilesPaths{ "D:\\file1.txt", "D:\\file2.txt", "D:\\file3.txt" };
	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "FirstProject", testsRootPath + "Workspace\\FirstProject" })));
	{
		InSequence seq;
		EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(originalSearchDirsPaths));
		EXPECT_CALL(filesMock, setSearchDirs(Strings({ "d:\\dir1", "d:\\dir2" })));
		EXPECT_CALL(filesMock, setSearchDirs(originalSearchDirsPaths));
		EXPECT_CALL(filesMock, getSearchDirs()).WillOnce(Return(originalSearchDirsPaths));
		EXPECT_CALL(filesMock, setSearchDirs(Strings({ "d:\\dir1New", "d:\\dir2New" })));
		EXPECT_CALL(filesMock, setSearchDirs(originalSearchDirsPaths));
	}
	{
		InSequence seq;
		EXPECT_CALL(incMock, clear());
		EXPECT_CALL(incMock, parse("d:\\dir1"));
		EXPECT_CALL(incMock, parse("d:\\dir2"));
		EXPECT_CALL(incMock, clear());
		EXPECT_CALL(incMock, clear());
		EXPECT_CALL(incMock, parse("d:\\dir1New"));
		EXPECT_CALL(incMock, parse("d:\\dir2New"));
		EXPECT_CALL(incMock, clear());
	}
	{
		InSequence seq;
		EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(originalTagFilesPaths));
		EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir1\\file1.txt", "d:\\dir2\\file2.txt" })));
		EXPECT_CALL(tagsMock, setTagFiles(originalTagFilesPaths));
		EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(originalTagFilesPaths));
		EXPECT_CALL(tagsMock, generateTags("d:\\dir1\\file1New.txt", Strings({ "d:\\dir1New" })));
		EXPECT_CALL(tagsMock, generateTags("d:\\dir2\\file2New.txt", Strings({ "d:\\dir2New" })));
		EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir1\\file1New.txt", "d:\\dir2\\file2New.txt" })));
		EXPECT_CALL(tagsMock, setTagFiles(originalTagFilesPaths));
	}
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));

	sut.openProject();
	modifiedProject = std::move(loadProject(testsRootPath + "Workspace\\ModifiedFirstProject1\\project.json"));
	sut.modifyProject();
}

}
