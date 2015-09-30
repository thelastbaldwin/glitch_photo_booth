/**
 * @author felixturner / http://airtight.cc/
 *
 * RGB Shift Shader
 * Shifts red and blue channels from center in opposite directions
 * Ported from http://kriss.cx/tom/2009/05/rgb-shift/
 * by Tom Butterworth / http://kriss.cx/tom/
 * adapted to OpenFrameworks by Steve Minor
 *
 * amount: shift distance (1 is width of input)
 * angle: shift angle in radians
 */

#version 410

uniform sampler2D tDiffuse;
uniform float amount;
uniform float angle;
in vec2 vUv;

out vec4 outputColor;

void main() {
	vec2 offset = amount * vec2( cos(angle), sin(angle));
	vec4 cr = texture(tDiffuse, vUv + offset);
	vec4 cga = texture(tDiffuse, vUv);
	vec4 cb = texture(tDiffuse, vUv - offset);
	outputColor = vec4(cr.r, cga.g, cb.b, cga.a);
}