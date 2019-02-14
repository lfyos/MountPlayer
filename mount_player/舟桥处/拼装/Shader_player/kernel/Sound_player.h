#pragma once


// Sound_player 

class Sound_player
{
public:
	static bool sound_state();
	static void turn_sound_on_off();
	static bool play(CString file_name);
	
	~Sound_player()
	{
		turn_sound_on_off();
	}
};
