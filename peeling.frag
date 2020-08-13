#version 120
#include "shading.h"
#include "qualifire.h"
#include "texture.h"

// Input parameters from vertex shader.
FragIn vec3 position;
FragIn vec3 normal;

// Uniform parameters.
uniform float width; // screen width
uniform float height; // screen height
uniform ShadingParameter shading;
uniform sampler2D depth_front;

void main()
{
    vec2 index = gl_FragCoord.xy / vec2( width, height );
    float d = LookupTexture2D( depth_front, index ).r;
    if ( gl_FragCoord.z <= d ) { discard; return; }

    vec3 color = gl_Color.rgb;

    // Light position in camera coordinate.
    vec3 light_position = gl_LightSource[0].position.xyz;

    // Light vector (L) and Normal vector (N) in camera coordinate.
    vec3 L = normalize( light_position - position );
    vec3 N = normalize( normal );

    // Shading.
#if   defined( ENABLE_LAMBERT_SHADING )
    vec3 shaded_color = ShadingLambert( shading, color, L, N );

#elif defined( ENABLE_PHONG_SHADING )
    vec3 V = normalize( -position );
    vec3 shaded_color = ShadingPhong( shading, color, L, N, V );

#elif defined( ENABLE_BLINN_PHONG_SHADING )
    vec3 V = normalize( -position );
    vec3 shaded_color = ShadingBlinnPhong( shading, color, L, N, V );

#else // DISABLE SHADING
    vec3 shaded_color = ShadingNone( shading, color );
#endif

    gl_FragColor = vec4( shaded_color, gl_Color.a );
}
