#pragma once

#include <boost/property_tree/ptree.hpp>
#include "Dialog.hpp"
#include "ControlGrid.hpp"

namespace WinApi
{

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
