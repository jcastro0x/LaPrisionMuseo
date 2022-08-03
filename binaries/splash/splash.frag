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

// Mask used to blend textures
uniform sampler2D mask_texture;

// Textures displayed
uniform sampler2D textures[4];

// Intensity of each texture (0 = black 1 = visible)
uniform float textures_intensity[4];

// Displacement velocity of each texture
uniform float pan_velocity[4];

// Global time (used to calculate displacement velocity)
uniform float time;

/**
 * Calculate texture coord to do a ping pong effect
 */
vec2 ping_pong(int index)
{
    return vec2(
        gl_TexCoord[0].x + abs(sin(time * pan_velocity[index])) * 0.75,
        gl_TexCoord[0].y
    );
}

void main()
{
    vec4 maskPixel = texture2D(mask_texture, gl_TexCoord[0].xy);

    vec4 t0 = maskPixel.r         * textures_intensity[0] * texture2D(textures[0], ping_pong(0)) * maskPixel.a;
    vec4 t1 = maskPixel.g         * textures_intensity[1] * texture2D(textures[1], ping_pong(1)) * maskPixel.a;
    vec4 t2 = maskPixel.b         * textures_intensity[2] * texture2D(textures[2], ping_pong(2)) * maskPixel.a;
    vec4 t3 = (1.0 - maskPixel.a) * textures_intensity[3] * texture2D(textures[3], ping_pong(3));

    gl_FragColor = gl_Color * t0 + t1 + t2 + t3;
}
