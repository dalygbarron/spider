#ifndef SHADERS_H
#define SHADERS_H

/**
 * Just an offshoot of the const namespace basically because I was sick of
 * recompiling a million files to change these shaders.
 */
namespace Shaders {
    static char const *COLOUR_SHADER = R"~~~(
        out vec3 color;
        void main() {
          color = vec3(1,0,0);
        }
    )~~~";
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

