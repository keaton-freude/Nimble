// MVP from IShader - Per Frame
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer PerFrameParticleParameters
{
	float2 ViewportScale;
	float CurrentTime;
};

// ParticleSystem constants
cbuffer ConstantParticleParameters
{
	float3 gravity;
	float duration;
	float4 min_color;
	float4 max_color;
	float2 rotate_speed;
	float duration_randomness;
	float end_velocity;
	float2 start_size;
	float2 end_size;
};

struct VertexInputType
{
	float2 Corner : POSITION0;
	float3 Position : POSITION1;
	float3 Velocity : POSITION2;
	float4 Random : COLOR0;
	float Time : TEXCOORD0;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR0;
	float2 TextureCoordinate : COLOR1;
};

// Vertex shader helper for computing the position of a particle.
float4 ComputeParticlePosition(float4 position, float3 velocity,
                               float age, float normalizedAge, float adjustedDuration)
{
    float startVelocity = length(velocity);

    // Work out how fast the particle should be moving at the end of its life,
    // by applying a constant scaling factor to its starting velocity.
    float endVelocity = startVelocity * 1.0f;
    
    // Our particles have constant acceleration, so given a starting velocity
    // S and ending velocity E, at time T their velocity should be S + (E-S)*T.
    // The particle position is the sum of this velocity over the range 0 to T.
    // To compute the position directly, we must integrate the velocity
    // equation. Integrating S + (E-S)*T for T produces S*T + (E-S)*T*T/2.

    float velocityIntegral = startVelocity * normalizedAge +
                             (endVelocity - startVelocity) * normalizedAge *
                                                             normalizedAge / 2;

	position += (float4(velocity, 0.0f) * adjustedDuration) * normalizedAge;
     
    //position += float4(normalize(velocity), 0.0f) * velocityIntegral * adjustedDuration;


    
    // Apply the gravitational force.
    position += float4(gravity, 0.0f) * age * normalizedAge;
    
    // Apply the camera view and projection transforms.
    return mul(mul(position, viewMatrix), projectionMatrix);
}

// Vertex shader helper for computing the color of a particle.
float4 ComputeParticleColor(float4 projectedPosition,
                            float randomValue, float normalizedAge)
{
    // Apply a random factor to make each particle a slightly different color.
    float4 color = lerp(min_color, max_color, normalizedAge);
    
    // Fade the alpha based on the age of the particle. This curve is hard coded
    // to make the particle fade in fairly quickly, then fade out more slowly:
    // plot x*(1-x)*(1-x) for x=0:1 in a graphing program if you want to see what
    // this looks like. The 6.7 scaling factor normalizes the curve so the alpha
    // will reach all the way up to fully solid.
    
    //color.a *= normalizedAge * (1-normalizedAge) * (1-normalizedAge) * 6.7;

	color.a = lerp(min_color.a, max_color.a, normalizedAge);

   
    return color;
}

// Vertex shader helper for computing the size of a particle.
float ComputeParticleSize(float randomValue, float normalizedAge)
{
    // Apply a random factor to make each particle a slightly different size.
    float startSize = lerp(start_size.x, start_size.y, randomValue);
    float endSize = lerp(end_size.x, end_size.y, randomValue);
    
    // Compute the actual size based on the age of the particle.
    float size = lerp(startSize, endSize, normalizedAge);
    
    // Project the size into screen coordinates.
    return size * projectionMatrix._m11;
}

// Vertex shader helper for computing the rotation of a particle.
float2x2 ComputeParticleRotation(float randomValue, float age)
{    
    // Apply a random factor to make each particle rotate at a different speed.
    float rotateSpeed = lerp(rotate_speed.x, rotate_speed.y, randomValue);
    
    float rotation = rotateSpeed * age;

    // Compute a 2x2 rotation matrix.
    float c = cos(rotation);
    float s = sin(rotation);
    
    return float2x2(c, -s, s, c);
}

PixelInputType ParticleVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    // Compute the age of the particle.
    float age = CurrentTime - input.Time;
    
    // Apply a random factor to make different particles age at different rates.
    age *= 1 + input.Random.x * duration_randomness;

	float adjusted_duration = duration - (input.Random.x * duration_randomness);
    
    // Normalize the age into the range zero to one.
    float normalizedAge = saturate(age / duration);

	if (normalizedAge == 1.0f)
	{
		// emit invis
		output.Position = float4(0.0f, 0.0f, 0.0f, 0.0f);
		output.Color = float4(0.0f, 0.0f, 0.0f, 0.0f);
		output.TextureCoordinate = float2(1.0f, 1.0f);
		return output;
	}

    // Compute the particle position, size, color, and rotation.
    output.Position = ComputeParticlePosition(float4(input.Position, 1.0f), input.Velocity,
                                              age, normalizedAge, adjusted_duration);

    float size = ComputeParticleSize(input.Random.y, normalizedAge);
    float2x2 rotation = ComputeParticleRotation(input.Random.w, age);

    output.Position.xy += mul(input.Corner, rotation) * size;
    
    output.Color = ComputeParticleColor(output.Position, input.Random.z, normalizedAge);
    output.TextureCoordinate = (input.Corner + 1) / 2;
    
    return output;
}

