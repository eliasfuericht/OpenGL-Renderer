#pragma once


#include "Interpolation.h"


class BezierCurve : public Interpolation
{
		public:
		BezierCurve() = default;
		// Initializes a instance with a set of control points
		BezierCurve(std::vector<glm::vec3> pControlPoints);
		BezierCurve(BezierCurve&&) = default;
		BezierCurve(const BezierCurve&) = default;
		BezierCurve& operator=(BezierCurve&&) = default;
		BezierCurve& operator=(const BezierCurve&) = default;
		~BezierCurve() = default;

		////Connect Beziercurves

		//BezierCurve connect_curves(std::vector<BezierCurve> bezierCurves);

		glm::vec3 value_at(float t) override;

		glm::vec3 slope_at(float t) override;
};

