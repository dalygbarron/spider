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
        #define HALF_PI 1.5707963267
        #define PI 3.14159265
        #define DOUBLE_PI 6.28318530718
        uniform sampler2D texture;
        uniform vec2 offset;
        uniform vec2 resolution;
        uniform int time;
        uniform mat4 camera;
        uniform vec2 fov;

        void main() {
            vec2 uv = gl_FragCoord.xy / resolution - vec2(0.5, 0.5);
            vec4 point = vec4(
                normalize(vec3(uv.x * fov.x, -uv.y * fov.y, 0.9)),
                0
            );
            vec4 cameraPoint = camera * point;
            vec2 cameraAngle = vec2(
                atan(cameraPoint.z, cameraPoint.x) + HALF_PI,
                acos(cameraPoint.y)
            );
            gl_FragColor = texture2D(texture, cameraAngle / vec2(DOUBLE_PI, PI));
        })~~~";
    static char const *WORLD_SHADER = R"~~~(
        #ifdef GL_ES
        precision mediump float;
        #endif
        #define QUARTER_PI 0.7853981633
        #define HALF_PI 1.5707963267
        #define PI 3.14159265
        #define DOUBLE_PI 6.28318530718
        uniform sampler2D texture;
        uniform vec2 offset;
        uniform vec2 resolution;
        uniform int time;
        uniform mat4 camera;
        uniform vec2 fov;
        uniform vec3 position;
        uniform float waves;

        void main() {
            vec2 uv = gl_FragCoord.xy / resolution - vec2(0.5, 0.5);
            vec4 point = vec4(
                normalize(vec3(-uv.x * fov.x, -uv.y * fov.y, 0.9)),
                0
            );
            vec4 cameraPoint = camera * point;
            vec2 cameraAngle = vec2(
                atan(cameraPoint.z, cameraPoint.x) + HALF_PI,
                acos(cameraPoint.y)
            );
            if (cameraAngle.y >= HALF_PI) {
                float distance = position.y * tan(cameraAngle.y);
                vec2 groundPoint = vec2(
                    position.x + cos(cameraAngle.x + HALF_PI) * distance,
                    position.z + sin(cameraAngle.x + HALF_PI) * distance
                );
                groundPoint.x += sin(float(time) / 60.0 + groundPoint.y) * waves;
                groundPoint.y += sin(float(time) / 60.0 + groundPoint.x) * waves;
                gl_FragColor = texture2D(texture, groundPoint * 0.1);
            } else {
                gl_FragColor = mix(
                    vec4(0.0, 0.1, 0.3, 1.0),
                    vec4(0.4, 0.6, 0.7, 1.0),
                    cameraAngle.y / 2.0 / QUARTER_PI
                );
            }
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

