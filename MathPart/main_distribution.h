#ifndef MAIN_DISTRIBUTION_H_
#define MAIN_DISTRIBUTION_H_

#include <fstream>
#include <string>
#include <random>
#include <optional>

#include "parameters.h"

namespace statistics {

	class MainDistribution {
		// Класс, реализующий основное распределение
	public:

		explicit MainDistribution() : _dist_params({ 1.0,0.0,1.0 }) {};
		explicit MainDistribution(const Parameters &inParams) : _dist_params(inParams) {};
		explicit MainDistribution(std::ifstream& inFile);
		MainDistribution(double inV, double inMu, double inLambda) : _dist_params({ inV, inMu, inLambda }) {};

		inline double get_v() const { return _dist_params.v; };
		inline double get_mu() const { return _dist_params.mu; };
		inline double get_lambda() const { return _dist_params.lambda; };

		inline void set_v(double inV) { _dist_params.v = inV; };
		inline void set_mu(double inMu) { _dist_params.mu = inMu; };
		inline void set_lambda(double inLambda) { _dist_params.lambda = inLambda; };

		double get_density_distribution(double inX) const;

		std::optional<double> get_expected_value() const;
		std::optional<double> get_dispersion() const;
		std::optional<double> get_krutosis() const;

		double generate_random_variable() const;

		void write_to_file(std::ofstream& outFile) const;
		void read_from_file(std::ifstream& inFile);

		friend std::ostream& operator<<(std::ostream& out, const MainDistribution& dist) {
			// Оператор вывода текущего состояния объекта
			out << "{ v = " << dist._dist_params.v << ", mu = " << dist._dist_params.mu << ", lambda = " << dist._dist_params.lambda << " }";
			return out;
		};

		MainDistribution& operator=(MainDistribution& dist) {
			// Проверяем на самоприсваивание
			if (this == &dist) {
				return *this;
			}

			this->_dist_params.v = dist._dist_params.v;
			this->_dist_params.mu = dist._dist_params.mu;
			this->_dist_params.lambda = dist._dist_params.lambda;

			return *this;
		};

	private:

		// Параметры распределения
		Parameters _dist_params;

	};

}; // namespace statistics

#endif