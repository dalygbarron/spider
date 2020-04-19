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
                     ##              Created By Liquid Pig Studios
                      ##                       www.liquidpigstudios.com
)~~~";
    static char const *SKY_SHADER = R"~~~(
        #ifdef GL_ES
        precision mediump float;
        #endif

        #define PI 3.14159265

        uniform sampler2D texture;
        uniform vec2 offset;
        uniform vec2 resolution;
        uniform int time;
        uniform mat4 camera;

        vec2 fov = vec2(2.094395102, 1.570796327);

        void main() {
            vec2 uv = gl_FragCoord.xy / resolution - vec2(0.5, 0.5);
            vec2 angle = uv * fov;
            vec3 point = vec3(
                cos(angle.y) * cos(angle.x),
                sin(angle.y),
                cos(angle.y) * sin(angle.x)
            );
            vec3 cameraPoint = camera * vec4(point, 1.0);
            vec2 cameraAngle = vec2(
                atan(cameraPoint.z / cameraPoint.x),
                asin(cameraPoint.y)
            );
            gl_FragColor = vec4(mod(cameraAngle.x, 1.0), mod(cameraAngle.y, 1.0), 0.0, 1.0);//texture2D(texture, cameraAngle / fov);
        })~~~";
    static char const *TRANSITION_SHADER = R"~~~(
        #ifdef GL_ES
        precision mediump float;
        #endif

        uniform sampler2D texture;
        uniform float power;
        uniform vec3 colour;

        float avg(vec3 col) {
            return (col.r + col.g + col.b) / 3.0;
        }

        void main() {
            vec2 uv = gl_TexCoord[0].xy;
            float alpha = 0.0;
            if (power >= avg(texture2D(texture, uv).rgb)) alpha = 1.0;
            gl_FragColor = vec4(0.0, 0.0, 0.0, alpha);
        })~~~";
    static char const *BLANK_SHADER = R"~~~(
        #ifdef GL_ES
        precision mediump float;
        #endif

        uniform int time;

        void main() {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        })~~~";
};

#endif
