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

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;

uniform float texture0_intensity;
uniform float texture1_intensity;
uniform float texture2_intensity;
uniform float texture3_intensity;

uniform float time;
uniform float pan_velocity;

/**
 * Calculate texture coord to do a ping pong effect
 */
vec2 ping_pong(vec2 texCoord)
{
    float x = texCoord.x + abs(sin(time * pan_velocity)) * 0.75;
    return vec2(x, texCoord.y);
}


void main()
{
    vec4 maskPixel = texture2D(mask_texture, gl_TexCoord[0].xy);

    vec4 tex0Pixel = texture2D(texture0, ping_pong(gl_TexCoord[0].xy));
    vec4 tex1Pixel = texture2D(texture1, ping_pong(gl_TexCoord[0].xy));
    vec4 tex2Pixel = texture2D(texture2, ping_pong(gl_TexCoord[0].xy));
    vec4 tex3Pixel = texture2D(texture3, ping_pong(gl_TexCoord[0].xy));

    vec4 t0 = maskPixel.r * texture0_intensity * tex0Pixel;
    vec4 t1 = maskPixel.g * texture1_intensity * tex1Pixel;
    //vec4 t2 = clamp((maskPixel.b * texture2_intensity * tex2Pixel) - (1.0 - maskPixel.a), 0, 1);
    vec4 t2 = maskPixel.b * texture2_intensity * tex2Pixel;
    vec4 t3 = (1.0 - maskPixel.a) * texture3_intensity * tex3Pixel;

    vec4 zero_one  = t0 + t1;
    vec4 two_three = t2 + t3;
    vec4 final = zero_one + two_three;

    gl_FragColor = gl_Color * t0 + t1 + t2 + t3;
}
