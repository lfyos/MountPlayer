#pragma once
#include "list_base.h"
#include "../Shader_player.h"


class Assemble_type_list :	public List_base
{
	void file_list_item(Assemble_type *assemble_type);
public:
	virtual void file_list();

	Assemble_type_list(void)
		:List_base(IDR_POPUP_EXPLORER)
	{
	};
	~Assemble_type_list(void)
	{
	};

	afx_msg void OnModify(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};
