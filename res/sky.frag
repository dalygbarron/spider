#define PI 3.14159265
#define DEG2RAD 0.01745329251994329576923690768489

uniform sampler2D picture;
uniform vec2 angle;

float hfovDegrees = 120.0;
float vfovDegrees = 60.0;

//tools
vec3 rotateXY(vec3 p, vec2 angle) {
    vec2 c = cos(angle), s = sin(angle);
    p = vec3(p.x, c.x*p.y + s.x*p.z, -s.x*p.y + c.x*p.z);
    return vec3(c.y*p.x + s.y*p.z, p.y, -s.y*p.x + c.y*p.z);
}

void main() {
    vec2 uv = gl_TexCoord[0].xy - vec2(0.5, 0.5);
    vec3 camDir = normalize(vec3(uv.xy * vec2(tan(0.5 * hfovDegrees * DEG2RAD), tan(0.5 * vfovDegrees * DEG2RAD)), 1.0));
    vec3 rd = normalize(rotateXY(camDir, angle.yx));
    vec2 texCoord = vec2(atan(rd.z, rd.x) + PI, acos(-rd.y)) / vec2(2.0 * PI, PI);
    gl_FragColor = texture(picture, texCoord);
}
