#version 330

in vec3 Color;

uniform vec3 SolidColor = vec3(-1,-1,-1);
uniform bool ComplementOn = true;

out vec4 Fragment;

void main()
{
	Fragment = vec4(Color,1);

	//3. CREATE CONDITIONAL FOR WHEN BOOL IS TRUE, GET COMPLEMENT OF "COLOR", SET THAT TO FRAGMENT OUPUT
	if (SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
		Fragment = vec4(SolidColor, 1);
	else
	{
		if (ComplementOn == true)
			Fragment = vec4((1.0f - Color.r), (1.0f - Color.g), (1.0f - Color.b), 1);
	}

	return;
}