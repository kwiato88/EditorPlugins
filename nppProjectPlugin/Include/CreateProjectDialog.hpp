#pragma once

#include <boost/property_tree/ptree.hpp>
#include "Dialog.hpp"
#include "ControlGrid.hpp"
#include "ControlCheckBox.hpp"
#include "ControlEdit.hpp"

namespace WinApi
{

class CreateItemDialog : public Dialog
{
public:
	CreateItemDialog(InstanceHandle p_hInstance, Handle p_parent);

	void setInputItem(const boost::property_tree::ptree& p_item);
	void setDefaultInputItem();
	boost::property_tree::ptree getResultItem();

private:
	void onInit() override;
	void onBrowseClick();
	void onOkClick();
	void onCancelClick();
	boost::property_tree::ptree buildModifiedItem();

	Control::CheckBox tagsGeneration;
	Control::CheckBox tagsNavigation;
	Control::CheckBox includesBrowsing;
	Control::CheckBox fileSearching;
	Control::Edit sourcePath;

	boost::property_tree::ptree inputItem;
	boost::property_tree::ptree modifiedItem;
};

class CreateProjectDialog : public Dialog
{
public:
	CreateProjectDialog(InstanceHandle p_hInstance, Handle p_parent);
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
};

}
