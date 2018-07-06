#pragma once

#include <boost/property_tree/ptree.hpp>
#include <functional>
#include "Dialog.hpp"
#include "ControlGrid.hpp"
#include "ControlCheckBox.hpp"
#include "ControlEdit.hpp"

namespace WinApi
{

typedef std::function<void(const boost::property_tree::ptree& p_item)> ValidateItem;

class CreateItemDialog : public Dialog
{
public:
	CreateItemDialog(InstanceHandle p_hInstance, Handle p_parent, ValidateItem p_validator);

	void setInputItem(const boost::property_tree::ptree& p_item);
	void setDefaultInputItem();
	boost::property_tree::ptree getResultItem();

private:
	void onInit() override;
	void onBrowseClick();
	void onOkClick();
	void onCancelClick();
	void updateModifiedItem();

	Control::CheckBox tagsGeneration;
	Control::CheckBox tagsNavigation;
	Control::CheckBox includesBrowsing;
	Control::CheckBox fileSearching;
	Control::Edit sourcePath;

	boost::property_tree::ptree inputItem;
	boost::property_tree::ptree modifiedItem;
	ValidateItem validator;
};

class CreateProjectDialog : public Dialog
{
public:
	CreateProjectDialog(InstanceHandle p_hInstance, Handle p_parent, ValidateItem p_validator);
	void setInputProjectData(const boost::property_tree::ptree& p_project);
	boost::property_tree::ptree getResultProject();

private:
	void onInit() override;
	void onOkClick();
	void onCancelClick();
	void onAddClick();
	void onDeleteClick();
	void onEditClick();

	void fillItemsTable();
	boost::property_tree::ptree buildModifiedItemsTree();
	boost::property_tree::ptree editItem(const boost::property_tree::ptree& p_item);

	Control::RcFileGrid itemsPaths;

	boost::property_tree::ptree inputProject;
	boost::property_tree::ptree modifiedProject;
	std::vector<boost::property_tree::ptree> items;
	ValidateItem validator;
};

}
