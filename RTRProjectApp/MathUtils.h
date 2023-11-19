#pragma once

#include <glm\glm.hpp>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <numeric>
#include <cstdlib>



	/// <summary>
	/// Calculate the factorial of an integer, has no kind of optimization 
	/// like memoizing values or suchlike => use this method for small numbers
	/// </summary>
	template <typename T>
	T factorial(T n)
	{
		T f = 1;
		for (int i = 2; i <= n; ++i) {
			f = f * i;
		}
		return f;
	}

	/**	Calculates the binomial coefficient, a.k.a. n over k
	 */
	template <typename T>
	T binomial_coefficient(T n, T k)
	{
		// return n.Factorial() / ((n - k).Factorial() * k.Factorial());

		// Optimized method, see http://stackoverflow.com/questions/9619743/how-to-calculate-binomial-coefficents-for-large-numbers			
		// (n C k) and (n C (n-k)) are the same, so pick the smaller as k:
		if (k > n - k) {
			k = n - k;
		}
		T result = 1;
		for (T i = 1; i <= k; ++i)
		{
			result *= n - k + i;
			result /= i;
		}
		return result;
	}

	/// <summary>
	/// Calculates the bernstein polynomial b_{i,n}(t)
	/// </summary>
	/// <returns>
	/// The polynomial.
	/// </returns>
	/// <param name='i'>
	/// The i parameter
	/// </param>
	/// <param name='n'>
	/// The n parameter
	/// </param>
	/// <param name='t'>
	/// t
	/// </param>
	template <typename T, typename P>
	P bernstein_polynomial(T i, T n, P t)
	{
		return static_cast<P>(binomial_coefficient(n, i) * glm::pow(t, i) * glm::pow(P(1) - t, n - i));
	}

	// Returns a quaternion such that q*start = dest
	// Sources: 
	//  1) "The Shortest Arc Quaternion" in Game Programming Gems 1, by Stan Melax
	//  2) opengl-tutorial, Tutorial 17 : Rotations, accessed 26.06.2019,
	//	   http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
	extern glm::quat rotation_between_vectors(glm::vec3 v0, glm::vec3 v1);

