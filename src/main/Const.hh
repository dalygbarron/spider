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
    static int const WIDTH = 1024;
    static int const HEIGHT = 600;
    static int const HALF_WIDTH = 512;
    static int const HALF_HEIGHT = 300;
    static sf::Vector2i const MOUSE_ORIGIN(
        Const::HALF_WIDTH,
        Const::HALF_HEIGHT
    );
    static int const MAX_BULLETS = 1024;
    static int const MAX_ACTORS = 32;
    static float const PI = 3.141592653589793238462643383279;
    static float const DOUBLE_PI = 6.283185307179586;
    static float const HALF_PI = 1.570796326794897;
    static float const FOV_X = 2.094395102;
    static float const FOV_Y = 1.570796327;
    static float const RENDER_LENGTH_X = 1.732050807;
    static float const RENDER_LENGTH_Y = 1;
    static float const INVERSE_RENDER_LENGTH_X = 0.577350269;
    static float const INVERSE_RENDER_LENGTH_Y = 1;
    static float const RADIAN_CONVERT = 57.2957795131;
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
                     ##           Created By Dany Burton
                      ##)~~~";
    static char const *SKY_SHADER = R"~~~(
        #define PI 3.14159265

        uniform sampler2D picture;
        uniform vec2 angle;

        float hfovDegrees = 2.094395102;
        float vfovDegrees = 1.570796327;

        //tools
        vec3 rotateXY(vec3 p, vec2 angle) {
            vec2 c = cos(angle), s = sin(angle);
            p = vec3(p.x, c.x*p.y + s.x*p.z, -s.x*p.y + c.x*p.z);
            return vec3(c.y*p.x + s.y*p.z, p.y, -s.y*p.x + c.y*p.z);
        }

        void main() {
            vec2 uv = gl_TexCoord[0].xy - vec2(0.5, 0.5);
            vec3 camDir = normalize(vec3(uv.xy * vec2(tan(0.5 * hfovDegrees), tan(0.5 * vfovDegrees)), 1.0));
            vec3 rd = normalize(rotateXY(camDir, angle.yx));
            vec2 texCoord = vec2(atan(rd.z, rd.x) + PI, acos(-rd.y)) / vec2(2.0 * PI, PI);
            gl_FragColor = texture2D(picture, texCoord);
        })~~~";
    static char const *TRANSITION_SHADER = R"~~~(
        uniform sampler2D picture;
        uniform float power;
        uniform vec3 colour;

        float avg(vec3 col) {
            return (col.r + col.g + col.b) / 3.0;
        }

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float alpha = 0.0;
            if (power >= avg(texture2D(picture, uv).rgb)) alpha = 1.0;
            gl_FragColor = vec4(0.0, 0.0, 0.0, alpha);
        } 
    )~~~";
};


#endif
