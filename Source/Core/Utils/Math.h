#pragma once
namespace Math
{
	const float Deg2Rad = glm::pi<float>() / 180.f;
	const float Rad2Deg = 180.f / glm::pi<float>();

	/**	Euler To Quaternion
	*******************************************************************************/
	inline glm::quat EulerToQuat( const glm::vec3& euler )
	{
		return glm::quat( glm::yawPitchRoll( euler.y * Deg2Rad, euler.x * Deg2Rad, euler.z * Deg2Rad ) );
	}

	/**	Quaternion to Euler
	*******************************************************************************/
	inline glm::vec3 QuatToEuler( const glm::quat& quat )
	{
		glm::vec3 euler = glm::eulerAngles( quat );
		euler *= Rad2Deg;

		return euler;
}

	/**	Make 2D
	*******************************************************************************/
	__forceinline glm::vec2 Make2D( const glm::vec3& vector )
	{
		return glm::vec2( vector.x, vector.z );
	}

	/**	Make 3D
	*******************************************************************************/
	__forceinline glm::vec3 Make3D( const glm::vec2& vector, float ycomp = 0.f )
	{
		return glm::vec3( vector.x, ycomp, vector.y );
	}

	/**	FPlane
	*******************************************************************************/
	struct FPlane
	{
		static FPlane PointNormal( const glm::vec3& point, const glm::vec3& normal )
		{
			return FPlane( normal, glm::dot( point, normal ) );
		}

		FPlane() : normal(), position( 0.f ) {}
		FPlane( const glm::vec3& normal, const float position ) : normal( normal ), position( position ) {}

		glm::vec3 normal;
		float position;
	};

	/**	FRay
	*******************************************************************************/
	struct FRay
	{
		static FRay FromTo( const glm::vec3& from, const glm::vec3& to )
		{
			FRay result;
			result.origin = from;
			result.direction = normalize( to - from );

			return result;
		}

		glm::vec3 origin;
		glm::vec3 direction;

		glm::vec3 operator*( float scalar )
		{
			return origin + direction * scalar;
		}

		glm::vec3 Intersect( const FPlane& FPlane )
		{
			float dis = glm::dot( origin, FPlane.normal ) - FPlane.position;
			float dir = -glm::dot( direction, FPlane.normal );

			return *this * ( dis / dir );
		}
	};
}