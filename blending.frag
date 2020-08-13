#version 120
#include "texture.h"

uniform float width; // screen width
uniform float height; // screen height
uniform sampler2D color_front;
uniform sampler2D depth_back;
uniform sampler2D color_back;

void main()
{
    vec2 index = gl_FragCoord.xy / vec2( width, height );
    vec4 front_color = LookupTexture2D( color_front, index );
    vec4 back_color = LookupTexture2D( color_back, index );

    float tmp_alpha = 1.0 - front_color.a;
    vec3 color = front_color.rgb + back_color.rgb * tmp_alpha * back_color.a;
    float alpha = front_color.a + tmp_alpha * back_color.a;

    gl_FragColor = vec4( color, alpha );
    gl_FragDepth = LookupTexture2D( depth_back, index ).r;
}
