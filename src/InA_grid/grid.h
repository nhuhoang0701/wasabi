#pragma once

namespace calculator
{
	class Cube;
	class Grid
	{
	public:
		Grid(const Cube& cube);
	private:

		const Cube& m_cube;
	};
} // grid
