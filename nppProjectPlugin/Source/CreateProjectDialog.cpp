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
void conditionalyEnable(Control::CheckBox& p_control, const std::string& p_value)
{
	if (p_value == "enabled")
		p_control.enable();
}
std::string toEnbleState(bool p_value)
{
	return p_value ? "enabled" : "disabled";
}
}

CreateItemDialog::CreateItemDialog(InstanceHandle p_hInstance, Handle p_parent)
	: Dialog(p_hInstance, p_parent, ResourceId(ID_CREATE_ITEM_DIALOG), "Edit Item")
{
	registerHandler(MsgMatchers::ButtonClick(IDOK), std::bind(&CreateItemDialog::onOkClick, this));
	registerHandler(MsgMatchers::ButtonClick(IDCANCEL), std::bind(&CreateItemDialog::onCancelClick, this));
	registerHandler(MsgMatchers::ButtonClick(ID_BROWSE_BUTTON), std::bind(&CreateItemDialog::onBrowseClick, this));
}

void CreateItemDialog::setDefaultInputItem()
{
	// TODO: init input with deafult values
	modifiedItem = inputItem;
}

void CreateItemDialog::setInputItem(const boost::property_tree::ptree& p_item)
{
	inputItem = p_item;
	modifiedItem = inputItem;
}

boost::property_tree::ptree CreateItemDialog::getResultItem()
{
	return modifiedItem;
}

void CreateItemDialog::onInit()
{
	tagsGeneration.init(getItem(ResourceId(ID_TAGS_GENERATION)));
	tagsNavigation.init(getItem(ResourceId(ID_TAGS_NAVIGATION)));
	includesBrowsing.init(getItem(ResourceId(ID_INCLUDES_BROWSING)));
	fileSearching.init(getItem(ResourceId(ID_FILES_SEARCHING)));
	sourcePath.init(getItem(ResourceId(ID_ITEM_PATH)));
	
	conditionalyEnable(tagsGeneration, inputItem.get<std::string>("tagsGeneration", ""));
	conditionalyEnable(tagsNavigation, inputItem.get<std::string>("tagsNavigation", ""));
	conditionalyEnable(includesBrowsing, inputItem.get<std::string>("includesBrowsing", ""));
	conditionalyEnable(fileSearching, inputItem.get<std::string>("fileSearching", ""));
	sourcePath.setContent(inputItem.get<std::string>("sourcePath", ""));
}

void CreateItemDialog::onBrowseClick()
{
	//TODO: impl
}

boost::property_tree::ptree CreateItemDialog::buildModifiedItem()
{
	boost::property_tree::ptree data;
	data.put("tagsGeneration", toEnbleState(tagsGeneration.isChecked()));
	data.put("tagsNavigation", toEnbleState(tagsNavigation.isChecked()));
	data.put("includesBrowsing", toEnbleState(includesBrowsing.isChecked()));
	data.put("fileSearching", toEnbleState(fileSearching.isChecked()));
	data.put("sourcePath", sourcePath.getContent());
	// TODO: how to set tag file path?
	//data.put("tagFilePath", ctagsFilePath);
	return data;
}

void CreateItemDialog::onOkClick()
{
	modifiedItem = buildModifiedItem();
	close(Dialog::RESULT_OK);
}

void CreateItemDialog::onCancelClick()
{
	modifiedItem = inputItem;
	close(Dialog::RESULT_CANCEL);
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
	CreateItemDialog dlg(m_hInstance, m_self);
	dlg.setInputItem(p_item);
	dlg.show();
	return dlg.getResultItem();
}

void CreateProjectDialog::onAddClick()
{
	CreateItemDialog dlg(m_hInstance, m_self);
	dlg.setDefaultInputItem();
	dlg.show();
	items.push_back(dlg.getResultItem());
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

//TODO: validate data correctness

}
