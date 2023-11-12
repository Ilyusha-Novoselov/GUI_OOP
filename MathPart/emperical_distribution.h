#ifndef EMPERICAL_DISTRIBUTION_H_
#define EMPERICAL_DISTRIBUTION_H_

#include <map>
#include <sstream>
#include <algorithm>
#include <QDebug>

#include "mixture_distribution.h"

namespace statistics {
	class EmpericalDistribution {
	public:
		explicit EmpericalDistribution() : _N(0), _X(), _function() {};
		explicit EmpericalDistribution(int N, const MainDistribution& dist);
		explicit EmpericalDistribution(int N, const MixtureDistribution& dist);
		explicit EmpericalDistribution(int N, const EmpericalDistribution& dist);
		explicit EmpericalDistribution(std::ifstream& inFile);

		~EmpericalDistribution() { _X.clear(); _function.clear(); };

		inline int get_N() const { return _N; };
		inline std::vector<double> get_X() const { return _X; };
		inline std::map<std::pair<double, double>, double> get_function() const { return _function; };

		inline void set_N(int N) { _N = N; };
		inline void set_X(std::vector<double> X) { _X = X; };
		inline void set_function(std::map<std::pair<double, double>, double> function) { _function = function; };

		double get_density_distribution(double inX) const;

		double get_expected_value() const;
		double get_dispersion() const;
		double get_asymmetry() const;
		double get_krutosis() const;

		std::map<std::pair<double, double>, double> generate_emperical_density_function();
		double generate_random_variable() const;

		friend std::ostream& operator<<(std::ostream& out, const EmpericalDistribution& dist) {
			// Оператор вывода текущего состояния объекта
			out << "{ N = " << dist._N << std::endl << "x_min: " << *std::min_element(dist._X.begin(), dist._X.end()) << " x_max: " << *std::max_element(dist._X.begin(), dist._X.end()) << std::endl;
			out << std::endl << "function:" << std::endl;
			for (auto f : dist._function)
			{
				if (f == *dist._function.rbegin())
					out << "[" << f.first.first << ";" << f.first.second << "]" << " f = " << f.second << " }" << std::endl;
				else
					out << "[" << f.first.first << ";" << f.first.second << ")" << " f = " << f.second << std::endl;
			}
			return out;
		};

        friend QDebug operator<<(QDebug debug, EmpericalDistribution& dist)
        {
            QDebugStateSaver saver(debug);
            for (auto f : dist._function)
            {
                if (f == *dist._function.rbegin())
                    debug.nospace() << "[" << f.first.first << ";" << f.first.second << "]" << " f = " << f.second << " }" << "\n";
                else
                    debug.nospace() << "[" << f.first.first << ";" << f.first.second << ")" << " f = " << f.second << "\n";
            }
            return debug;
        };

		EmpericalDistribution& operator=(EmpericalDistribution& dist) {
			// Проверяем на самоприсваивание
			if (this == &dist) {
				return *this;
			}

			this->_N = dist._N;
			this->_X = dist._X;
			this->_function = dist._function;

			return *this;
		};

		void write_to_file(std::ofstream& outFile) const;
		void read_from_file(std::ifstream& inFile);

	private:
		int _N;
		std::vector<double> _X;
		std::map<std::pair<double, double>, double> _function;
	};
}

#endif
