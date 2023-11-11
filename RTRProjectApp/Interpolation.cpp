#include "Interpolation.h"


	Interpolation::Interpolation(std::vector<glm::vec3> pControlPoints)
		: mControlPoints{ std::move(pControlPoints) }
	{

	}

	void Interpolation::set_control_points(std::vector<glm::vec3> pControlPoints)
	{
		mControlPoints = std::move(pControlPoints);
	}

	float Interpolation::distance_between_control_points(size_t first, size_t second)
	{
		assert(first >= 0 && first < num_control_points() - 1 && second >= 1 && second < num_control_points());
		return glm::distance(mControlPoints[first], mControlPoints[second]);
	}

	float Interpolation::squared_distance_between_control_points(size_t first, size_t second)
	{
		assert(first >= 0 && first < num_control_points() - 1 && second >= 1 && second < num_control_points());
		auto d = mControlPoints[second] - mControlPoints[first];
		return glm::dot(d, d);
	}

	float Interpolation::arc_length_between_adjacent_control_points(size_t first)
	{
		assert(first >= 0 && first < num_control_points() - 1);
		// An (for most curves obviously wrong) default implementation:
		return distance_between_control_points(first, first + 1);
	}

	float Interpolation::arc_length_between_control_points(size_t first, size_t second)
	{
		assert(first >= 0 && first < num_control_points() - 1 && second >= 1 && second < num_control_points());
		float sum = 0.0f;
		for (size_t i = first; i < second; ++i)
		{
			sum += arc_length_between_adjacent_control_points(i);
		}
		return sum;
	}

	float Interpolation::arc_length()
	{
		return arc_length_between_control_points(0, num_control_points() - 1);
	}
