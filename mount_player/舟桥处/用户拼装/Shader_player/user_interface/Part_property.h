#pragma once


// Part_property
#include "PropertiesWnd.h"
#include "../kernel/Component_collector.h"

class Part_property : public CPropertiesWnd
{
	DECLARE_DYNAMIC(Part_property)

	int stack_top_pointer;
	Part_collector *backward_stack[100];

	void push_part_collector(Part_collector &p)
	{
		if(backward_stack[stack_top_pointer]==NULL)
			backward_stack[stack_top_pointer]=new Part_collector(p);
		else
			backward_stack[stack_top_pointer]->copy_from(p);
		stack_top_pointer++;
		if(stack_top_pointer==(sizeof(backward_stack)/sizeof(backward_stack[0])))
			stack_top_pointer=0;
	}
	void pop_part_collector()
	{
		stack_top_pointer--;
		if(stack_top_pointer<0)
			stack_top_pointer=sizeof(backward_stack)/sizeof(backward_stack[0])-1;
	}
	void display_part_collector_process();
	void clear_list()
	{
		list.RemoveAll();
		SetPropListFont();
		list.EnableHeaderCtrl(FALSE);
		list.EnableDescriptionArea();
		list.SetVSDotNetLook();
		list.MarkModifiedProperties();
	}
	void display_mount_message(Part_collector &collector,int main_step,int sub_step,CString msg,CString sound_file);
	Part_collector *get_stack_top()
	{
		return (stack_top_pointer>0)?backward_stack[stack_top_pointer-1]:backward_stack[sizeof(backward_stack)/sizeof(backward_stack[0])-1];
	}
public:
	Part_collector * display_part_collector(Part_collector &collector)
	{
		push_part_collector(collector);
		clear_list();
		display_part_collector_process();
		return get_stack_top();
	}

	afx_msg void OnVirtualMount();

	afx_msg void OnDisplayAllComponent()
	{
		Part_collector collector(theApp.ws->parts);
		for(int i=theApp.mounter->current_step;i<(theApp.mounter->move_number);i++)
			collector.collect(theApp.mounter->move[i]->comp);
		display_part_collector(collector);
	}

	afx_msg void OnDisplaySelection()
	{
		Part_collector collector(Component_collector_for_selected_component(theApp.ws->root_component),theApp.ws->parts);
		display_part_collector(collector);
		if(collector.box_component_number>0)
			theApp.ws->mount_on_box(collector.component_box,1.5,theApp.time_length);
	}
	afx_msg void OnClearDisplay()
	{
		display_part_collector(Part_collector(theApp.ws->parts));
	}
	afx_msg void OnMarkSelection()
	{
		Part_collector *collector=get_stack_top();
		theApp.redraw_flag=true;
		if(collector!=NULL)
			for(int i=0;i<(collector->component_number);i++)
				collector->component_pointer[i]->set_component_selected();
	}
	afx_msg void OnBackWard()
	{
		pop_part_collector();
		clear_list();
		display_part_collector_process();
		
		Part_collector *collector=get_stack_top();
		if(collector!=NULL)
			if(collector->box_component_number>0)
				theApp.ws->mount_on_box(collector->component_box,1.5,theApp.time_length);
	}
	
	Part_property()
	{
		stack_top_pointer=0;
		for(int i=0;i<(sizeof(backward_stack)/sizeof(backward_stack[0]));i++)
			backward_stack[i]=NULL;
	};
	virtual ~Part_property()
	{
		for(int i=0;i<(sizeof(backward_stack)/sizeof(backward_stack[0]));i++)
			if(backward_stack[i]!=NULL)
				delete (backward_stack[i]);
		stack_top_pointer=0;
	};
protected:

	DECLARE_MESSAGE_MAP()
};


