#include "BezierCurve.h"
#include "MathUtils.h"


	BezierCurve::BezierCurve(std::vector<glm::vec3> pControlPoints)
		: Interpolation{ std::move(pControlPoints) }
	{

	}


	glm::vec3 BezierCurve::value_at(float t)
	{
		uint32_t n = static_cast<uint32_t>(num_control_points()) - 1u;
		glm::vec3 sum(0.0f, 0.0f, 0.0f);
		for (uint32_t i = 0; i <= n; ++i)
		{
			sum += bernstein_polynomial(i, n, t) * control_point_at(i);
		}
		return sum;
	}

	glm::vec3 BezierCurve::slope_at(float t)
	{
		uint32_t n = static_cast<uint32_t>(num_control_points()) - 1u;
		uint32_t nMinusOne = n - 1;
		glm::vec3 sum(0.0f, 0.0f, 0.0f);
		for (uint32_t i = 0; i <= nMinusOne; ++i)
		{
			sum += (control_point_at(i + 1) - control_point_at(i)) * bernstein_polynomial(i, nMinusOne, t);
		}
		return static_cast<float>(n) * sum;
	}

	//BezierCurve BezierCurve::connect_curves(std::vector<BezierCurve> bezierCurves)
	//{
	//	// Evaluate the combined curve
	//	for (int i = 0; i < bezierCurves.size(); ++i) {
	//		for (float t = 0.0; t <= 1.0; t += 0.01) {
	//			if (i < bezierCurves.size() - 1 || t < 1.0) {
	//				glm::vec3 pointOnCurve = bezierCurves[i].value_at(t);
	//			}
	//		}
	//	}
	//}