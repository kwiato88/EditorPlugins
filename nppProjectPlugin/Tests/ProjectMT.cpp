#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstdlib>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include "Project.hpp"
#include "Workspace.hpp"

#include "UiMock.hpp"
#include "ITagsMock.hpp"

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

struct ProjectMgmyMT : public Test
{
	ProjectMgmyMT()
	{
		rootPath = std::getenv("projectRootPath");
		testsRootPath = rootPath + "nppProjectPlugin\\Tests\\";
		projectsDirPath = testsRootPath + "Projects\\";
		EXPECT_CALL(tagsNiceMock, getTagFiles()).WillRepeatedly(Return(Strings()));
	}

	std::string rootPath;
	std::string testsRootPath;
	std::string projectsDirPath;

	StrictMock<ITagsMock> tagsMock;
	NiceMock<ITagsMock> tagsNiceMock;
	StrictMock<Plugin::UiMock> uiMock;
};

struct ProjectMT : public ProjectMgmyMT
{
	Project loadProject(const std::string& p_projectFile)
	{
		boost::property_tree::ptree projectData;
		boost::property_tree::json_parser::read_json(projectsDirPath + p_projectFile, projectData);
		
		return Project(projectData);
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
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", g_noTags, false, true },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\file2.txt", g_noTags, true, false }
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
	ASSERT_THROW(Elem("source", "", tagsNiceMock, false, true), std::runtime_error);
}

TEST_F(ProjectMT, shoukdNotCreateItemWithTagGenerationButWithNoTagFile)
{
	ASSERT_THROW(Elem("source", "", tagsNiceMock, true, false), std::runtime_error);
}

TEST_F(ProjectMT, shoukdNotCreateItemWithTagGenerationButWithNoSource)
{
	ASSERT_THROW(Elem("", "tagFile", tagsNiceMock, true, false), std::runtime_error);
}

TEST_F(ProjectMT, shouldSetTagFilesPathsWhenRefreshCodeNavigation)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsMock }
		},
		tagsMock
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
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsMock, true, false },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsMock }
		},
		tagsMock
	};

	EXPECT_CALL(tagsMock, setTagFiles(Strings({ "d:\\dir2\\dir3\\file2.txt" })));
	project.refershCodeNavigation();
}

TEST_F(ProjectMT, shouldGenerateTagsDuringRefresh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt", tagsMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsMock }
		},
		tagsMock
	};

	EXPECT_CALL(tagsMock, generateTags("d:\\dir1\\file1.txt", Strings({ "d:\\dir1\\dir" })));
	EXPECT_CALL(tagsMock, generateTags("d:\\dir2\\dir3\\file2.txt", Strings({ "d:\\dir2\\dir" })));
	project.refresh();
}

TEST_F(ProjectMT, shouldSupprotSpacesDuringRefersh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1 q\\dir w", "d:\\dir 2\\file 1.txt", tagsMock },
		},
		tagsMock
	};

	EXPECT_CALL(tagsMock, generateTags("d:\\dir 2\\file 1.txt", Strings({ "d:\\dir1 q\\dir w" })));
	project.refresh();
}

TEST_F(ProjectMT, shouldIgnoreItemWithDisabledTagsGenerationDuringRefresh)
{
	Project project
	{
		"ProjectName",
		{
			Elem{ "d:\\dir1\\dir", "d:\\file1.txt", tagsMock },
			Elem{ "d:\\dir2\\dir", "d:\\dir2\\dir3\\file2.txt", tagsMock, false }
		},
		tagsMock
	};

	EXPECT_CALL(tagsMock, generateTags("d:\\file1.txt", Strings({ "d:\\dir1\\dir" })));
	project.refresh();
}

struct WorkspaceMT : public ProjectMgmyMT
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
		return Workspace(std::make_unique<TagsProxy>(tagsMock), uiMock, testsRootPath + p_workspaceDir);
	}
	Workspace buildWorkspaceWithNiceTagsMock(const std::string& p_workspaceDir)
	{
		return Workspace(std::make_unique<TagsProxy>(tagsNiceMock), uiMock, testsRootPath + p_workspaceDir);
	}

	std::string projectDirToCleanup;
};

