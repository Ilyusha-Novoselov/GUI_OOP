#include "mixture_distribution.h"
#include "spec_func.hpp"

namespace statistics {

	MixtureDistribution::MixtureDistribution(std::ifstream& inFile) 
	{
		read_from_file(inFile);
	}

	double MixtureDistribution::get_density_distribution(double inX) const
	{
		return (1 - _p) * _main_dist_1.get_density_distribution(inX) + _p * _main_dist_2.get_density_distribution(inX);
	}

	std::optional<double> MixtureDistribution::get_expected_value() const
	{
		auto M1 = _main_dist_1.get_expected_value();
		auto M2 = _main_dist_2.get_expected_value();
		if (!M1 || !M2)
			return std::nullopt;
		return  std::optional<double>((1 - _p) * (*M1) + _p * (*M2));
	}

	std::optional<double> MixtureDistribution::get_dispersion() const
	{
		auto M1 = _main_dist_1.get_expected_value();
		auto M2 = _main_dist_2.get_expected_value();
		auto D1 = _main_dist_1.get_dispersion();
		auto D2 = _main_dist_2.get_dispersion();
		if (!M1 || !M2 || !D1 || !D2)
			return std::nullopt;
		auto ME = this->get_expected_value();
		return  std::optional<double>(((1 - _p) * (pow(*M1, 2) + *D1) + _p * (pow(*M2, 2) + *D2)) - pow(*ME, 2));
	}

	std::optional<double> MixtureDistribution::get_asymmetry() const
	{
		auto M1 = _main_dist_1.get_expected_value();
		auto M2 = _main_dist_2.get_expected_value();
		auto D1 = _main_dist_1.get_dispersion();
		auto D2 = _main_dist_2.get_dispersion();
		if (!M1 || !M2 || !D1 || !D2)
			return std::nullopt;
		auto ME = this->get_expected_value();
		auto DE = this->get_dispersion();
		return std::optional<double>((1 / pow(*DE, 1.5)) * ((1 - _p) * (pow(*M1 - *ME, 3) + 3 * (*M1 - *ME) * *D1) + _p * (pow(*M2 - *ME, 3) + 3 * (*M2 - *ME) * *D2)));
	}

	std::optional<double> MixtureDistribution::get_krutosis() const
	{
		auto M1 = _main_dist_1.get_expected_value();
		auto M2 = _main_dist_2.get_expected_value();
		auto D1 = _main_dist_1.get_dispersion();
		auto D2 = _main_dist_2.get_dispersion();
		auto Ex1 = _main_dist_1.get_krutosis();
		auto Ex2 = _main_dist_2.get_krutosis();
		if (!M1 || !M2 || !D1 || !D2 || !Ex1 || !Ex2)
			return std::nullopt;
		auto ME = this->get_expected_value();
		auto DE = this->get_dispersion();
		double sum_1 = pow(*M1 - *ME, 4) + 6 * pow(*M1 - *ME, 2) * *D1 + pow(*D1, 2) * (*Ex1 + 3);
		double sum_2 = pow(*M2 - *ME, 4) + 6 * pow(*M2 - *ME, 2) * *D2 + pow(*D2, 2) * (*Ex2 + 3);
		double result = ((1 / pow(*DE, 2)) * ((1 - _p) * sum_1 + _p * sum_2)) - 3;
		return  std::optional<double>(result);
	}

	double MixtureDistribution::generate_random_variable() const
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> rand_dist(std::nextafter(0.0, 1.0), std::nextafter(1.0, 0.0));
		double r = rand_dist(gen);
		if (r > _p)
			return _main_dist_1.generate_random_variable();
		else
			return _main_dist_2.generate_random_variable();
	}

	void MixtureDistribution::write_to_file(std::ofstream& outFile) const
	{
		if (outFile.is_open()) {
			_main_dist_1.write_to_file(outFile);
			_main_dist_2.write_to_file(outFile);
			outFile << _p << std::endl;
			outFile.close();
		}
	}

	void MixtureDistribution::read_from_file(std::ifstream& inFile)
	{
		std::string v, mu, lambda, p;

		if (inFile.is_open()) {
			MainDistribution main_1(inFile);
			_main_dist_1 = main_1;

			MainDistribution main_2(inFile);
			_main_dist_2 = main_2;

			std::getline(inFile, p);
			_p = std::stod(p);
			if (!(_p >= 0 && _p <= 1))
				throw 1.0;
			inFile.close();
		}
	}

} // namespace statistics