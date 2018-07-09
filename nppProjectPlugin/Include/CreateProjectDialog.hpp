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
typedef std::function<std::string(const std::string& p_sourcePath)> GetTagFilePath;

class CreateItemDialog : public Dialog
{
public:
	CreateItemDialog(InstanceHandle p_hInstance, Handle p_parent,
		ValidateItem p_validator, GetTagFilePath p_tagFile);

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
	GetTagFilePath tagFile;
};

class CreateProjectDialog : public Dialog
{
public:
	CreateProjectDialog(InstanceHandle p_hInstance, Handle p_parent,
		ValidateItem p_validator, GetTagFilePath p_tagFile);
	void setInputProjectData(const boost::property_tree::ptree& p_project);
	boost::property_tree::ptree getResultProject() const;

private:
	void onInit() override;
	bool showContextMenu(int p_xPos, int p_yPos) override;
	void onOkClick();
	void onCancelClick();
	void onAddClick();
	void onDeleteClick();
	void onEditClick();
	void copyProject() const;
	void copyItem();
	void copyAllItems() const;
	void copyPath();
	void copyAllPaths() const;

	void copyToClipboard(const boost::property_tree::ptree& p_data) const;
	void fillItemsTable();
	void updateModifiedProject();
	boost::property_tree::ptree buildModifiedItemsTree() const;
	boost::property_tree::ptree editItem(const boost::property_tree::ptree& p_item) const;

	Control::RcFileGrid itemsPaths;

	boost::property_tree::ptree inputProject;
	boost::property_tree::ptree modifiedProject;
	std::vector<boost::property_tree::ptree> items;
	ValidateItem validator;
	GetTagFilePath tagFile;
};

}
