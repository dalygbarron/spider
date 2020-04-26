#ifndef CONST_H
#define CONST_H

#include <SFML/Graphics.hpp>

/**
 * Contains constants that should be available everywhere in the program.
 */
namespace Const {
    static int const VERSION_MAJOR = 0;
    static int const VERSION_MINOR = 0;
    static int const VERSION_REV = 0;
    static int const MAX_BULLETS = 1024;
    static int const MAX_ACTORS = 32;
    static float const PI = 3.141592653589793238462643383279;
    static float const DOUBLE_PI = 6.283185307179586;
    static float const HALF_PI = 1.570796326794897;
    static float const FOV = 1.3;
    static float const FRAME_TIME = 0.016666666666667;
    static float const DAINTY_RADIUS = 6;
    static char const *TITLE = "Spider";
    static char const *LOGO = R"~~~(
          #######                          ##
        /       ###              #          ##
       /         ##             ###         ##
       ##        #               #          ##
        ###                                 ##
       ## ###           /###   ###      ### ##    /##  ###  /###
        ### ###        / ###  / ###    ######### / ###  ###/ #### /
          ### ###     /   ###/   ##   ##   #### /   ###  ##   ###/
            ### /##  ##    ##    ##   ##    ## ##    ### ##
              #/ /## ##    ##    ##   ##    ## ########  ##
               #/ ## ##    ##    ##   ##    ## #######   ##
                # /  ##    ##    ##   ##    ## ##        ##
      /##        /   ##    ##    ##   ##    /# ####    / ##
     /  ########/    #######     ### / ####/    ######/  ###
    /     #####      ######       ##/   ###      #####    ###
    |                ##
     \)              ##       Version %d.%d.%d
                     ##              Created By Liquid Pig Studios
                      ##                       www.liquidpigstudios.com
)~~~";
};

#endif
