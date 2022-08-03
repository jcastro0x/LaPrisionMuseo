// Copyright (c) 2022 Javier Castro - jcastro0x@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

uniform sampler2D mask_texture;
uniform sampler2D textures[4];

uniform float textures_intensity[4];
uniform float pan_velocity[4];

uniform float time;

/**
 * Calculate texture coord to do a ping pong effect
 */
vec2 ping_pong(vec2 texCoord, int index)
{
    float x = texCoord.x + abs(sin(time * pan_velocity[index])) * 0.75;
    return vec2(x, texCoord.y);
}


void main()
{
    vec4 maskPixel = texture2D(mask_texture, gl_TexCoord[0].xy);

    vec4 tex0Pixel = texture2D(textures[0], ping_pong(gl_TexCoord[0].xy, 0));
    vec4 tex1Pixel = texture2D(textures[1], ping_pong(gl_TexCoord[0].xy, 1));
    vec4 tex2Pixel = texture2D(textures[2], ping_pong(gl_TexCoord[0].xy, 2));
    vec4 tex3Pixel = texture2D(textures[3], ping_pong(gl_TexCoord[0].xy, 3));

    vec4 t0 = maskPixel.r         * textures_intensity[0] * tex0Pixel * maskPixel.a;
    vec4 t1 = maskPixel.g         * textures_intensity[1] * tex1Pixel * maskPixel.a;
    vec4 t2 = maskPixel.b         * textures_intensity[2] * tex2Pixel * maskPixel.a;
    vec4 t3 = (1.0 - maskPixel.a) * textures_intensity[3] * tex3Pixel;

    gl_FragColor = gl_Color * t0 + t1 + t2 + t3;
    //gl_FragColor = gl_Color * t0;
}

//vec4 t2 = clamp((maskPixel.b * texture2_intensity * tex2Pixel) - (1.0 - maskPixel.a), 0, 1);
