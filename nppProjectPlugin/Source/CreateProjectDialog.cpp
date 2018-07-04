#include "CreateProjectDialog.hpp"
#include "CreateProjectDialogDef.h"
#include "DialogMsgMatchers.hpp"

namespace WinApi
{
namespace
{
std::string getLastComponentName(const std::string& p_filePath)
{
	size_t l_pos = p_filePath.find_last_of('\\');
	if (l_pos != std::string::npos)
		return p_filePath.substr(l_pos + 1);
	else
		return p_filePath;
}
std::vector<std::string> itemToRow(const boost::property_tree::ptree& p_item)
{
	std::string src = p_item.get<std::string>("sourcePath", "");
	if (src.empty())
		return{ "Item", "source dir not defined" };
	return{ getLastComponentName(src), src };
}
}

CreateProjectDialog::CreateProjectDialog(InstanceHandle p_hInstance, Handle p_parent)
	: Dialog(p_hInstance, p_parent, ResourceId(ID_CREATE_PROJECT_DIALOG), "Edit Project")
{
	registerHandler(MsgMatchers::ButtonClick(IDOK), std::bind(&CreateProjectDialog::onOkClick, this));
	registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&CreateProjectDialog::onCancelClick, this));
	registerHandler(MsgMatchers::ButtonClick(ID_ADD_BUTTON), std::bind(&CreateProjectDialog::onAddClick, this));
	registerHandler(MsgMatchers::ButtonClick(ID_DELETE_BUTTON), std::bind(&CreateProjectDialog::onDeleteClick, this));
	registerHandler(MsgMatchers::KeyDown(itemsPaths, VK_DELETE), std::bind(&CreateProjectDialog::onDeleteClick, this));
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
	std::vector<std::vector<std::string>> rows;
	for (const auto& item : items)
		rows.push_back(itemToRow(item));
	itemsPaths.addRows(rows);
}

boost::property_tree::ptree CreateProjectDialog::buildModifiedItemsTree()
{
	boost::property_tree::ptree modifiedItems;
	for (const auto& item : items)
		modifiedItems.push_back(std::make_pair("", item));
	return modifiedItems;
}

void CreateProjectDialog::onOkClick()
{
	modifiedProject.erase("items");
	modifiedProject.add_child("items", buildModifiedItemsTree());
	close(Dialog::RESULT_OK);
}

void CreateProjectDialog::onCancelClick()
{
	modifiedProject = inputProject;
	close(Dialog::RESULT_CANCEL);
}

boost::property_tree::ptree CreateProjectDialog::editItem(const boost::property_tree::ptree& p_item)
{
	return p_item;
}

void CreateProjectDialog::onAddClick()
{
	items.push_back(editItem(boost::property_tree::ptree()));
	fillItemsTable();
}

void CreateProjectDialog::onDeleteClick()
{
	auto idxToRemove = itemsPaths.getSelectedRowIndex();
	if (idxToRemove >= 0 && idxToRemove < items.size())
	{
		items.erase(items.begin() + idxToRemove);
		fillItemsTable();
	}
}

void CreateProjectDialog::onEditClick()
{
	auto idxToEdit = itemsPaths.getSelectedRowIndex();
	if (idxToEdit >= 0 && idxToEdit < items.size())
	{
		items[idxToEdit] = editItem(items[idxToEdit]);
		fillItemsTable();
	}
}

void CreateProjectDialog::setInputProjectData(const boost::property_tree::ptree& p_project)
{
	inputProject = p_project;
	modifiedProject = inputProject;
	for (const auto& item : modifiedProject.get_child("items"))
		items.push_back(item.second);
}

boost::property_tree::ptree CreateProjectDialog::getResultProject()
{
	return modifiedProject;
}

}
