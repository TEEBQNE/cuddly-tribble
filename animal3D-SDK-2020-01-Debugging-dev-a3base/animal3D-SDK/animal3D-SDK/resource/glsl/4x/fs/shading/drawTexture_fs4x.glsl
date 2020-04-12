/*
	Copyright 2011-2020 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	drawTexture_fs4x.glsl
	Draw texture sample.
*/

#version 430

layout (location = 0)	in vec4 vTexcoord;

layout (location = 64)	uniform vec4 uColor;
layout (binding = 0)	uniform sampler2D uTex;

layout (location = 0)	out vec4 rtFragColor;
layout (location = 3)	out vec4 rtTexcoord;
layout (location = 4)	out vec4 rtTexSample;


void main()
{
	vec4 sample_tex = texture(uTex, vTexcoord.xy);
	rtFragColor = sample_tex * uColor;
	rtTexcoord = vTexcoord;
	rtTexSample = sample_tex;
}
