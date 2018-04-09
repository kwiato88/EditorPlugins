#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cstdlib>
#include <boost/property_tree/json_parser.hpp>
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
	std::string rootPath;
	std::string testsRootPath;
	std::string projectsDirPath;
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
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "notExistingDir");
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();
}

TEST_F(ProjectMT, shouldPrintMessageWhenWorkspacePathIsNotADir)
{
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "WorkspaceFile");
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();
}

TEST_F(ProjectMT, shouldOpenSelectedProject)
{
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "Workspace");
	
	EXPECT_CALL(selectorMock, select(std::vector<std::string>(
	{ testsRootPath + "Workspace\\FirstProject", testsRootPath + "Workspace\\SecondProject" }))
		).WillOnce(Return(testsRootPath + "Workspace\\SecondProject"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
}

TEST_F(ProjectMT, shouldNotOpenProjectWhenOneAlreadyOpened)
{
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "Workspace");
	
	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "Workspace\\FirstProject"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
	
	sut.openProject();
}

TEST_F(ProjectMT, shouldPrintMessageWhenNoProjectFile)
{
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock),
		testsRootPath + "WorkspaceWithInvalidProjects");

	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "WorkspaceWithInvalidProjects\\NoProjectFile"));
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();
}

TEST_F(ProjectMT, shouldPrintMessageWhenProjectFileIsInvalid)
{
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock),
		testsRootPath + "WorkspaceWithInvalidProjects");

	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "WorkspaceWithInvalidProjects\\InvaliProjectFile"));
	EXPECT_CALL(printerMock, printErrorMessage(_, _));
	sut.openProject();
}

TEST_F(ProjectMT, shouldBeAbleToOpenProjectWhenPreviousOpenFailed)
{
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
	Workspace sut(printerMock, std::make_unique<ProjectSelectorProxy>(selectorMock), testsRootPath + "Workspace");

	EXPECT_CALL(selectorMock, select(_))
		.WillOnce(Return(testsRootPath + "Workspace\\SecondProject"));
	EXPECT_CALL(printerMock, printInfoMessage(_, _)).Times(AtLeast(0));
	sut.openProject();
	sut.closeProject();
}
//MT:
// - close with invalid workspace
//   - workspace with invalid path
//   - new project
//   - close project failed due to invalid workspace
// - close project will create project dir and file
//   - workspace with valid path
//   - new project
//   - delete project dir and file if exist
//   - close project (assert project dir and file created)
}
