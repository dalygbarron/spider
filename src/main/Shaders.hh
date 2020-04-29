#ifndef SHADERS_H
#define SHADERS_H

/**
 * Just an offshoot of the const namespace basically because I was sick of
 * recompiling a million files to change these shaders.
 */
namespace Shaders {
    static char const *SKY_SHADER = R"~~~(
        #ifdef GL_ES
        precision mediump float;
        #endif
        #define PI 3.14159265
        #define DOUBLE_PI 6.28318530718
        uniform sampler2D texture;
        uniform vec2 offset;
        uniform vec2 resolution;
        uniform int time;
        uniform mat4 camera;

        vec2 fov = vec2(1.95, 1.3);

        void main() {
            vec2 uv = gl_FragCoord.xy / resolution - vec2(0.5, 0.5);
            vec2 angle = uv * vec2(tan(fov.x * 0.5), tan(fov.y * 0.5));
            vec4 point = vec4(normalize(vec3(-angle.x, -angle.y, 1.0)), 1.0);
            vec4 cameraPoint = camera * point;
            vec2 cameraAngle = vec2(
                atan(cameraPoint.z, cameraPoint.x) + PI,
                acos(cameraPoint.y)
            );
            //gl_FragColor = vec4(mod(gl_FragCoord.x / resolution.x, 1.0), mod(cameraAngle.y, 1.0), 1.0, 1.0);
            gl_FragColor = texture2D(texture, cameraAngle / vec2(DOUBLE_PI, PI));
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

