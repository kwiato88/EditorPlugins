#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstdlib>
#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include "Project.hpp"
#include "Workspace.hpp"

#include "MessagePrinterMock.hpp"
#include "ProjectSelectorMock.hpp"

namespace ProjectMgmt
{
using namespace ::testing;

class ProjectSelectorProxy : public IProjectsSelector
{
public:
	ProjectSelectorProxy(IProjectsSelector& p_other)
		: selector(p_other)
	{}

	std::string select(const std::vector<std::string>& p_projects) const override
	{
		return selector.select(p_projects);
	}

private:
	IProjectsSelector& selector;
};

struct ProjectMT : public Test
{
	ProjectMT()
	{
		rootPath = std::getenv("projectRootPath");
		testsRootPath = rootPath + "nppProjectPlugin\\Tests\\";
		projectsDirPath = testsRootPath + "Projects\\";
	}
	~ProjectMT()
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

	std::string rootPath;
	std::string testsRootPath;
	std::string projectsDirPath;
	std::string projectDirToCleanup;
	StrictMock<Plugin::MessagePrinterMock> printerMock;
	StrictMock<ProjectSelectorMock> selectorMock;
};

TEST_F(ProjectMT, shouldLoadProjectFromConfigFile)
{
	boost::property_tree::ptree projectData;
	boost::property_tree::json_parser::read_json(projectsDirPath + "projectFile.json", projectData);

	ASSERT_EQ(
		(Project{ "NewProject",
				{
					Elem{ "d:\\dir1", "d:\\dir1\\file1.txt" },
					Elem{ "d:\\dir2", "d:\\dir2\\file2.txt" }
				}}),
		Project{ projectData });
}

TEST_F(ProjectMT, loadedProjectShouldBeTheSameAsSaved)
{
	Project project{ "ProjectName",{ Elem{ "d:\\dir1\\dir", "d:\\dir1\\file1.txt" }, Elem{ "d:\\dir2\\dir", "d:\\dir2\\file2.txt" } } };
	std::string configFile = projectsDirPath + "tmpProjectFile.json";
	boost::property_tree::json_parser::write_json(configFile, project.exportData());

	boost::property_tree::ptree loadedData;
	boost::property_tree::json_parser::read_json(configFile, loadedData);

	ASSERT_EQ(project, Project(loadedData));
}

TEST_F(ProjectMT, shouldSaveAndLoadProjectWithoutItems)
{
	Project project{ "ProjectName",{} };
	std::string configFile = projectsDirPath + "tmpProjectFile.json";
	boost::property_tree::json_parser::write_json(configFile, project.exportData());

	boost::property_tree::ptree loadedData;
	boost::property_tree::json_parser::read_json(configFile, loadedData);

	ASSERT_EQ(project, Project(loadedData));
}

TEST_F(ProjectMT, shouldThrowWhenProjectNameIsNotDefinedInFile)
{
	boost::property_tree::ptree projectData;
	boost::property_tree::json_parser::read_json(projectsDirPath + "projectWithNoName.json" , projectData);

	ASSERT_THROW(Project{ projectData }, std::runtime_error);
}

TEST_F(ProjectMT, shouldThrowWhenItemHasNoPathsDefined)
{
	boost::property_tree::ptree projectData;
	boost::property_tree::json_parser::read_json(projectsDirPath + "itemWithoutPaths.json", projectData);

	ASSERT_THROW(Project{ projectData }, std::runtime_error);
}

TEST_F(ProjectMT, shouldPrintMessageWhenWorkspaceDirDoesNotExist)
{
	ASSERT_FALSE(doesDirExist(testsRootPath + "notExistingDir"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "notExistingDir");
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();
}

TEST_F(ProjectMT, shouldPrintMessageWhenWorkspacePathIsNotADir)
{
	ASSERT_TRUE(doesFileExist(testsRootPath + "WorkspaceFile"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "WorkspaceFile");
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();
}

TEST_F(ProjectMT, shouldOpenSelectedProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "Workspace");
	
	EXPECT_CALL(selectorMock, select(std::vector<std::string>(
	{ testsRootPath + "Workspace\\FirstProject", testsRootPath + "Workspace\\SecondProject" }))
		).WillOnce(Return(testsRootPath + "Workspace\\SecondProject"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(ProjectMT, shouldNotOpenProjectWhenOneAlreadyOpened)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "Workspace");
	
	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "Workspace\\FirstProject"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
	
	sut.openProject();
}

TEST_F(ProjectMT, shouldPrintMessageWhenNoProjectFile)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock),
		testsRootPath + "WorkspaceWithInvalidProjects");

	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "WorkspaceWithInvalidProjects\\NoProjectFile"));
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();
}

TEST_F(ProjectMT, shouldPrintMessageWhenProjectFileIsInvalid)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock),
		testsRootPath + "WorkspaceWithInvalidProjects");

	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "WorkspaceWithInvalidProjects\\InvaliProjectFile"));
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();
}

TEST_F(ProjectMT, shouldBeAbleToOpenProjectWhenPreviousOpenFailed)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "WorkspaceWithInvalidProjects"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock),
		testsRootPath + "WorkspaceWithInvalidProjects");
	InSequence dummy;

	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "WorkspaceWithInvalidProjects\\NoProjectFile"));
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();

	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "WorkspaceWithInvalidProjects\\ValidProject"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(ProjectMT, shouldOpenAndCloseProject)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "Workspace");

	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "Workspace\\SecondProject"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
	sut.closeProject();
}

TEST_F(ProjectMT, closingNewProjectShouldCreateProjectDirAndFile)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "Workspace");

	std::string projectDir = testsRootPath + "Workspace\\NewProject";
	projectDirToCleanup = projectDir;
	deleteProjectDirIfExists(projectDir);
	sut.newProject();
	ASSERT_TRUE(doesDirExist(projectDir));
	sut.closeProject();
	ASSERT_TRUE(doesFileExist(projectDir + "\\project.json"));
}

TEST_F(ProjectMT, shouldNotCreateNewProjectWhenOneWithGivenNameAlreadyExists)
{
	ASSERT_TRUE(doesDirExist(testsRootPath + "Workspace"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "Workspace");

	std::string projectDir = testsRootPath + "Workspace\\NewProject";
	projectDirToCleanup = projectDir;
	deleteProjectDirIfExists(projectDir);
	sut.newProject();
	sut.closeProject();
	ASSERT_TRUE(doesDirExist(projectDir));
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.newProject();
}

TEST_F(ProjectMT, shouldNotCreateNewProjectIfWorkspaceIsInvalid)
{
	ASSERT_FALSE(doesDirExist(testsRootPath + "NotExistingWorkspace"));
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "NotExistingWorkspace");
	EXPECT_CALL(printerMock, printErrorMessage(_, _)).Times(AtLeast(0));
	sut.newProject();
	ASSERT_FALSE(doesDirExist(testsRootPath + "NotExistingWorkspace"));
	ASSERT_FALSE(doesDirExist(testsRootPath + "NotExistingWorkspace\\NewProject"));
}

}