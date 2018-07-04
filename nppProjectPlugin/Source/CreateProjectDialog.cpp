#include "CreateProjectDialog.hpp"
#include "CreateProjectDialogDef.h"
#include "DialogMsgMatchers.hpp"

namespace WinApi
{

CreateProjectDialog::CreateProjectDialog(InstanceHandle p_hInstance, Handle p_parent)
	: Dialog(p_hInstance, p_parent, ResourceId(ID_CREATE_PROJECT_DIALOG), "Edit Project")
{
	registerHandler(MsgMatchers::ButtonClick(IDOK), std::bind(&CreateProjectDialog::onOkClick, this));
	registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&CreateProjectDialog::onCancelClick, this));
	registerHandler(MsgMatchers::ButtonClick(ID_ADD_BUTTON), std::bind(&CreateProjectDialog::onAddClick, this));
	registerHandler(MsgMatchers::ButtonClick(ID_DELETE_BUTTON), std::bind(&CreateProjectDialog::onDeleteClick, this));
	registerHandler(MsgMatchers::ButtonClick(ID_EDIT_BUTTON), std::bind(&CreateProjectDialog::onEditClick, this));
}

void CreateProjectDialog::onInit()
{
	itemsPaths.init(this->getItem(ResourceId(ID_ITEMS_NAMES)));
	itemsPaths.addColumns({ "Item", "Path" });
	fillItemsTable();
}

void CreateProjectDialog::fillItemsTable()
{

}

void CreateProjectDialog::onOkClick()
{
	close(Dialog::RESULT_OK);
}
void CreateProjectDialog::onCancelClick()
{
	modifiedProject = inputProject;
	close(Dialog::RESULT_CANCEL);
}
void CreateProjectDialog::onAddClick()
{

}
void CreateProjectDialog::onDeleteClick()
{

}
void CreateProjectDialog::onEditClick()
{

}

void CreateProjectDialog::setInputProjectData(const boost::property_tree::ptree& p_project)
{
	inputProject = p_project;
	modifiedProject = inputProject;
}

boost::property_tree::ptree CreateProjectDialog::getResultProject()
{
	return modifiedProject;
}

}
