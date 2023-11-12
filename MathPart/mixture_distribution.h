#ifndef MIXTURE_DISTRIBUTION_H_
#define MIXTURE_DISTRIBUTION_H_

#include "main_distribution.h"

namespace statistics {
	class MixtureDistribution {
		// Класс, реализующий смесь распределений
	public:

		explicit MixtureDistribution() : _main_dist_1(), _main_dist_2(), _p(0.5) {};
		explicit MixtureDistribution(const MainDistribution& main_dist_1, const MainDistribution& main_dist_2, double p) : _main_dist_1(main_dist_1), _main_dist_2(main_dist_2), _p(p > 0 && p < 1 ? p : throw 1.0) {};
		explicit MixtureDistribution(std::ifstream& inFile);

		inline MainDistribution& get_main_dist_1() { return _main_dist_1; };
		inline MainDistribution& get_main_dist_2() { return _main_dist_2; };
		inline double get_p() const { return _p; };

		inline void set_main_dist_1(MainDistribution& main_dist_1) { _main_dist_1 = main_dist_1; };
		inline void set_main_dist_2(MainDistribution& main_dist_2) { _main_dist_2 = main_dist_2; };
		inline void set_p(double p) { _p = p; };

		double get_density_distribution(double inX) const;

		std::optional<double> get_expected_value() const;
		std::optional<double> get_dispersion() const;
		std::optional<double> get_asymmetry() const;
		std::optional<double> get_krutosis() const;

		double generate_random_variable() const;

		friend std::ostream& operator<<(std::ostream& out, const MixtureDistribution& dist) {
			// Оператор вывода текущего состояния объекта
			out << "[ f1(x) = " << dist._main_dist_1 << ", f2(x) = " << dist._main_dist_2 << ", p = " << dist._p << " ]";
			return out;
		};

		MixtureDistribution& operator=(MixtureDistribution& dist) {
			// Проверяем на самоприсваивание
			if (this == &dist) {
				return *this;
			}

			this->_main_dist_1 = dist._main_dist_1;
			this->_main_dist_2 = dist._main_dist_2;
			this->_p = dist._p;

			return *this;
		};

		void write_to_file(std::ofstream& outFile) const;
		void read_from_file(std::ifstream& inFile);
	
	private:

		MainDistribution _main_dist_1;
		MainDistribution _main_dist_2;
		double _p;
	};
} // namespace statistics

#endif