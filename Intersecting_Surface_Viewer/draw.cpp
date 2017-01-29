#include <python2.7/Python.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <cmath>
#include <stdlib.h>
#include <limits>
#include <string>
#include <cfloat>
//#include <cstdlib>
#include <vector>
#include <algorithm> 

#include <allegro5/allegro.h>
#include "allegro5/allegro_main.h"


//#define al_init()   (al_install_system(ALLEGRO_VERSION_INT, atexit));

int main(int argc, char **argv){
	
   al_init();
   al_create_display(640, 480);
   al_clear_to_color(al_map_rgb_f(1, 1, 0));
   al_flip_display();
   al_rest(5.0);
   return 0;


	return 0;
}