TEST_F(WorkspaceMT, shouldPrintMessageWhenWorkspaceDirDoesNotExist)
{
	ASSERT_FALSE(doesDirExist(testsRootPath + "notExistingDir"));
	Workspace sut(buildWorkspaceWithNiceTagsMock("notExistingDir"));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldPrintMessageWhenWorkspacePathIsNotADir)
{
	ASSERT_TRUE(doesFileExist(testsRootPath + "WorkspaceFile"));
	Workspace sut(buildWorkspaceWithNiceTagsMock("WorkspaceFile"));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldOpenSelectedProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspaceWithNiceTagsMock("Workspace"));
	
	EXPECT_CALL(uiMock, selectRow(
		Strings({ "Project", "Path" }),
		std::vector<Plugin::UI::Row>(
		{
			Plugin::UI::Row({"FirstProject", testsRootPath + "Workspace\\FirstProject"}),
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
	Workspace sut(buildWorkspaceWithNiceTagsMock("Workspace"));
	
	EXPECT_CALL(uiMock, selectRow(_, _,	_))
		.WillOnce(Return(Plugin::UI::Row({ "FirstProject", testsRootPath + "Workspace\\FirstProject" })));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
	
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldPrintMessageWhenNoProjectFile)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(buildWorkspaceWithNiceTagsMock("WorkspaceWithInvalidProjects"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "NoProjectFile", testsRootPath + "WorkspaceWithInvalidProjects\\NoProjectFile" })));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldPrintMessageWhenProjectFileIsInvalid)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(buildWorkspaceWithNiceTagsMock("WorkspaceWithInvalidProjects"));

	EXPECT_CALL(uiMock, selectRow(_, _, _))
		.WillOnce(Return(Plugin::UI::Row({ "InvaliProjectFile", testsRootPath + "WorkspaceWithInvalidProjects\\InvaliProjectFile" })));
	EXPECT_CALL(uiMock, errorMessage(_, _));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldBeAbleToOpenProjectWhenPreviousOpenFailed)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(buildWorkspaceWithNiceTagsMock("WorkspaceWithInvalidProjects"));
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
	Workspace sut(buildWorkspaceWithNiceTagsMock("Workspace"));

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
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(WorkspaceMT, shouldRestoreTagFilesPathsAfterCloseProject)
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
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));

	sut.openProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, closingNewProjectShouldCreateProjectDirAndFile)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	Workspace sut(buildWorkspaceWithNiceTagsMock("Workspace"));

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
	Workspace sut(buildWorkspaceWithNiceTagsMock( "Workspace"));

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
	Workspace sut(buildWorkspaceWithNiceTagsMock("NotExistingWorkspace"));
	EXPECT_CALL(uiMock, query(_, _, _)).WillOnce(Return("NewProject"));
	EXPECT_CALL(uiMock, errorMessage(_, _)).Times(AtLeast(0));
	sut.newProject();
	ASSERT_FALSE(doesDirExist(testsRootPath + "NotExistingWorkspace"));
	ASSERT_FALSE(doesDirExist(testsRootPath + "NotExistingWorkspace\\NewProject"));
}

TEST_F(WorkspaceMT, shouldClearTagFilesPathsWhenCreateNewProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(buildWorkspace("Workspace"));
	projectDirToCleanup = testsRootPath + "Workspace\\NewProject";

	EXPECT_CALL(tagsMock, getTagFiles()).WillOnce(Return(Strings()));
	EXPECT_CALL(tagsMock, setTagFiles(Strings({}))).Times(2);
	EXPECT_CALL(uiMock, query(_, _, _)).WillOnce(Return("NewProject"));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	sut.newProject();
}

TEST_F(WorkspaceMT, shouldRestoreTagFilesPathsAfterCloseNewProject)
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
	EXPECT_CALL(uiMock, query(_, _, _)).WillOnce(Return("NewProject"));
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));
	
	sut.newProject();
	sut.closeProject();
}

TEST_F(WorkspaceMT, shouldRefreshProject)
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
		EXPECT_CALL(tagsMock, setTagFiles(Strings()));
	}
	EXPECT_CALL(uiMock, infoMessage(_, _)).Times(AtLeast(0));

	sut.openProject();
	sut.refreshProject();
	sut.closeProject();
}

}
